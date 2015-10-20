import qbs

Stm32Product {
    type: [
        "application",
        "combHex",
    ]
    consoleApplication: false
    
    cpp.positionIndependentCode: false
    cpp.linkerFlags: {
        base.push("-Wl,--gc-sections");
        return base;
    }
}
