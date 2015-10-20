import qbs
import Stm32Library

Stm32Library {

    Group {
        name: "sources"
        files: [
            "Src/stm32f1xx_hal_rcc_ex.c",
            "Src/stm32f1xx_hal_pwr.c",
            "Src/stm32f1xx_hal.c",
            "Src/stm32f1xx_hal_gpio_ex.c",
            "Src/stm32f1xx_hal_cortex.c",
            "Src/stm32f1xx_hal_rcc.c",
            "Src/stm32f1xx_hal_uart.c",
            "Src/stm32f1xx_hal_usart.c",
            "Src/stm32f1xx_hal_spi_ex.c",
            "Src/stm32f1xx_hal_flash.c",
            "Src/stm32f1xx_hal_gpio.c",
            "Src/stm32f1xx_hal_dma.c",
            "Src/stm32f1xx_hal_spi.c",
            "Src/stm32f1xx_hal_tim.c",
            "Src/stm32f1xx_hal_flash_ex.c",
            "Src/stm32f1xx_hal_tim_ex.c",
        ]
    }

    cpp.defines: [
        "STM32F103xB",
        "STM32F103C8T6",
        "HSE_VALUE=8000000",
        "USE_HAL_DRIVER"
    ]
    cpp.includePaths: [
        "Inc",
        path + "/../../Inc",
        path + "/../CMSIS/Device/ST/STM32F1xx/Include/",
        path + "/../CMSIS/Include/",
    ]
    cpp.commonCompilerFlags: [
        "-mcpu=cortex-m3",
        "-mthumb",
        "-O1",
        "-Wall",
        "-g2",
        "-std=gnu11",
    ]
    cpp.linkerFlags: [
        "-mcpu=cortex-m3",
        "-mthumb",
        "-nostartfiles",
        "-O1",
        "-g2",
        "-Wl,-Map,"+ name + ".map",
        "--gc-sections",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.defines: [
            "STM32F103xB",
            "STM32F103C8T6",
            "HSE_VALUE=8000000",
            "USE_HAL_DRIVER"
        ]
        cpp.includePaths: [
            "Inc",
            path + "/../../Inc",
            path + "/../CMSIS/Device/ST/STM32F1xx/Include/",
            path + "/../CMSIS/Include/",
        ]
    }
}
