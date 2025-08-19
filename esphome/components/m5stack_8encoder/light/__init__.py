import esphome.codegen as cg
from esphome.components import light
import esphome.config_validation as cv
from esphome.const import CONF_CHANNEL, CONF_OUTPUT_ID

from .. import CONF_M5STACK_8ENCODER_ID, M5Stack8EncoderComponent, m5stack_8encoder_ns

M5Stack8EncoderLightOutput = m5stack_8encoder_ns.class_(
    "M5Stack8EncoderLightOutput",
    light.LightOutput,
)


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(M5Stack8EncoderLightOutput),
            cv.GenerateID(CONF_M5STACK_8ENCODER_ID): cv.use_id(M5Stack8EncoderComponent),
            cv.Required(CONF_CHANNEL): cv.int_range(min=1, max=8),
        }
    ).extend(light.RGB_LIGHT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    await cg.register_parented(var, config[CONF_M5STACK_8ENCODER_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL] - 1))  # Convert to 0-based index