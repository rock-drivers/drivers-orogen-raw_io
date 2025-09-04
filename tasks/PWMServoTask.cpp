/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "PWMServoTask.hpp"

using namespace raw_io;

PWMServoTask::PWMServoTask(std::string const& name, TaskCore::TaskState initial_state)
    : PWMServoTaskBase(name, initial_state)
{
}

PWMServoTask::~PWMServoTask()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See PWMServoTask.hpp for more detailed
// documentation about them.

bool PWMServoTask::configureHook()
{
    if (! PWMServoTaskBase::configureHook())
        return false;

    m_servos.clear();
    for (auto const& conf: _conf.get()) {
        m_servos.emplace_back(PWMServo{conf});
    }
    m_cmd.resize(m_servos.size());
    m_pwm.on_durations.reserve(m_servos.size());
    return true;
}
bool PWMServoTask::startHook()
{
    if (! PWMServoTaskBase::startHook())
        return false;
    return true;
}
void PWMServoTask::updateHook()
{
    PWMServoTaskBase::updateHook();

    while(_cmd.read(m_cmd) == RTT::NewData) {
        if (m_cmd.size() != m_servos.size()) {
            exception(INPUT_SIZE_MISMATCH);
            return;
        }

        m_pwm.on_durations.clear();
        for (size_t i = 0; i < m_servos.size(); ++i) {
            auto const& j { m_cmd[i] };
            if (!j.hasPosition()) {
                exception(INPUT_NOT_POSITION);
                return;
            }

            auto angle = base::Angle::fromRad(j.position);
            auto normalized_angle = m_servos[i].clampAngleToValidRange(angle);

            m_pwm.on_durations.push_back(m_servos[i].toPWM(normalized_angle));
        }

        m_pwm.time = m_cmd.time;
        _pwm.write(m_pwm);
    }
}
void PWMServoTask::errorHook()
{
    PWMServoTaskBase::errorHook();
}
void PWMServoTask::stopHook()
{
    PWMServoTaskBase::stopHook();
}
void PWMServoTask::cleanupHook()
{
    PWMServoTaskBase::cleanupHook();
}
