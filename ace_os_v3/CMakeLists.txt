cmake_minimum_required(VERSION 3.20)

project(ace_os_v3 C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(MCU_FLAGS
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
)

set(COMMON_COMPILE_OPTIONS
    ${MCU_FLAGS}
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
    -Werror
    -Og
    -g3
)

function(add_ace_lab LAB_NAME)
    file(GLOB_RECURSE APP_C_SRC      CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${LAB_NAME}/*.c")
    file(GLOB_RECURSE APP_ASM_S_SRC  CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${LAB_NAME}/*.S" "${CMAKE_CURRENT_SOURCE_DIR}/${LAB_NAME}/*.s")

    add_executable(${LAB_NAME}.elf
        common/startup_stm32f746.c
        common/system_stm32f746.c
        common/itm.c
        common/syscalls.c
        ${APP_C_SRC}
        ${APP_ASM_S_SRC}
    )

    target_include_directories(${LAB_NAME}.elf PRIVATE 
        common
        "${CMAKE_CURRENT_SOURCE_DIR}/${LAB_NAME}"
        "${CMAKE_CURRENT_SOURCE_DIR}/${LAB_NAME}/cpu"
        "${CMAKE_CURRENT_SOURCE_DIR}/${LAB_NAME}/cpu/arm/gnu"
        "${CMAKE_CURRENT_SOURCE_DIR}/${LAB_NAME}/osKernel/config"
        "${CMAKE_CURRENT_SOURCE_DIR}/${LAB_NAME}/osKernel/impl"
        "${CMAKE_CURRENT_SOURCE_DIR}/${LAB_NAME}/osKernel/port/arm-cortex-m/armv7-m/gnu"
    )
    target_compile_options(${LAB_NAME}.elf PRIVATE ${COMMON_COMPILE_OPTIONS})
    target_link_options(${LAB_NAME}.elf PRIVATE
        ${MCU_FLAGS}
        -T${CMAKE_SOURCE_DIR}/common/stm32f746.ld
        -Wl,--gc-sections
        -Wl,-Map=${CMAKE_BINARY_DIR}/${LAB_NAME}.map
        --specs=nano.specs
        --specs=nosys.specs
    )

    add_custom_command(TARGET ${LAB_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${LAB_NAME}.elf> ${LAB_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${LAB_NAME}.elf> ${LAB_NAME}.bin
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${LAB_NAME}.elf>
        COMMENT "Generating ${LAB_NAME}.hex and ${LAB_NAME}.bin"
    )
endfunction()

add_ace_lab(lab0)

