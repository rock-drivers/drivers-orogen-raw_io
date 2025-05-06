#ifndef raw_io_TYPES_HPP
#define raw_io_TYPES_HPP

#include <base/Float.hpp>
#include <base/Temperature.hpp>

namespace raw_io {

    struct TemperatureSensorSettings {
        float min_voltage = base::unknown<float>();
        float max_voltage = base::unknown<float>();
        base::Temperature min_temperature;
        base::Temperature max_temperature;
    };
}

#endif
