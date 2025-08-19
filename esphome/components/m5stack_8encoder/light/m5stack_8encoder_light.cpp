#include "m5stack_8encoder_light.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5stack_8encoder {

static const char *const TAG = "m5stack_8encoder.light";

void M5Stack8EncoderLightOutput::write_state(light::LightState *state) {
  float red, green, blue;
  state->current_values_as_rgb(&red, &green, &blue);
  
  // Convert float values (0.0-1.0) to uint8_t (0-255)
  uint8_t r = (uint8_t) (red * 255);
  uint8_t g = (uint8_t) (green * 255);
  uint8_t b = (uint8_t) (blue * 255);
  
  bool success = this->parent_->set_led_color(this->channel_, r, g, b);
  if (!success) {
    ESP_LOGW(TAG, "Failed to set LED %d color to RGB(%d,%d,%d)", this->channel_ + 1, r, g, b);
  }
}

void M5Stack8EncoderLightOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "M5Stack 8Encoder Light Output:");
  ESP_LOGCONFIG(TAG, "  Channel: %d", this->channel_ + 1);  // Display as 1-based
}

}  // namespace m5stack_8encoder
}  // namespace esphome