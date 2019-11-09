SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\ProgramData\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\MATLAB\R2017a\runtime\win64;C:\Program Files\MATLAB\R2017a\bin;C:\Program Files\Java\jdk-9.0.4\bin;C:\Program Files\Java\jdk-9.0.4\jre\bin;c:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;c:\Program Files\Microsoft SQL Server\100\Tools\Binn\;c:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin;C:\Program Files (x86)\Microchip\MPLAB C30\bin;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SDKs\TypeScript\1.0\;C:\Program Files (x86)\Flash Magic;C:\TDM-GCC-32\bin;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Users\Administrator\AppData\Local\Programs\Python\Python36;C:\ProgramData\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\MATLAB\R2017a\runtime\win64;C:\Program Files\MATLAB\R2017a\bin;C:\Program Files\Java\jdk-9.0.4\bin;C:\Program Files\Java\jdk-9.0.4\jre\bin;c:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;c:\Program Files\Microsoft SQL Server\100\Tools\Binn\;c:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files (x86)\Microchip\MPLAB C32 Suite\bin;C:\Program Files (x86)\Microchip\MPLAB C30\bin;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SDKs\TypeScript\1.0\;C:\Program Files (x86)\Flash Magic;C:\TDM-GCC-32\bin;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Users\Administrator\AppData\Local\Programs\Python\Python36;;C:\PROGRA~2\FLASHM~1
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=song370
SET CPU_CLOCK=0x044AA200
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\project\startup_stm32f10x_hd._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\croutine.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\list.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\queue.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\tasks.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\timers.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\heap_2.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\port.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\misc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\stm32f10x_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\stm32f10x_flash.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\stm32f10x_adc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\stm32f10x_dma.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\stm32f10x_tim.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\stm32f10x_rtc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\my_inittask.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\project\vios.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Project\BrakeDetection.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\Project\BrakeDetection.axf" --i32combined --output ".\Project\BrakeDetection.hex"
