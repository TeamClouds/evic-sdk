#set(CMAKE_C_COMPILER "${ARMGCC}/bin/arm-none-eabi-gcc")

project(sdk C ASM)
cmake_minimum_required(VERSION 3.5)

set(ARMGCC "/usr/local/Cellar/gcc-arm-none-eabi/20151219")
set(CMAKE_ASM_COMPILER "${ARMGCC}/bin/arm-none-eabi-as")
set(CMAKE_C_COMPILER "${ARMGCC}/bin/arm-none-eabi-gcc")
set(CMAKE_AR "${ARMGCC}/bin/arm-none-eabi-ar")
set(CMAKE_OBJCOPY "${ARMGCC}/bin/arm-none-eabi-objcopy")

set(INCDIRS "-I$ENV{EVICSDK}/include/ \
    -I$ENV{EVICSDK}/nuvoton-sdk/Library/CMSIS/Include/ \
    -I$ENV{EVICSDK}/nuvoton-sdk/Library/Device/Nuvoton/M451Series/Include/ \
    -I$ENV{EVICSDK}/nuvoton-sdk/Library/StdDriver/inc/")

set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_MINSIZEREL  "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_ASM_FLAGS_DEBUG "")
set(CMAKE_ASM_FLAGS_MINSIZEREL  "")
set(CMAKE_ASM_FLAGS_RELEASE  "")

set(CMAKE_C_FLAGS "${INCDIRS} -c -fshort-enums -Wall -mcpu=cortex-m4 -mthumb -Os -fdata-sections -ffunction-sections")

SET(BUILD_SHARED_LIBS ON)

# linker
set (TAGNAME $ENV{EVICSDK}/src/startup/evicsdk_tag.s)
set (STARTUP $ENV{EVICSDK}/src/startup/startup.s)
add_library(libevicsdk_crt0 OBJECT ${TAGNAME} ${STARTUP})

# sdk
set(SOURCE_FILES
    include/ADC.h
    include/Atomizer.h
    include/Battery.h
    include/Button.h
    include/Dataflash.h
    include/Display.h
    include/Display_SSD.h
    include/Display_SSD1306.h
    include/Display_SSD1327.h
    include/Font.h
    include/Font_Data.h
    include/Font_DejaVuSansMono_8pt.h
    include/RTCUtils.h
    include/SysInfo.h
    include/System.h
    include/TimerUtils.h
    include/USB.h
    include/USB_VirtualCOM.h
    src/adc/ADC.c
    src/aeabi/aeabi_memclr.c
    src/atomizer/Atomizer.c
    src/battery/Battery.c
    src/button/Button.c
    src/dataflash/Dataflash.c
    src/display/Display.c
    src/display/Display_SSD.c
    src/display/Display_SSD1306.c
    src/display/Display_SSD1327.c
    src/font/Font_DejaVuSansMono_8pt.c
    src/rtc/RTCUtils.c
    src/startup/init.c
    src/startup/initfini.c
    src/startup/sbrk.c
    src/startup/sleep.c
    src/sysinfo/SysInfo.c
    src/timer/TimerUtils.c
    src/usb/USB_VirtualCOM.c)

include_directories(
        "$ENV{EVICSDK}/include/ \
        $ENV{EVICSDK}/nuvoton-sdk/Library/CMSIS/Include/ \
        $ENV{EVICSDK}/nuvoton-sdk/Library/Device/Nuvoton/M451Series/Include/ \
        $ENV{EVICSDK}/nuvoton-sdk/Library/StdDriver/inc/")

#$<TARGET_OBJECTS:libevicsdk_crt0>
add_library(libevicsdk STATIC $<TARGET_OBJECTS:libevicsdk_crt0> ${SOURCE_FILES} )