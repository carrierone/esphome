import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_CHANNEL,
    ICON_ROTATE_RIGHT,
    UNIT_EMPTY,
)

from .. import (
    CONF_M5STACK_8ENCODER_ID,
    M5Stack8EncoderComponent,
    m5stack_8encoder_ns,
)

M5Stack8EncoderSensor = m5stack_8encoder_ns.class_(
    "M5Stack8EncoderSensor",
    sensor.Sensor,
    cg.PollingComponent,
)


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(M5Stack8EncoderSensor),
            cv.GenerateID(CONF_M5STACK_8ENCODER_ID): cv.use_id(M5Stack8EncoderComponent),
            cv.Required(CONF_CHANNEL): cv.int_range(min=1, max=8),
        }
    )
    .extend(
        sensor.sensor_schema(
            M5Stack8EncoderSensor,
            accuracy_decimals=0,
            icon=ICON_ROTATE_RIGHT,
            unit_of_measurement=UNIT_EMPTY,
        )
    )
    .extend(cv.polling_component_schema("1s"))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_M5STACK_8ENCODER_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL] - 1))  # Convert to 0-based index