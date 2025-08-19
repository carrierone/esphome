#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace m5stack_8encoder {

// Based on research and similar M5Stack units, these are the estimated register addresses
static const uint8_t M5STACK_8ENCODER_REGISTER_ENCODER_COUNT = 0x00;   // 8 encoders * 4 bytes each = 32 bytes
static const uint8_t M5STACK_8ENCODER_REGISTER_BUTTON_STATE = 0x20;    // 8 buttons + 1 switch = 9 bits in 2 bytes
static const uint8_t M5STACK_8ENCODER_REGISTER_RGB_LED = 0x30;         // 8 LEDs * 3 bytes RGB = 24 bytes
static const uint8_t M5STACK_8ENCODER_REGISTER_FW_VERSION = 0xFE;      // Firmware version register

class M5Stack8EncoderComponent : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  
  // Encoder functions
  int32_t read_encoder_count(uint8_t channel);
  
  // Button functions  
  bool read_button_state(uint8_t channel, bool *success = nullptr);
  bool read_toggle_switch(bool *success = nullptr);
  
  // LED functions
  bool set_led_color(uint8_t channel, uint8_t red, uint8_t green, uint8_t blue);
  bool set_all_leds_color(uint8_t red, uint8_t green, uint8_t blue);

 protected:
  uint8_t fw_version_;
};

}  // namespace m5stack_8encoder
}  // namespace esphome