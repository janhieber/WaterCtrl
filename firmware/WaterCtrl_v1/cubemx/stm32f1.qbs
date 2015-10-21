import qbs
import qbs.FileInfo

import Stm32Application

Stm32Application {

    name: "WaterCtrl_v1"

    type: "hex"

    //Depends { name: "STM32_F1xx_HAL_Lib" }
    Group {
        name: "sources"
        prefix: "Src/**/"
        files: [
            "*.c",
            "*.h",
            "*.s",
        ]

        cpp.cxxFlags: [
            "-std=gnu++11",
        ]
        cpp.cFlags: [
            "-std=gnu11",
        ]
    }

    Group {
        name: "sources2"
        files: [
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi_ex.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c",
            "Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c",

        ]

        cpp.cxxFlags: [
            "-std=gnu++11",
        ]
        cpp.cFlags: [
            "-std=gnu11",
        ]
    }

    cpp.includePaths: [
        "Inc",
        "Drivers/STM32F1xx_HAL_Driver/Inc",
        "Drivers/CMSIS/Include/",
        "Drivers/CMSIS/Device/ST/STM32F1xx/Include/",
    ]
    cpp.defines: [
        "STM32F10X_MD",
        "STM32F103xB",
        "STM32F103C8T6",
        "HSE_VALUE=8000000",
        "USE_HAL_DRIVER"
    ]

    Rule {
        id: hex
        inputs: ["application"]

        Artifact {
            fileTags: ["hex"]
            filePath: ".obj/" + product.name + "/" + input.baseDir + "/" + input.fileName + ".bin"
        }

        prepare: {
//          var compilerPath = ModUtils.moduleProperty(product, "compilerPath");

            var objCopyPath = "arm-none-eabi-objcopy";
            var args = ["-O", "binary", input.filePath, output.filePath];
            var cmd = new Command(objCopyPath, args);

            cmd.description = "converting to hex: " + FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;
        }
    }
}
