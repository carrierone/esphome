import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import CONF_CHANNEL, CONF_TYPE, DEVICE_CLASS_BUTTON

from .. import CONF_M5STACK_8ENCODER_ID, M5Stack8EncoderComponent, m5stack_8encoder_ns

CONF_BUTTON = "button"
CONF_TOGGLE = "toggle"

M5Stack8EncoderBinarySensor = m5stack_8encoder_ns.class_(
    "M5Stack8EncoderBinarySensor",
    binary_sensor.BinarySensor,
    cg.PollingComponent,
)

BINARY_SENSOR_TYPES = {
    CONF_BUTTON: "button",
    CONF_TOGGLE: "toggle",
}


def validate_config(config):
    # Both button and toggle types require a channel (1-8)
    if CONF_CHANNEL not in config:
        raise cv.Invalid("Channel must be specified")
    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(M5Stack8EncoderBinarySensor),
            cv.GenerateID(CONF_M5STACK_8ENCODER_ID): cv.use_id(M5Stack8EncoderComponent),
            cv.Required(CONF_TYPE): cv.enum(BINARY_SENSOR_TYPES, lower=True),
            cv.Required(CONF_CHANNEL): cv.int_range(min=1, max=8),
        }
    )
    .extend(binary_sensor.binary_sensor_schema(M5Stack8EncoderBinarySensor))
    .extend(cv.polling_component_schema("50ms")),
    validate_config,
)


async def to_code(config):
    # Set device class for buttons
    if config[CONF_TYPE] == CONF_BUTTON:
        config[binary_sensor.CONF_DEVICE_CLASS] = DEVICE_CLASS_BUTTON
    
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_M5STACK_8ENCODER_ID])
    
    cg.add(var.set_type(config[CONF_TYPE]))
    
    cg.add(var.set_channel(config[CONF_CHANNEL] - 1))  # Convert to 0-based index