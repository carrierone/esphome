# M5Stack 8Encoder Component

This component provides support for the [M5Stack 8Encoder Unit](https://docs.m5stack.com/en/unit/8encoder), which features 8 digital rotary encoders with RGB LEDs and push buttons connected via I2C.

## Purchase & Documentation Links

### Purchasing
- **🛒 M5Stack Official Store**: [8-Encoder Unit](https://shop.m5stack.com/products/8-encoder-unit)
- **🌍 M5Stack Global Store**: [8-Encoder Unit International](https://m5stack.com/products/8-encoder-unit)
- **🛒 Alternative Retailers**: Available on Amazon, AliExpress, and electronics distributors worldwide

### Official Resources
- **📖 Documentation**: [M5Stack 8Encoder Unit Docs](https://docs.m5stack.com/en/unit/8encoder)
- **📋 Datasheet**: [Technical Specifications & Schematic PDF](https://static-cdn.m5stack.com/resource/docs/datasheet/unit/8ENCODER/8ENCODER_datasheet_en.pdf)
- **💻 Arduino Library**: [M5Unit-8Encoder GitHub Repository](https://github.com/m5stack/M5Unit-8Encoder)
- **🎥 Tutorials**: [M5Stack YouTube Channel](https://www.youtube.com/c/M5Stack)

### Hardware Information
- **📱 Product Code**: U167
- **📏 Dimensions**: 48mm × 24mm × 12.8mm
- **⚡ Power**: 5V DC, ~200mA max (all LEDs on)
- **🔧 MCU**: [STM32F030 ARM Cortex-M0](https://www.st.com/en/microcontrollers-microprocessors/stm32f030.html)
- **📞 Support**: [M5Stack Official Support](https://m5stack.com/pages/contact-us)

## Hardware Features

- 8 digital rotary encoders (endless rotation)
- 8 RGB LEDs (one per encoder)
- 8 push buttons (one per encoder)
- 1 slide switch (device-wide toggle)
- I2C communication interface (address: 0x41)
- STM32F030 microcontroller

## Configuration

```yaml
# Example configuration for ESP32
i2c:
  sda: 21
  scl: 22
  scan: true

# For ESP32-S3, use different pins:
# i2c:
#   sda: 40
#   scl: 41
#   scan: true

m5stack_8encoder:
  - id: my_8encoder
    address: 0x41

# Encoder count sensors
sensor:
  - platform: m5stack_8encoder
    m5stack_8encoder_id: my_8encoder
    name: "Encoder 1 Count"
    channel: 1
    update_interval: 1s

  - platform: m5stack_8encoder
    m5stack_8encoder_id: my_8encoder
    name: "Encoder 2 Count"
    channel: 2
    update_interval: 1s

# Button states
binary_sensor:
  - platform: m5stack_8encoder
    m5stack_8encoder_id: my_8encoder
    name: "Encoder 1 Button"
    channel: 1
    type: button
    update_interval: 50ms

  - platform: m5stack_8encoder
    m5stack_8encoder_id: my_8encoder
    name: "Encoder 2 Button"
    channel: 2
    type: button
    update_interval: 50ms

  # Device-wide slide switch (no channel needed)
  - platform: m5stack_8encoder
    m5stack_8encoder_id: my_8encoder
    name: "Slide Switch"
    type: toggle
    update_interval: 50ms

# RGB LED control
light:
  - platform: m5stack_8encoder
    m5stack_8encoder_id: my_8encoder
    name: "Encoder 1 LED"
    channel: 1

  - platform: m5stack_8encoder
    m5stack_8encoder_id: my_8encoder
    name: "Encoder 2 LED"
    channel: 2
```

## Configuration Options

### Base Component

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `id` | ID | | ID for this component |
| `address` | int | `0x41` | I2C address of the device |

### Sensor (Encoder Counts)

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `m5stack_8encoder_id` | ID | | ID of the M5Stack 8Encoder component |
| `channel` | int | | Encoder channel (1-8) |
| `update_interval` | time | `1s` | How often to read the encoder count |

All standard [sensor](https://esphome.io/components/sensor/index.html) options are also available.

### Binary Sensor (Buttons)

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `m5stack_8encoder_id` | ID | | ID of the M5Stack 8Encoder component |
| `channel` | int | | Button channel (1-8). **Not used for `toggle` type** |
| `type` | enum | `button` | Type: `button` or `toggle` |
| `update_interval` | time | `50ms` | How often to read the button state |

All standard [binary sensor](https://esphome.io/components/binary_sensor/index.html) options are also available.

### Light (RGB LEDs)

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `m5stack_8encoder_id` | ID | | ID of the M5Stack 8Encoder component |
| `channel` | int | | LED channel (1-8) |

All standard [light](https://esphome.io/components/light/index.html) options are also available.

## Usage Notes

### Encoder Counts
- Encoder counts are signed 32-bit integers that increment/decrement with rotation
- Values persist across device resets
- Range: -2,147,483,648 to 2,147,483,647
- Failed reads return a special error value

### Button States  
- **`button` type**: Momentary press detection - ideal for triggering actions (requires `channel` 1-8)
- **`toggle` type**: Device-wide slide switch - physical slide switch on the unit (no `channel` needed)
- Fast update intervals (50ms) recommended for responsive detection

### Slide Switch
- **Hardware**: Physical slide switch on the M5Stack 8Encoder unit
- **Type**: Use `type: toggle` in binary sensor configuration  
- **States**: Returns `true` when switch is in position 1, `false` when in position 0
- **Global**: Affects the entire device, not tied to any specific encoder channel

### RGB LEDs
- Individual control per encoder channel
- Full RGB color support
- Standard ESPHome light effects available

### I2C Communication
- Default address: 0x41
- Requires I2C bus configuration
- Auto-detects communication failures
- Status warnings for I2C errors

## Hardware Connections

Connect the M5Stack 8Encoder to your ESP32/ESP8266 I2C bus:

| 8Encoder Pin | ESP32 Pin | ESP32-S3 Pin | ESP8266 Pin |
|--------------|-----------|--------------|-------------|
| SDA | GPIO 21 | GPIO 40 | GPIO 4 |
| SCL | GPIO 22 | GPIO 41 | GPIO 5 |
| VCC | 3.3V/5V | 3.3V/5V | 3.3V |
| GND | GND | GND | GND |

## Error Handling & Reliability

The component includes comprehensive error handling:

- **I2C Communication**: Automatic detection and recovery from I2C failures
- **Status Warnings**: Clear error reporting in Home Assistant interface
- **Bounds Checking**: Safe handling of out-of-range channel access
- **Memory Management**: Proper cleanup of dynamically allocated memory
- **Data Validation**: Encoder value validation with error indicators
- **Endianness Handling**: Correct little-endian data conversion from STM32F030

## Home Assistant Integration

**Automatic Discovery**: Home Assistant automatically discovers and creates entities for all configured M5Stack 8Encoder components via the ESPHome integration. No additional Home Assistant configuration required.

**Entity Types Created**:
- **Sensors**: `sensor.encoder_X_count` - Encoder count values with rotation icon
- **Binary Sensors**: `binary_sensor.encoder_X_button` - Button states with automatic button device class
- **Binary Sensors**: `binary_sensor.slide_switch` - Device-wide slide switch state
- **Lights**: `light.encoder_X_led` - Individual RGB LED controls with full color support

**Home Assistant Features**:
- **Lovelace Cards**: Use standard sensor, button, and light cards
- **Automations**: Trigger on button presses, encoder changes, control LED colors
- **Templates**: Access encoder values in template sensors and automations
- **Scripts**: Control LED colors and read encoder states programmatically

## Troubleshooting

### Device Not Found
1. Check I2C connections
2. Verify I2C address (should be 0x41)
3. Enable I2C scanning: `scan: true`
4. Check power supply (3.3V or 5V)

### Erratic Readings
1. Reduce update intervals if needed
2. Check for I2C bus interference
3. Verify stable power supply
4. Check for loose connections

### LED Control Issues
1. Ensure proper I2C communication
2. Check if device supports RGB mode
3. Verify channel numbers (1-8)

## Component Dependencies

- **i2c**: Required for communication
- **sensor**: For encoder count values
- **binary_sensor**: For button states
- **light**: For RGB LED control

## Compatible Platforms

- **ESP32**: All variants (ESP32, ESP32-S2, ESP32-S3, ESP32-C3, etc.)
- **ESP8266**: Standard ESP8266 modules  
- **RP2040**: Raspberry Pi Pico and compatible boards
- **LibreTiny**: Realtek and Beken based modules

**Frameworks**: Arduino and ESP-IDF supported on applicable platforms

## References

- [M5Stack 8Encoder Unit Documentation](https://docs.m5stack.com/en/unit/8encoder)
- [ESPHome I2C Component](https://esphome.io/components/i2c.html)
- [ESPHome Sensor Component](https://esphome.io/components/sensor/index.html)
- [ESPHome Binary Sensor Component](https://esphome.io/components/binary_sensor/index.html)
- [ESPHome Light Component](https://esphome.io/components/light/index.html)