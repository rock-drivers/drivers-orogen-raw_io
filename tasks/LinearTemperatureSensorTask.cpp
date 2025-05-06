/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "LinearTemperatureSensorTask.hpp"

using namespace raw_io;
using namespace base::samples;

LinearTemperatureSensorTask::LinearTemperatureSensorTask(std::string const& name)
    : LinearTemperatureSensorTaskBase(name)
{
}

LinearTemperatureSensorTask::~LinearTemperatureSensorTask()
{
}

// Convert the voltage in temperature
Temperature temperature(base::Time const& timestamp,
    double voltage,
    LinearTemperatureSensorSettings const& sensor_config)
{
    auto temperature =
        sensor_config.min_temperature +
        (voltage - sensor_config.min_voltage) *
            ((sensor_config.max_temperature - sensor_config.min_temperature) *
                (1 / (sensor_config.max_voltage - sensor_config.min_voltage)));
    return Temperature(timestamp, temperature);
}

std::vector<Temperature> convertVoltagesToTemperatures(base::Time const& timestamp,
    std::vector<raw_io::Analog> const& voltages,
    LinearTemperatureSensorSettings const& sensor_config)
{
    std::vector<Temperature> temperatures;
    for (auto const& voltage : voltages) {
        temperatures.push_back(temperature(timestamp, voltage.data, sensor_config));
    }
    return temperatures;
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See LinearTemperatureSensorTask.hpp for more detailed
// documentation about them.

bool LinearTemperatureSensorTask::configureHook()
{
    if (!LinearTemperatureSensorTaskBase::configureHook())
        return false;
    m_temperature_sensor_config = _sensor_config.get();
    return true;
}
bool LinearTemperatureSensorTask::startHook()
{
    if (!LinearTemperatureSensorTaskBase::startHook())
        return false;
    return true;
}
void LinearTemperatureSensorTask::updateHook()
{
    std::vector<raw_io::Analog> voltages;
    if (_analog_samples.read(voltages) == RTT::NewData) {
        std::vector<Temperature> temperatures =
            convertVoltagesToTemperatures(base::Time::now(),
                voltages,
                m_temperature_sensor_config);
        _temperatures.write(temperatures);
    }
    LinearTemperatureSensorTaskBase::updateHook();
}
void LinearTemperatureSensorTask::errorHook()
{
    LinearTemperatureSensorTaskBase::errorHook();
}
void LinearTemperatureSensorTask::stopHook()
{
    LinearTemperatureSensorTaskBase::stopHook();
}
void LinearTemperatureSensorTask::cleanupHook()
{
    LinearTemperatureSensorTaskBase::cleanupHook();
}
