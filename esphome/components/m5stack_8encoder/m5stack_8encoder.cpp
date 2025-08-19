#include "m5stack_8encoder.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <climits>

namespace esphome {
namespace m5stack_8encoder {

static const char *const TAG = "m5stack_8encoder";

void M5Stack8EncoderComponent::setup() {
  i2c::ErrorCode err;

  // Test I2C communication
  err = this->read(nullptr, 0);
  if (err != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "I2C communication failed: %02X", err);
    this->mark_failed();
    return;
  }

  // Read firmware version
  err = this->read_register(M5STACK_8ENCODER_REGISTER_FW_VERSION, &this->fw_version_, 1);
  if (err != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Failed to read firmware version: %02X", err);
    this->mark_failed();
    return;
  }

  ESP_LOGD(TAG, "M5Stack 8Encoder setup complete");
}

void M5Stack8EncoderComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "M5Stack 8Encoder:");
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "  Firmware version: %d", this->fw_version_);
}

int32_t M5Stack8EncoderComponent::read_encoder_count(uint8_t channel) {
  if (channel >= 8) {
    ESP_LOGE(TAG, "Invalid encoder channel: %d", channel);
    return INT32_MIN;  // Error indicator
  }

  uint8_t data[4];
  i2c::ErrorCode err = this->read_register(M5STACK_8ENCODER_REGISTER_ENCODER_COUNT + (channel * 4), 
                                          data, 4);
  
  if (err != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Failed to read encoder %d count: %02X", channel, err);
    this->status_set_warning("I2C communication failed");
    return INT32_MIN;  // Error indicator
  }

  this->status_clear_warning();
  // Assume little-endian byte order from STM32F030 (LSB first)
  // encode_uint32 expects bytes in MSB to LSB order, so we pass them in reverse
  return (int32_t) encode_uint32(data[3], data[2], data[1], data[0]);
}


bool M5Stack8EncoderComponent::read_button_state(uint8_t channel, bool *success) {
  if (success) *success = true;
  
  if (channel >= 8) {
    ESP_LOGE(TAG, "Invalid button channel: %d", channel);
    if (success) *success = false;
    return false;
  }

  uint8_t data[2];
  i2c::ErrorCode err = this->read_register(M5STACK_8ENCODER_REGISTER_BUTTON_STATE, data, 2);
  
  if (err != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Failed to read button states: %02X", err);
    this->status_set_warning("I2C communication failed");
    if (success) *success = false;
    return false;
  }

  this->status_clear_warning();
  // Combine bytes to create 16-bit value (little-endian)
  uint16_t button_data = data[0] | (data[1] << 8);
  return (button_data & (1 << channel)) != 0;
}

bool M5Stack8EncoderComponent::read_toggle_switch(bool *success) {
  if (success) *success = true;
  
  uint8_t data[2];
  i2c::ErrorCode err = this->read_register(M5STACK_8ENCODER_REGISTER_BUTTON_STATE, data, 2);
  
  if (err != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Failed to read toggle switch state: %02X", err);
    this->status_set_warning("I2C communication failed");
    if (success) *success = false;
    return false;
  }

  this->status_clear_warning();
  // Combine bytes to create 16-bit value (little-endian)
  uint16_t button_data = data[0] | (data[1] << 8);
  // Toggle switch is bit 8 (after the 8 encoder buttons)
  return (button_data & (1 << 8)) != 0;
}

bool M5Stack8EncoderComponent::set_led_color(uint8_t channel, uint8_t red, uint8_t green, uint8_t blue) {
  if (channel >= 8) {
    ESP_LOGE(TAG, "Invalid LED channel: %d", channel);
    return false;
  }

  uint8_t rgb_data[3] = {red, green, blue};
  i2c::ErrorCode err = this->write_register(M5STACK_8ENCODER_REGISTER_RGB_LED + (channel * 3), 
                                           rgb_data, 3);
  
  if (err != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Failed to set LED %d color: %02X", channel, err);
    this->status_set_warning("I2C communication failed");
    return false;
  }

  this->status_clear_warning();
  return true;
}

bool M5Stack8EncoderComponent::set_all_leds_color(uint8_t red, uint8_t green, uint8_t blue) {
  uint8_t rgb_data[24];  // 8 LEDs * 3 bytes each
  
  for (int i = 0; i < 8; i++) {
    rgb_data[i * 3] = red;
    rgb_data[i * 3 + 1] = green;
    rgb_data[i * 3 + 2] = blue;
  }

  i2c::ErrorCode err = this->write_register(M5STACK_8ENCODER_REGISTER_RGB_LED, rgb_data, 24);
  
  if (err != i2c::NO_ERROR) {
    ESP_LOGE(TAG, "Failed to set all LED colors: %02X", err);
    this->status_set_warning("I2C communication failed");
    return false;
  }

  this->status_clear_warning();
  return true;
}

float M5Stack8EncoderComponent::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace m5stack_8encoder
}  // namespace esphome