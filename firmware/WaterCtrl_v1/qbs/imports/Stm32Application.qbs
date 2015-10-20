import qbs

Stm32Product {
    type: [
        "application",
        "combHex",
    ]
    consoleApplication: false
    
    cpp.positionIndependentCode: false
    cpp.linkerFlags: {
        base.push("-Xlinker");
        base.push("--gc-sections");
        return base;
    }
}
