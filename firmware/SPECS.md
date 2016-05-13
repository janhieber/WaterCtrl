# SPI message format
- First byte is a identifier/command
- other data depends on identifier, they may be parameters

Protocol:
- 0x01 - Message
  - 2. byte: error/message code
- 0x10 - Motor control command
  - 2. byte: motor number
  - 3. byte: time to run in sec
  - 4. byte: max speed: 0-100% (of PWM)
- 0x11 - Motor control response
  - 2. byte: motor number
  - 3. byte: 0xab = done/finish | 0xac = error
- 0x12 - Moisture measure request
  - 2. byte: sensor number
- 0x13 - Moisture measure value
  - 2. byte: sensor number
  - 3. byte: moisture value scaled to 8bit unsigned in kHz (0-255kHz)


