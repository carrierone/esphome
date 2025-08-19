#pragma once

#include "esphome/components/light/light_output.h"
#include "../m5stack_8encoder.h"

namespace esphome {
namespace m5stack_8encoder {

class M5Stack8EncoderLightOutput : public light::LightOutput, public Parented<M5Stack8EncoderComponent> {
 public:
  void set_channel(uint8_t channel) { this->channel_ = channel; }
  
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
  }
  
  void write_state(light::LightState *state) override;
  void dump_config() override;

 protected:
  uint8_t channel_;
};

}  // namespace m5stack_8encoder
}  // namespace esphome