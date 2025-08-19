#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "../m5stack_8encoder.h"

namespace esphome {
namespace m5stack_8encoder {

class M5Stack8EncoderSensor : public sensor::Sensor,
                              public PollingComponent,
                              public Parented<M5Stack8EncoderComponent> {
 public:
  void set_channel(uint8_t channel) { this->channel_ = channel; }
  
  void update() override;
  void dump_config() override;

 protected:
  uint8_t channel_;
};

}  // namespace m5stack_8encoder
}  // namespace esphome