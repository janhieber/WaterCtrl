import qbs

Product {
    Depends { name: "cpp" }
    
    cpp.commonCompilerFlags: [
        "-mcpu=cortex-m3",
        "-mthumb",
        "-O0",
        "-fmessage-length=0",
        "-fsigned-char",
        "-ffunction-sections",
        "-fdata-sections",
        "-ffreestanding",
        "-Wall",
        "-Wextra",
        "-g",
        "-std=gnu11",
        "-Wno-unused",
    ]
    cpp.linkerFlags: [
        "-mcpu=cortex-m3",
        "-mthumb",
        "-nostartfiles",
        "-fmessage-length=0",
        "-fsigned-char",
        "-ffunction-sections",
        "-fdata-sections",
        "-ffreestanding",
        "-Wall",
        "-Wextra",
        "-g",
        "-Wl,-Map,"+ name + ".map",
        "-lg",
        "-lnosys",
    ]
    cpp.linkerScripts: [
        //"../arm-gcc-link.ld",
        "SW4STM32/cubemx Configuration/STM32F103C8Tx_FLASH.ld",
    ]
}
