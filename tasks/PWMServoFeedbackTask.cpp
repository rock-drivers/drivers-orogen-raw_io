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

    std::vector<PWMServo> servos;
    for (auto const& conf : _conf.get()) {
        servos.emplace_back(PWMServo{conf});
    }
    m_joints.resize(servos.size());
    m_servos = servos;
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

        bool all_valid = true;
        for (size_t i = 0; all_valid && i < m_servos.size(); ++i) {
            auto position = m_servos[i].fromPWM(m_pwm.on_durations[i]).getRad();
            m_joints.elements[i].position = position;

            if (base::isUnknown(position)) {
                all_valid = false;
            }
        }

        if (!all_valid) {
            continue;
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
