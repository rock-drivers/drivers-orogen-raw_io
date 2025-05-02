#ifndef raw_io_TYPES_HPP
#define raw_io_TYPES_HPP

#include <base/Float.hpp>
#include <stdint.h>

namespace raw_io {

    struct TemperatureSensorSettings {
        float min_voltage = base::unknown<float>();
        float max_voltage = base::unknown<float>();
        float min_temperature = base::unknown<float>();
        float max_temperature = base::unknown<float>();
    };
}

#endif
