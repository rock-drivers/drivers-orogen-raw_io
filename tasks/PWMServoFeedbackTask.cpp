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

        if (updateJoints(m_servos, m_joints, m_pwm)) {
            m_joints.time = m_pwm.time;
            _joints.write(m_joints);
        }
    }
}

bool PWMServoFeedbackTask::updateJoints(std::vector<PWMServo> const& servos,
    base::samples::Joints& joints,
    raw_io::PWMDutyDurations const& pwm)
{
    for (size_t i = 0; i < pwm.on_durations.size(); ++i) {
        auto position = servos[i].fromPWM(pwm.on_durations[i]).getRad();
        if (base::isUnknown(position)) {
            return false;
        }

        joints.elements[i].position = position;
    }
    return true;
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
