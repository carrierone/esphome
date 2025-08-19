#include "m5stack_8encoder_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5stack_8encoder {

static const char *const TAG = "m5stack_8encoder.sensor";

void M5Stack8EncoderSensor::update() {
  if (this->parent_ != nullptr) {
    int32_t count = this->parent_->read_encoder_count(this->channel_);
    if (count == INT32_MIN) {  // Use INT32_MIN as error indicator
      this->status_set_warning("Could not read encoder count from M5Stack 8Encoder.");
      return;
    }
    this->publish_state(count);
    this->status_clear_warning();
  }
}

void M5Stack8EncoderSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "M5Stack 8Encoder Sensor:");
  ESP_LOGCONFIG(TAG, "  Channel: %d", this->channel_ + 1);  // Display as 1-based
  LOG_SENSOR("  ", "Encoder Count", this);
  LOG_UPDATE_INTERVAL(this);
}

}  // namespace m5stack_8encoder
}  // namespace esphome