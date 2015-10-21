import qbs

Stm32Product {
    type: [
        "application",
        "combHex",
    ]

    consoleApplication: false
    
    cpp.positionIndependentCode: false
    cpp.treatWarningsAsErrors: false
    cpp.warningLevel: "default" // "all" or "none" or "default"

//    cpp.linkerFlags: {
//        base.push("-Wl,--gc-sections");
//        return base;
//    }
}
