import qbs
import Stm32Library

Stm32Library {

    Group {
        name: "sources"
        prefix: "**/"
        files: [
            "*.c",
            "*.cpp",
            "*.h",
            "../../CMSIS/Device/ST/STM32F1xx/Source/Templates/*.c",
        ]
    }

    cpp.defines: [
        "STM32F103C8T6",
        "HSE_VALUE=8000000",
        "USE_HAL_DRIVER",
        "STM32F103xB",
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
            "STM32F103C8T6",
            "HSE_VALUE=8000000",
            "USE_HAL_DRIVER",
            "STM32F103xB",
        ]
        cpp.includePaths: [
            "Inc",
            path + "/../../Inc",
            path + "/../CMSIS/Device/ST/STM32F1xx/Include/",
            path + "/../CMSIS/Include/",
        ]
    }
}
