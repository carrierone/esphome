#include "m5stack_8encoder_addressable_light.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5stack_8encoder {

static const char *const TAG = "m5stack_8encoder.addressable_light";

void M5Stack8EncoderAddressableLightOutput::setup() {
  this->buf_ = new uint8_t[M5STACK_8ENCODER_NUM_LEDS * 3];
  this->effect_data_ = new uint8_t[M5STACK_8ENCODER_NUM_LEDS];
  memset(this->buf_, 0x00, M5STACK_8ENCODER_NUM_LEDS * 3);
  memset(this->effect_data_, 0x00, M5STACK_8ENCODER_NUM_LEDS);
}

void M5Stack8EncoderAddressableLightOutput::write_state(light::LightState *state) {
  // Write all LEDs at once using the complete buffer
  if (this->parent_ != nullptr) {
    // Send all 24 bytes (8 LEDs * 3 RGB bytes each) to the device
    for (int i = 0; i < M5STACK_8ENCODER_NUM_LEDS; i++) {
      uint8_t r = this->buf_[i * 3 + 0];
      uint8_t g = this->buf_[i * 3 + 1]; 
      uint8_t b = this->buf_[i * 3 + 2];
      
      bool success = this->parent_->set_led_color(i, r, g, b);
      if (!success) {
        ESP_LOGW(TAG, "Failed to write LED %d color", i);
        return; // Stop on first error
      }
    }
  }
}

M5Stack8EncoderAddressableLightOutput::~M5Stack8EncoderAddressableLightOutput() {
  if (this->buf_ != nullptr) {
    delete[] this->buf_;
    this->buf_ = nullptr;
  }
  if (this->effect_data_ != nullptr) {
    delete[] this->effect_data_;
    this->effect_data_ = nullptr;
  }
}

light::ESPColorView M5Stack8EncoderAddressableLightOutput::get_view_internal(int32_t index) const {
  if (index < 0 || index >= M5STACK_8ENCODER_NUM_LEDS) {
    // Return a safe default for out-of-bounds access
    static uint8_t dummy_buf[3] = {0, 0, 0};
    static uint8_t dummy_effect = 0;
    return light::ESPColorView(dummy_buf, dummy_buf + 1, dummy_buf + 2, nullptr, &dummy_effect, &this->correction_);
  }
  
  int32_t r = index * 3 + 0;
  int32_t g = index * 3 + 1;
  int32_t b = index * 3 + 2;
  return light::ESPColorView(this->buf_ + r, this->buf_ + g, this->buf_ + b, nullptr, this->effect_data_ + index,
                            &this->correction_);
}

}  // namespace m5stack_8encoder
}  // namespace esphome