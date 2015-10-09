# SPI message format
- First byte is a identifier
- other data depends on identifier

Identifiers (1st byte):
- 0x01 - Error message as ASCII string
- 0x02 - Info message as ASCII string
- 0x03 - Debug message as ASCII string
- 0x10 - Motor control command
  - 2. byte: motor number
  - 3. byte: motor speed
  - 4. byte: time to run
- 0x11 - Motor control response
  - 2. byte: 0x00 = done
- 0x12 - Moisture measure request
  - 2. byte: sensor number
- 0x13 - Moisture measure value
  - 2. byte: moisture value as 8bit unsigned



# UART message format
- First two digits is a global identifier
- Then one whitespace character
- then the message as ASCII string, zero terminated

Identifiers:  
- EE - Error message as ASCII string  
- II - Info message as ASCII string  
- DD - Debug message as ASCII string  
