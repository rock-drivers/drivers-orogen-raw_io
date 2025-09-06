# frozen_string_literal: true

using_task_library "raw_io"

describe OroGen.raw_io.PWMServoTask do
    run_live

    attr_reader :task

    before do
        @task = syskit_deploy(
            OroGen.raw_io.PWMServoTask
                  .deployed_as("task_under_test")
        )
        @task.properties.conf = [
            {
                angle_min: { rad: -0.5 },
                angle_max: { rad: -0.1 },
                pwm_min: 200,
                pwm_center: 325,
                pwm_deadband_width: 50,
                pwm_max: 650,
                angle_center_threshold: { rad: 1 * Math::PI / 180 }
            },
            { # same conf, but with +500 on the PWM
                angle_min: { rad: -0.5 },
                angle_max: { rad: -0.1 },
                pwm_min: 700,
                pwm_center: 825,
                pwm_deadband_width: 50,
                pwm_max: 1150,
                angle_center_threshold: { rad: 1 * Math::PI / 180 }
            }
        ]

        syskit_configure_and_start(@task)
    end

    it "goes into exception if the joint size is below the configured PWM size" do
        joints = { elements: [{ position: 0 }] }
        expect_execution { syskit_write task.cmd_port, joints }
            .to_emit(task.input_size_mismatch_event)
    end

    it "goes into exception if the joint size is above the configured PWM size" do
        joints = { elements: [{ position: 0 }, { position: 0 }, { position: 0 }] }
        expect_execution { syskit_write task.cmd_port, joints }
            .to_emit(task.input_size_mismatch_event)
    end

    it "goes into exception if one of the joints does not have the position set" do
        joints = { elements: [{ position: -0.4 }, { position: Float::NAN }] }
        expect_execution { syskit_write task.cmd_port, joints }
            .to_emit(task.input_not_position_event)
    end

    it "converts the input joints by interpreting their position fields as radians" do
        joints = { elements: [{ position: -0.45 }, { position: -0.3 }] }
        sample =
            expect_execution { syskit_write task.cmd_port, joints }
            .to_have_one_new_sample(task.pwm_port)

        assert_equal [250, 950], sample.on_durations.to_a
    end

    it "clamps input to the configured angle_min and angle_max" do
        joints = { elements: [{ position: -0.6 }, { position: 0 }] }
        sample =
            expect_execution { syskit_write task.cmd_port, joints }
            .to_have_one_new_sample(task.pwm_port)

        assert_equal [200, 1150], sample.on_durations.to_a
    end

    it "forwards the joints' time" do
        joints = {
            time: Time.now.floor(6),
            elements: [{ position: -0.6 }, { position: 0 }]
        }
        sample =
            expect_execution { syskit_write task.cmd_port, joints }
            .to_have_one_new_sample(task.pwm_port)

        assert_equal sample.time, joints[:time]
    end
end