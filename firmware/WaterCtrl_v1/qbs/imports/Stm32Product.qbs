import qbs

Product {
    Depends { name: "cpp" }
    
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
        "-g2",
        "-O1",
        "-Wl,-Map,/tmp/"+ name + ".map",
        "-g",
    ]
    cpp.linkerScripts: [
        "../arm-gcc-link.ld"
    ]
    cpp.architecture: "arm"

}
