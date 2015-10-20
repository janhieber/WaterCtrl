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
            "*.cpp",
            "*.h",
            "*.s",

        ]
        cpp.cxxFlags: [
            "-std=c++11",
        ]
        cpp.cFlags: [
            "-std=gnu99",
        ]
    }


    cpp.includePaths: [
        "Inc",
        "Drivers/STM32F1xx_HAL_Driver/Inc"
    ]
    cpp.defines: [
        "STM32F10X_MD",
        "STR_MAX_SIZE=127",
        "HSE_VALUE=8000000",
        "USE_HAL_DRIVER",
        "STM32F103xB",
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
