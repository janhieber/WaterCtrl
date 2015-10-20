import qbs
import qbs.FileInfo

import Stm32Application

Stm32Application {

    name: "WaterCtrl_v1"

    type: "hex"

    Depends { name: "STM32_F1xx_HAL_Lib" }
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


    cpp.includePaths: [
        "Inc",
        "Drivers/STM32F1xx_HAL_Driver/Inc"
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
