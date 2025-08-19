#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/component.h"
#include "../m5stack_8encoder.h"

namespace esphome {
namespace m5stack_8encoder {

class M5Stack8EncoderBinarySensor : public binary_sensor::BinarySensor,
                                    public PollingComponent,
                                    public Parented<M5Stack8EncoderComponent> {
 public:
  void set_type(const std::string &type) { this->type_ = type; }
  void set_channel(uint8_t channel) { this->channel_ = channel; }
  
  void update() override;
  void dump_config() override;

 protected:
  std::string type_;
  uint8_t channel_{0};
};

}  // namespace m5stack_8encoder
}  // namespace esphome