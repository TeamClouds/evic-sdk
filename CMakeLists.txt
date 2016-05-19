project(sdk C ASM)
cmake_minimum_required(VERSION 3.5)

set(ARMGCC "/usr/local/Cellar/gcc-arm-none-eabi/20151219")
set(CMAKE_ASM_COMPILER "${ARMGCC}/bin/arm-none-eabi-as")
set(CMAKE_C_COMPILER "${ARMGCC}/bin/arm-none-eabi-gcc")
set(CMAKE_AR "${ARMGCC}/bin/arm-none-eabi-ar")
set(CMAKE_OBJCOPY "${ARMGCC}/bin/arm-none-eabi-objcopy")
set(CMAKE_LINKER "${ARMGCC}/bin/arm-none-eabi-ld")

set(NUVOSDK nuvoton-sdk/Library)

set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_MINSIZEREL "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_ASM_FLAGS_DEBUG "")
set(CMAKE_ASM_FLAGS_MINSIZEREL "")
set(CMAKE_ASM_FLAGS_RELEASE "")

set(CMAKE_ASM_FLAGS "-mcpu=cortex-m4")
set(CMAKE_C_FLAGS "-Wall -mcpu=cortex-m4 -mthumb -Os -fdata-sections -ffunction-sections")

# don't need to ranlib
set(CMAKE_C_ARCHIVE_FINISH "")

# create our libevicsdk_crt0.o properly
set(CMAKE_ASM_CREATE_STATIC_LIBRARY "${CMAKE_LINKER} -r <OBJECTS> -o <TARGET>")

# create libevicsdk.a properly
set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> -rv <TARGET> <OBJECTS>")

set(SDK_ASM_FILES
        src/startup/startup.s
        src/startup/evicsdk_tag.s)

add_library(evicsdk_crt0 STATIC ${SDK_ASM_FILES})

# create lib dir
add_custom_command(
        TARGET evicsdk_crt0 PRE_LINK
        COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/lib
        VERBATIM
)

# save static lib to /lib
set_target_properties(evicsdk_crt0 PROPERTIES SUFFIX ".o")
set_target_properties(
        evicsdk_crt0
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

# sdk
set(SOURCE_FILES
        ${NUVOSDK}/Device/Nuvoton/M451Series/Source/system_M451Series.c
        ${NUVOSDK}/StdDriver/src/clk.c
        ${NUVOSDK}/StdDriver/src/fmc.c
        ${NUVOSDK}/StdDriver/src/gpio.c
        ${NUVOSDK}/StdDriver/src/spi.c
        ${NUVOSDK}/StdDriver/src/sys.c
        ${NUVOSDK}/StdDriver/src/timer.c
        ${NUVOSDK}/StdDriver/src/rtc.c
        ${NUVOSDK}/StdDriver/src/usbd.c
        ${NUVOSDK}/StdDriver/src/eadc.c
        ${NUVOSDK}/StdDriver/src/pwm.c
        src/startup/initfini.c
        src/startup/sbrk.c
        src/startup/init.c
        src/startup/sleep.c
        src/sysinfo/SysInfo.c
        src/dataflash/Dataflash.c
        src/display/Display_SSD.c
        src/display/Display_SSD1306.c
        src/display/Display_SSD1327.c
        src/display/Display.c
        src/font/Font_DejaVuSansMono_8pt.c
        src/timer/TimerUtils.c
        src/rtc/RTCUtils.c
        src/button/Button.c
        src/usb/USB_VirtualCOM.c
        src/adc/ADC.c
        src/battery/Battery.c
        src/atomizer/Atomizer.c
        )

set(SDK_INCLUDE_DIRS
        ${NUVOSDK}/CMSIS/Include
        ${NUVOSDK}/Device/Nuvoton/M451Series/Include
        ${NUVOSDK}/StdDriver/inc
        $ENV{EVICSDK}/include
        )
include_directories(${SDK_INCLUDE_DIRS})

add_library(evicsdk STATIC ${SOURCE_FILES})
add_dependencies(evicsdk evicsdk_crt0)

# save to /lib
set_target_properties(
        evicsdk
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

# create output lib dir
add_custom_command(
        TARGET evicsdk PRE_LINK
        COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/lib
        VERBATIM
)