cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

project(HELIALights VERSION 1.0.0 LANGUAGES C ASM)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src
)

add_link_options(-Wl,--print-memory-usage)

set(HAL_COMP_LIST RCC GPIO CORTEX CAN UART USART)
set(CMSIS_COMP_LIST "")

list(APPEND HAL_COMP_LIST STM32F3)
list(APPEND CMSIS_COMP_LIST STM32F3)

find_package(HAL COMPONENTS "${HAL_COMP_LIST}" REQUIRED)
find_package(CMSIS COMPONENTS "${CMSIS_COMP_LIST}" REQUIRED)

set(MAIN_SOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/main.c)


add_executable(HELIALights
    ${MAIN_SOURCE_FILE} 
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/stm32f3xx_hal_conf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f3xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f3xx_it.c
    )

target_include_directories(HELIALights PRIVATE ${INCLUDE_DIRS})

target_link_libraries(HELIALights
        HAL::STM32::F3::RCC
        HAL::STM32::F3::GPIO
        HAL::STM32::F3::CORTEX
        HAL::STM32::F3::UART
        HAL::STM32::F3::CAN
        CMSIS::STM32::F303K8
        STM32::NoSys
    )

stm32_print_size_of_target(HELIALights)