<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>log.h</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Inc/</path>
    <filename>log_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>Log</name>
      <anchorfile>group__Logging.html</anchorfile>
      <anchor>gaa5f1e819907ea056f541d68e85272cd2</anchor>
      <arglist>(LogLevel loglevel, char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSetDestination</name>
      <anchorfile>group__Logging.html</anchorfile>
      <anchor>gaa0b7f0e3704065f96cacc925211a5af3</anchor>
      <arglist>(LogDestination destination)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>motors.h</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Inc/</path>
    <filename>motors_8h</filename>
  </compound>
  <compound kind="file">
    <name>spicomm.h</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Inc/</path>
    <filename>spicomm_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>SPI_SENDQUEUE_SIZE</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>ga046708bf8b10b373fc94bf9b92b45649</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SPI_RECVQUEUE_SIZE</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>ga6177ab4ab1e1790508797c4987003b3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spiQueueInit</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>ga9efb05b0b48169d9ad99cead17797f1b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spiSend</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>gae7a5ae0fcd4d456fa8341756fd3f5c37</anchor>
      <arglist>(uint8_t id, char *msg)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>stm32f1xx_hal_conf.h</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Inc/</path>
    <filename>stm32f1xx__hal__conf_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>HAL_MODULE_ENABLED</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a877ae99e8c47a609ea97c888912bf75f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HSE_VALUE</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>aeafcff4f57440c60e64812dddd13e7cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HSE_STARTUP_TIMEOUT</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a68ecbc9b0a1a40a1ec9d18d5e9747c4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HSI_VALUE</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>aaa8c76e274d0f6dd2cefb5d0b17fbc37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LSE_VALUE</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a7bbb9d19e5189a6ccd0fb6fa6177d20d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LSE_STARTUP_TIMEOUT</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a85e6fc812dc26f7161a04be2568a5462</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VDD_VALUE</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>aae550dad9f96d52cfce5e539adadbbb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TICK_INT_PRIORITY</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>ae27809d4959b9fd5b5d974e3e1c77d2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAC_ADDR0</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>ab84a2e15d360e2644ada09641513a941</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_BCR</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a8abe1a40c71e68881ec669d59f513fdb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_BSR</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a4b8f2c29a9e74412395e1b1809666838</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_RESET</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a6f5048620b3dde8583f7f1118e9de187</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_LOOPBACK</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a7833d885caa7e29abbebfb90a4b96f86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_FULLDUPLEX_100M</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a5729771244f68779fc694ba819cd60a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_HALFDUPLEX_100M</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a1ac901a4ad405241d90a5c10104b8986</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_FULLDUPLEX_10M</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a6b6254fd3dacbf1578a9d8058cd86373</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_HALFDUPLEX_10M</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a4fa7ca6faf60ee074576ebb6103f8dd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_AUTONEGOTIATION</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a9b7f5c8f71047ee449f21562d26b1b43</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_RESTART_AUTONEGOTIATION</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a66c4b69bd08dc25b6730365d3ff740c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_POWERDOWN</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>aa0b1e6d4a23470fc1ac4f9222b51f8a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_ISOLATE</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a7d5233295134a385866eb5bdafe2162b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_AUTONEGO_COMPLETE</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a36c4dbd5f6df1f5eaefa010929ef9773</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_LINKED_STATUS</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>ace209074499dbef0b97300da5bd7c707</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_JABBER_DETECTION</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a057b4d3fb66548d65c291a5b41611be2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_SR</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a32b55e84d27cf298a77f54b133cd1acc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_MICR</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>ac4d8c2e6c2509a9bdaf214b24deafea7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_MISR</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a81d36e97e4a9da33f2a7e142b01964f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_LINK_STATUS</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a4a6cbf61f5e1a134d8983ef29fd2d386</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_SPEED_STATUS</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a74c081bc55e9ff96bf229f44e96c6155</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_DUPLEX_STATUS</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>ab928f45585242fde1a8d81a2d9ed22d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_MICR_INT_EN</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>ab0314b8559d5895194b435ae93aee9c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_MICR_INT_OE</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>aca1bf9e00caba70caa1f1a9e56cbee5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_MISR_LINK_INT_EN</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>aaf06885683edcd946ad960f59e8a6f9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PHY_LINK_INTERRUPT</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a7c378eb26673981df0834658f4fec4c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>assert_param</name>
      <anchorfile>stm32f1xx__hal__conf_8h.html</anchorfile>
      <anchor>a631dea7b230e600555f979c62af1de21</anchor>
      <arglist>(expr)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>stm32f1xx_it.h</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Inc/</path>
    <filename>stm32f1xx__it_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>SysTick_Handler</name>
      <anchorfile>stm32f1xx__it_8h.html</anchorfile>
      <anchor>ab5e09814056d617c521549e542639b7e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>TIM2_IRQHandler</name>
      <anchorfile>stm32f1xx__it_8h.html</anchorfile>
      <anchor>a38ad4725462bdc5e86c4ead4f04b9fc2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>TIM3_IRQHandler</name>
      <anchorfile>stm32f1xx__it_8h.html</anchorfile>
      <anchor>ac8e51d2183b5230cbd5481f8867adce9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SPI1_IRQHandler</name>
      <anchorfile>stm32f1xx__it_8h.html</anchorfile>
      <anchor>a9bbd8c17ce4f49adcca47d11f482aab6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>USART1_IRQHandler</name>
      <anchorfile>stm32f1xx__it_8h.html</anchorfile>
      <anchor>a7139cd4baabbbcbab0c1fe6d7d4ae1cc</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>isr_moisture.c</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Src/</path>
    <filename>isr__moisture_8c</filename>
    <member kind="function">
      <type>void</type>
      <name>HAL_TIM_IC_CaptureCallback</name>
      <anchorfile>group__MoistureMeasure.html</anchorfile>
      <anchor>ga77a2401a35ddd9bd0b8fc28331b81381</anchor>
      <arglist>(TIM_HandleTypeDef *htim)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>log.c</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Src/</path>
    <filename>log_8c</filename>
    <includes id="log_8h" name="log.h" local="no" imported="no">log.h</includes>
    <member kind="function">
      <type>void</type>
      <name>Log</name>
      <anchorfile>group__Logging.html</anchorfile>
      <anchor>gaa5f1e819907ea056f541d68e85272cd2</anchor>
      <arglist>(LogLevel loglevel, char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSetDestination</name>
      <anchorfile>group__Logging.html</anchorfile>
      <anchor>gaa0b7f0e3704065f96cacc925211a5af3</anchor>
      <arglist>(LogDestination destination)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>moistureMeasure.c</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Src/</path>
    <filename>moistureMeasure_8c</filename>
    <member kind="function">
      <type>int</type>
      <name>MeasureInit</name>
      <anchorfile>group__MoistureMeasure.html</anchorfile>
      <anchor>gac306477cfeeb89d9fb7c5aaab4d7d2fa</anchor>
      <arglist>(TIM_HandleTypeDef *ptrTimerRef, uint32_t channel)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EnableMeasureInterrupt</name>
      <anchorfile>group__MoistureMeasure.html</anchorfile>
      <anchor>ga88dd3486884d225ff651e28d30877201</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DisableMeasureInterrupt</name>
      <anchorfile>group__MoistureMeasure.html</anchorfile>
      <anchor>ga5f3f4f49c56ba5d2b1843d59ab583239</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>motors.c</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Src/</path>
    <filename>motors_8c</filename>
    <includes id="log_8h" name="log.h" local="no" imported="no">log.h</includes>
    <includes id="motors_8h" name="motors.h" local="no" imported="no">motors.h</includes>
  </compound>
  <compound kind="file">
    <name>printf.c</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Src/</path>
    <filename>printf_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>MAX_STRING_SIZE</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga220f38b26fa99d4d91b574f42d991516</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>__attribute__</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gaf9aace1b44b73111e15aa39f06f43456</anchor>
      <arglist>((weak))</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutChar</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga9d6957a52d69935cd92755844e9252be</anchor>
      <arglist>(char *pStr, char c)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutString</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga9692c423423d4787d69f090a359c5644</anchor>
      <arglist>(char *pStr, const char *pSource)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutUnsignedInt</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gafeda5b1cd1c8e201b2fc997b35d03281</anchor>
      <arglist>(char *pStr, char fill, signed int width, unsigned int value)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutSignedInt</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gae7337166e7d8bcc7d589e63777ae2541</anchor>
      <arglist>(char *pStr, char fill, signed int width, signed int value)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutHexa</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga3b3bdb18a334228eb2b08b1a70c1a62a</anchor>
      <arglist>(char *pStr, char fill, signed int width, unsigned char maj, unsigned int value)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>vsnprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga8f2d28f08e5d66db9a14ed70cb1f4220</anchor>
      <arglist>(char *pStr, size_t length, const char *pFormat, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>snprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gae4d7a9a581baef0b2b2174383580ab15</anchor>
      <arglist>(char *pString, size_t length, const char *pFormat,...)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>vsprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gac2bd6c4c9ad1b563bdcd20f18a36811a</anchor>
      <arglist>(char *pString, const char *pFormat, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>vfprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga4c938d1cedba90cbc8107400ebea6100</anchor>
      <arglist>(FILE *pStream, const char *pFormat, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>vprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gadc27df6594a246219000f9d2a584a3be</anchor>
      <arglist>(const char *pFormat, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>fprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga714a7aeb0b480bc7cc85c1685964861f</anchor>
      <arglist>(FILE *pStream, const char *pFormat,...)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>printf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gad6c265f6d41266cbb62035915f9c0030</anchor>
      <arglist>(const char *pFormat,...)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>sprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga2ab9fb00bf66223b1a3f334376963100</anchor>
      <arglist>(char *pStr, const char *pFormat,...)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>puts</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga5a8c5f197e584138be351a4437f67689</anchor>
      <arglist>(const char *pStr)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>fputc</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga25c4b74226a8a3a0d6eef3a072be087b</anchor>
      <arglist>(signed int c, FILE *pStream)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>fputs</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gad870e26ae50d712bb504aa4b067802f4</anchor>
      <arglist>(const char *pStr, FILE *pStream)</arglist>
    </member>
    <member kind="variable">
      <type>struct _reent</type>
      <name>r</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga933442104e916763ad534cd32e42bbae</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>spicomm.c</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Src/</path>
    <filename>spicomm_8c</filename>
    <includes id="log_8h" name="log.h" local="no" imported="no">log.h</includes>
    <includes id="spicomm_8h" name="spicomm.h" local="no" imported="no">spicomm.h</includes>
    <member kind="function">
      <type>void</type>
      <name>spiQueueInit</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>ga9efb05b0b48169d9ad99cead17797f1b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spiSend</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>gae7a5ae0fcd4d456fa8341756fd3f5c37</anchor>
      <arglist>(uint8_t id, char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HAL_SPI_TxRxCpltCallback</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>ga04e63f382f172164c8e7cae4ff5d883c</anchor>
      <arglist>(SPI_HandleTypeDef *hspi)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>stm32f1xx_it.c</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Src/</path>
    <filename>stm32f1xx__it_8c</filename>
    <includes id="stm32f1xx__it_8h" name="stm32f1xx_it.h" local="yes" imported="no">stm32f1xx_it.h</includes>
    <member kind="function">
      <type>void</type>
      <name>SysTick_Handler</name>
      <anchorfile>stm32f1xx__it_8c.html</anchorfile>
      <anchor>ab5e09814056d617c521549e542639b7e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>TIM2_IRQHandler</name>
      <anchorfile>stm32f1xx__it_8c.html</anchorfile>
      <anchor>a38ad4725462bdc5e86c4ead4f04b9fc2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>TIM3_IRQHandler</name>
      <anchorfile>stm32f1xx__it_8c.html</anchorfile>
      <anchor>ac8e51d2183b5230cbd5481f8867adce9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SPI1_IRQHandler</name>
      <anchorfile>stm32f1xx__it_8c.html</anchorfile>
      <anchor>a9bbd8c17ce4f49adcca47d11f482aab6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>USART1_IRQHandler</name>
      <anchorfile>stm32f1xx__it_8c.html</anchorfile>
      <anchor>a7139cd4baabbbcbab0c1fe6d7d4ae1cc</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>system_stm32f1xx.c</name>
    <path>/home/jan/WaterCtrl/firmware/WaterCtrl_v1/cubemx/Src/</path>
    <filename>system__stm32f1xx_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>HSE_VALUE</name>
      <anchorfile>group__STM32F1xx__System__Private__Defines.html</anchorfile>
      <anchor>gaeafcff4f57440c60e64812dddd13e7cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HSI_VALUE</name>
      <anchorfile>group__STM32F1xx__System__Private__Defines.html</anchorfile>
      <anchor>gaaa8c76e274d0f6dd2cefb5d0b17fbc37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VECT_TAB_OFFSET</name>
      <anchorfile>group__STM32F1xx__System__Private__Defines.html</anchorfile>
      <anchor>ga40e1495541cbb4acbe3f1819bd87a9fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SystemInit</name>
      <anchorfile>group__STM32F1xx__System__Private__Functions.html</anchorfile>
      <anchor>ga93f514700ccf00d08dbdcff7f1224eb2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SystemCoreClockUpdate</name>
      <anchorfile>group__STM32F1xx__System__Private__Functions.html</anchorfile>
      <anchor>gae0c36a9591fe6e9c45ecb21a794f0f0f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>SystemCoreClock</name>
      <anchorfile>group__STM32F1xx__System__Private__Variables.html</anchorfile>
      <anchor>gaa3cd3e43291e81e795d642b79b6088e6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>CMSIS</name>
    <title>CMSIS</title>
    <filename>group__CMSIS.html</filename>
    <subgroup>Stm32f1xx_system</subgroup>
  </compound>
  <compound kind="group">
    <name>Communication</name>
    <title>Communication</title>
    <filename>group__Communication.html</filename>
    <subgroup>Logging</subgroup>
    <subgroup>printf</subgroup>
    <subgroup>SPI</subgroup>
  </compound>
  <compound kind="group">
    <name>Logging</name>
    <title>Logging</title>
    <filename>group__Logging.html</filename>
    <member kind="function">
      <type>void</type>
      <name>Log</name>
      <anchorfile>group__Logging.html</anchorfile>
      <anchor>gaa5f1e819907ea056f541d68e85272cd2</anchor>
      <arglist>(LogLevel loglevel, char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSetDestination</name>
      <anchorfile>group__Logging.html</anchorfile>
      <anchor>gaa0b7f0e3704065f96cacc925211a5af3</anchor>
      <arglist>(LogDestination destination)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>Main</name>
    <title>Main</title>
    <filename>group__Main.html</filename>
    <member kind="function">
      <type>void</type>
      <name>SystemClock_Config</name>
      <anchorfile>group__Main.html</anchorfile>
      <anchor>ga70af21c671abfcc773614a9a4f63d920</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>PrintChar</name>
      <anchorfile>group__Main.html</anchorfile>
      <anchor>gaac7233dce0ef357b1ea507667a7b6e07</anchor>
      <arglist>(char c)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>MoistureMeasure</name>
    <title>MoistureMeasure</title>
    <filename>group__MoistureMeasure.html</filename>
    <member kind="function">
      <type>void</type>
      <name>HAL_TIM_IC_CaptureCallback</name>
      <anchorfile>group__MoistureMeasure.html</anchorfile>
      <anchor>ga77a2401a35ddd9bd0b8fc28331b81381</anchor>
      <arglist>(TIM_HandleTypeDef *htim)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>MeasureInit</name>
      <anchorfile>group__MoistureMeasure.html</anchorfile>
      <anchor>gac306477cfeeb89d9fb7c5aaab4d7d2fa</anchor>
      <arglist>(TIM_HandleTypeDef *ptrTimerRef, uint32_t channel)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EnableMeasureInterrupt</name>
      <anchorfile>group__MoistureMeasure.html</anchorfile>
      <anchor>ga88dd3486884d225ff651e28d30877201</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DisableMeasureInterrupt</name>
      <anchorfile>group__MoistureMeasure.html</anchorfile>
      <anchor>ga5f3f4f49c56ba5d2b1843d59ab583239</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>MotorControl</name>
    <title>MotorControl</title>
    <filename>group__MotorControl.html</filename>
  </compound>
  <compound kind="group">
    <name>printf</name>
    <title>Printf</title>
    <filename>group__printf.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>MAX_STRING_SIZE</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga220f38b26fa99d4d91b574f42d991516</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>__attribute__</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gaf9aace1b44b73111e15aa39f06f43456</anchor>
      <arglist>((weak))</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutChar</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga9d6957a52d69935cd92755844e9252be</anchor>
      <arglist>(char *pStr, char c)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutString</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga9692c423423d4787d69f090a359c5644</anchor>
      <arglist>(char *pStr, const char *pSource)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutUnsignedInt</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gafeda5b1cd1c8e201b2fc997b35d03281</anchor>
      <arglist>(char *pStr, char fill, signed int width, unsigned int value)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutSignedInt</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gae7337166e7d8bcc7d589e63777ae2541</anchor>
      <arglist>(char *pStr, char fill, signed int width, signed int value)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>PutHexa</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga3b3bdb18a334228eb2b08b1a70c1a62a</anchor>
      <arglist>(char *pStr, char fill, signed int width, unsigned char maj, unsigned int value)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>vsnprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga8f2d28f08e5d66db9a14ed70cb1f4220</anchor>
      <arglist>(char *pStr, size_t length, const char *pFormat, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>snprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gae4d7a9a581baef0b2b2174383580ab15</anchor>
      <arglist>(char *pString, size_t length, const char *pFormat,...)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>vsprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gac2bd6c4c9ad1b563bdcd20f18a36811a</anchor>
      <arglist>(char *pString, const char *pFormat, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>vfprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga4c938d1cedba90cbc8107400ebea6100</anchor>
      <arglist>(FILE *pStream, const char *pFormat, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>vprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gadc27df6594a246219000f9d2a584a3be</anchor>
      <arglist>(const char *pFormat, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>fprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga714a7aeb0b480bc7cc85c1685964861f</anchor>
      <arglist>(FILE *pStream, const char *pFormat,...)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>printf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gad6c265f6d41266cbb62035915f9c0030</anchor>
      <arglist>(const char *pFormat,...)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>sprintf</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga2ab9fb00bf66223b1a3f334376963100</anchor>
      <arglist>(char *pStr, const char *pFormat,...)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>puts</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga5a8c5f197e584138be351a4437f67689</anchor>
      <arglist>(const char *pStr)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>fputc</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga25c4b74226a8a3a0d6eef3a072be087b</anchor>
      <arglist>(signed int c, FILE *pStream)</arglist>
    </member>
    <member kind="function">
      <type>signed int</type>
      <name>fputs</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>gad870e26ae50d712bb504aa4b067802f4</anchor>
      <arglist>(const char *pStr, FILE *pStream)</arglist>
    </member>
    <member kind="variable">
      <type>struct _reent</type>
      <name>r</name>
      <anchorfile>group__printf.html</anchorfile>
      <anchor>ga933442104e916763ad534cd32e42bbae</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>SPI</name>
    <title>SPI</title>
    <filename>group__SPI.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>SPI_SENDQUEUE_SIZE</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>ga046708bf8b10b373fc94bf9b92b45649</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SPI_RECVQUEUE_SIZE</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>ga6177ab4ab1e1790508797c4987003b3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spiQueueInit</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>ga9efb05b0b48169d9ad99cead17797f1b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>spiSend</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>gae7a5ae0fcd4d456fa8341756fd3f5c37</anchor>
      <arglist>(uint8_t id, char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HAL_SPI_TxRxCpltCallback</name>
      <anchorfile>group__SPI.html</anchorfile>
      <anchor>ga04e63f382f172164c8e7cae4ff5d883c</anchor>
      <arglist>(SPI_HandleTypeDef *hspi)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>STM32F1xx_System_Private_Defines</name>
    <title>STM32F1xx_System_Private_Defines</title>
    <filename>group__STM32F1xx__System__Private__Defines.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>HSE_VALUE</name>
      <anchorfile>group__STM32F1xx__System__Private__Defines.html</anchorfile>
      <anchor>gaeafcff4f57440c60e64812dddd13e7cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HSI_VALUE</name>
      <anchorfile>group__STM32F1xx__System__Private__Defines.html</anchorfile>
      <anchor>gaaa8c76e274d0f6dd2cefb5d0b17fbc37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>VECT_TAB_OFFSET</name>
      <anchorfile>group__STM32F1xx__System__Private__Defines.html</anchorfile>
      <anchor>ga40e1495541cbb4acbe3f1819bd87a9fe</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>STM32F1xx_System_Private_FunctionPrototypes</name>
    <title>STM32F1xx_System_Private_FunctionPrototypes</title>
    <filename>group__STM32F1xx__System__Private__FunctionPrototypes.html</filename>
  </compound>
  <compound kind="group">
    <name>STM32F1xx_System_Private_Functions</name>
    <title>STM32F1xx_System_Private_Functions</title>
    <filename>group__STM32F1xx__System__Private__Functions.html</filename>
    <member kind="function">
      <type>void</type>
      <name>SystemInit</name>
      <anchorfile>group__STM32F1xx__System__Private__Functions.html</anchorfile>
      <anchor>ga93f514700ccf00d08dbdcff7f1224eb2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SystemCoreClockUpdate</name>
      <anchorfile>group__STM32F1xx__System__Private__Functions.html</anchorfile>
      <anchor>gae0c36a9591fe6e9c45ecb21a794f0f0f</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>STM32F1xx_System_Private_Includes</name>
    <title>STM32F1xx_System_Private_Includes</title>
    <filename>group__STM32F1xx__System__Private__Includes.html</filename>
  </compound>
  <compound kind="group">
    <name>STM32F1xx_System_Private_Macros</name>
    <title>STM32F1xx_System_Private_Macros</title>
    <filename>group__STM32F1xx__System__Private__Macros.html</filename>
  </compound>
  <compound kind="group">
    <name>STM32F1xx_System_Private_TypesDefinitions</name>
    <title>STM32F1xx_System_Private_TypesDefinitions</title>
    <filename>group__STM32F1xx__System__Private__TypesDefinitions.html</filename>
  </compound>
  <compound kind="group">
    <name>STM32F1xx_System_Private_Variables</name>
    <title>STM32F1xx_System_Private_Variables</title>
    <filename>group__STM32F1xx__System__Private__Variables.html</filename>
    <member kind="variable">
      <type>uint32_t</type>
      <name>SystemCoreClock</name>
      <anchorfile>group__STM32F1xx__System__Private__Variables.html</anchorfile>
      <anchor>gaa3cd3e43291e81e795d642b79b6088e6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>Stm32f1xx_system</name>
    <title>Stm32f1xx_system</title>
    <filename>group__stm32f1xx__system.html</filename>
    <subgroup>STM32F1xx_System_Private_Defines</subgroup>
    <subgroup>STM32F1xx_System_Private_FunctionPrototypes</subgroup>
    <subgroup>STM32F1xx_System_Private_Functions</subgroup>
    <subgroup>STM32F1xx_System_Private_Includes</subgroup>
    <subgroup>STM32F1xx_System_Private_Macros</subgroup>
    <subgroup>STM32F1xx_System_Private_TypesDefinitions</subgroup>
    <subgroup>STM32F1xx_System_Private_Variables</subgroup>
  </compound>
  <compound kind="group">
    <name>Stuff</name>
    <title>Stuff</title>
    <filename>group__Stuff.html</filename>
    <subgroup>syscalls</subgroup>
  </compound>
  <compound kind="group">
    <name>syscalls</name>
    <title>Syscalls</title>
    <filename>group__syscalls.html</filename>
  </compound>
</tagfile>
