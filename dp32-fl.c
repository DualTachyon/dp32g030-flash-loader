#include "flash.h"

#define BLOCK_SIZE 512

typedef struct {
	uint32_t Size;
	uint32_t Address;
} SectorInfo_t;

typedef struct {
	uint16_t AlgoVer;
	uint8_t  Name[128];
	uint16_t Type;
	uint32_t BaseAddr;
	uint32_t TotalSize;
	uint32_t PageSize;
	uint32_t Reserved;
	uint8_t  ErasedVal;
	uint8_t  Padding[3];
	uint32_t TimeoutProg;
	uint32_t TimeoutErase;
	SectorInfo_t SectorInfo[2];
} FlashDevice_t;

extern const FlashDevice_t FlashDevice  __attribute__((section(".constdata")));

const FlashDevice_t FlashDevice = {
	0x0101,
	"DP32G030 Internal Flash",
	1,
	0x00000000,
	0x0000F000,
	0x00000200,
	0x00000000,
	0xFF,
	{ 0x00, 0x00, 0x00},
	500,
	500,
	{
		{ 0x00000200, 0x00000000 },
		{ 0xFFFFFFFF, 0xFFFFFFFF },
	}
};

static void WaitNotBusy(void)
{
	while (1) {
		if ((FLASH_ST & FLASH_ST_BUSY_MASK) == FLASH_ST_BUSY_BITS_READY) {
			return;
		}
	}
}

static void WaitForEmpty(void)
{
	while (1) {
		if ((FLASH_ST & FLASH_ST_PROG_BUF_EMPTY_MASK) == FLASH_ST_PROG_BUF_EMPTY_BITS_EMPTY) {
			return;
		}
	}
}

static void FlashLock(void)
{
	FLASH_LOCK = FLASH_LOCK_LOCK_BITS_LOCK;
}

static void FlashUnlock(void)
{
	FLASH_UNLOCK = FLASH_UNLOCK_UNLOCK_BITS_UNLOCK;
}

static void FlashStart(void)
{
	FlashUnlock();

	FLASH_START = FLASH_START_START_BITS_START;
}

static void FlashStop(void)
{
	FLASH_CFG = (FLASH_CFG & ~FLASH_CFG_MODE_MASK) | FLASH_CFG_MODE_VALUE_READ_AHB;
}

static void FlashHalfSector(uint32_t Address, const uint8_t *pBuffer, uint32_t WordCount)
{
	const uint32_t *pWord = (const uint32_t *)pBuffer;
	uint32_t i;

	WaitNotBusy();

	FLASH_CFG = (FLASH_CFG & ~FLASH_CFG_MODE_MASK) | FLASH_CFG_MODE_BITS_PROGRAM;
	FLASH_ADDR = Address >> 2;
	FLASH_WDATA = *pWord++;

	FlashStart();

	for (i = 1; i < WordCount; i++) {
		WaitForEmpty();
		FLASH_WDATA = *pWord++;
	}

	WaitNotBusy();

	FlashStop();
	FlashLock();
}

// --------

int Init(uint32_t Addr, uint32_t Freq, uint32_t Func) __attribute__((section("PrgCode")));
int UnInit(uint32_t Func) __attribute__((section("PrgCode")));
int EraseChip(void) __attribute__((section("PrgCode")));
int EraseSector(uint32_t SectorAddr) __attribute__((section("PrgCode")));
int ProgramPage(uint32_t DestAddr, uint32_t NumBytes, const uint8_t *pSrcBuff) __attribute__((section("PrgCode")));
int CheckBlank(uint32_t Addr, uint32_t NumBytes, uint8_t BlankValue) __attribute__((section("PrgCode")));

int Init(uint32_t Addr, uint32_t Freq, uint32_t Func)
{
	if (FLASH_MASK != 6) {
		FLASH_MASK = 0;
		WaitNotBusy();
		FLASH_MASK = 6;
		WaitNotBusy();
		if (FLASH_MASK != 6) {
			return 1;
		}
	}

	return 0;
}

int UnInit(uint32_t Func)
{
	// restore the MCU

	return 0;
}

int EraseChip(void)
{
	uint16_t i;

	for (i = 0; i < 0xF000; i += 512) {
		int ret = EraseSector(i);

		if (ret) {
			return ret;
		}
	}

	return 0;
}

int EraseSector(uint32_t SectorAddr)
{

	if (SectorAddr % 512 || SectorAddr >= 0xF000) {
		return 1;
	}

	WaitNotBusy();

	FLASH_CFG = (FLASH_CFG & ~FLASH_CFG_MODE_MASK) | FLASH_CFG_MODE_BITS_ERASE;

	FLASH_ADDR = SectorAddr >> 2;

	FlashStart();
	WaitNotBusy();
	FlashStop();
	FlashLock();

	return 0;
}


int ProgramPage(uint32_t DestAddr, uint32_t NumBytes, const uint8_t *pBuffer)
{
	if (DestAddr % BLOCK_SIZE || DestAddr >= 0xF000 || NumBytes % BLOCK_SIZE || NumBytes > 0xF000 || (DestAddr + NumBytes) > 0xF000) {
		return 1;
	}

	while (NumBytes >= BLOCK_SIZE) {
		FlashHalfSector(DestAddr + 0x0000, pBuffer + 0x0000, BLOCK_SIZE / 4);
		FlashHalfSector(DestAddr + 0x0100, pBuffer + 0x0100, BLOCK_SIZE / 4);
		NumBytes -= BLOCK_SIZE;
		pBuffer += BLOCK_SIZE;
	}

	FlashStop();
	FlashLock();

	return 0;
}
