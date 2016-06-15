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
- 0x12 - Sensor measure request
  - 2. byte: sensor number
- 0x13 - Sensor measure value
  - 2. byte: sensor number
  - 3. byte: measure value1 MSB
  - 4. byte: measure value1 LSB
  - 5. byte: measure value2 MSB
  - 6. byte: measure value2 LSB
- 0x14 - Sensor type SELECT
  - 2. byte: sensor number
  - 3. byte: type
- 0x15 - Sensor type Select response
  - 2. byte: sensor number
  - 3. byte: error

Example Sensor response in python:
 ['', '\x13\03\x03\x06']

Sensor type dependent response:
Type 1 (Capacitive Moisture)
 - value 1 frequency scaled to 16bit unsigned in kHz (0-800kHz)
Type 2 (DHT22)
 - value 1 temperature scaled to 10 times signed 16bit (-300 - 900)
 - value 1 humidity scaled to 10 times unsigned 16bit (0 - 1000)
