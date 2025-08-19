#include "m5stack_8encoder_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5stack_8encoder {

static const char *const TAG = "m5stack_8encoder.binary_sensor";

void M5Stack8EncoderBinarySensor::update() {
  if (this->parent_ != nullptr) {
    bool state = false;
    bool success = true;
    
    if (this->type_ == "button") {
      state = this->parent_->read_button_state(this->channel_, &success);
    } else if (this->type_ == "toggle") {
      state = this->parent_->read_toggle_switch(&success);
    }
    
    if (success) {
      this->publish_state(state);
      this->status_clear_warning();
    } else {
      this->status_set_warning("Could not read binary sensor state from M5Stack 8Encoder.");
    }
  }
}

void M5Stack8EncoderBinarySensor::dump_config() {
  ESP_LOGCONFIG(TAG, "M5Stack 8Encoder Binary Sensor:");
  ESP_LOGCONFIG(TAG, "  Type: %s", this->type_.c_str());
  ESP_LOGCONFIG(TAG, "  Channel: %d", this->channel_ + 1);  // Display as 1-based
  LOG_BINARY_SENSOR("  ", "State", this);
  LOG_UPDATE_INTERVAL(this);
}

}  // namespace m5stack_8encoder
}  // namespace esphome