/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "PWMServoFeedbackTask.hpp"
#include <base-logging/Logging.hpp>

using namespace raw_io;

PWMServoFeedbackTask::PWMServoFeedbackTask(std::string const& name,
    TaskCore::TaskState initial_state)
    : PWMServoFeedbackTaskBase(name, initial_state)
{
}

PWMServoFeedbackTask::~PWMServoFeedbackTask()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See PWMServoFeedbackTask.hpp for more detailed
// documentation about them.

bool PWMServoFeedbackTask::configureHook()
{
    if (!PWMServoFeedbackTaskBase::configureHook())
        return false;

    m_conf = _conf.get();
    for (auto const& conf : _conf.get()) {
        m_servos.emplace_back(PWMServo{conf});
    }
    m_joints.resize(m_servos.size());
    return true;
}
bool PWMServoFeedbackTask::startHook()
{
    if (!PWMServoFeedbackTaskBase::startHook())
        return false;
    return true;
}
void PWMServoFeedbackTask::updateHook()
{
    PWMServoFeedbackTaskBase::updateHook();

    while (_pwm.read(m_pwm) == RTT::NewData) {
        if (m_pwm.on_durations.size() != m_servos.size()) {
            LOG_ERROR_S << "expected input of size " << m_servos.size() << "but got "
                        << m_pwm.on_durations.size();
            exception(INPUT_SIZE_MISMATCH);
            return;
        }

        for (size_t i = 0; i < m_servos.size(); ++i) {
            m_joints.elements[i].position =
                m_servos[i].fromPWM(m_pwm.on_durations[i]).getRad();
        }

        m_joints.time = m_pwm.time;
        _joints.write(m_joints);
    }
}
void PWMServoFeedbackTask::errorHook()
{
    PWMServoFeedbackTaskBase::errorHook();
}
void PWMServoFeedbackTask::stopHook()
{
    PWMServoFeedbackTaskBase::stopHook();
}
void PWMServoFeedbackTask::cleanupHook()
{
    PWMServoFeedbackTaskBase::cleanupHook();
}
