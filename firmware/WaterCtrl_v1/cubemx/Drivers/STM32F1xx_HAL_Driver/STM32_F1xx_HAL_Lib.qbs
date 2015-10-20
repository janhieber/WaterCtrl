import qbs
import Stm32Library

Stm32Library {

    Group {
        name: "sources"
        prefix: "**/"
        files: [
            "*.c",
            "*.h",
            "../../CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c",
        ]
    }

    cpp.defines: [
        "STM32F10X_MD",
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

    Export {
        Depends { name: "cpp" }
        cpp.defines: [
            "STM32F10X_MD",
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
