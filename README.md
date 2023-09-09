# Flash Loader for Segger JLink software to support the DP32G030

This flash loader supports flashing to the DP32G030 as used in the [UV-K5](https://github.com/DualTachyon/uv-k5-firmware) class of radio transceivers.

# Build

```
make
```

# Usage on Windows

Create a folder:

```
md "%USERPROFILE%\AppData\Roaming\SEGGER\JLinkDevices"
md "%USERPROFILE%\AppData\Roaming\SEGGER\JLinkDevices\DualTachyon"
md "%USERPROFILE%\AppData\Roaming\SEGGER\JLinkDevices\DualTachyon\DP32G030"
copy Devices.xml "%USERPROFILE%\AppData\Roaming\SEGGER\JLinkDevices\DualTachyon\DP32G030\"
copy dp32-fl.elf "%USERPROFILE%\AppData\Roaming\SEGGER\JLinkDevices\DualTachyon\DP32G030\"
```

For Linux, you'll have to figure out the paths, but should be a similar approach as Windows.

# License

Copyright 2023 Dual Tachyon
https://github.com/DualTachyon

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

