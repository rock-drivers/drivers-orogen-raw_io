# frozen_string_literal: true

using_task_library "raw_io"

describe OroGen.raw_io.PWMServoTask do
    run_live

    attr_reader :task

    before do
        @task = syskit_deploy(
            OroGen.raw_io.PWMServoFeedbackTask
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

    it "goes into exception if the input size is below the expected" do
        input = { on_durations: [0] }
        expect_execution { syskit_write task.pwm_port, input }
            .to_emit(task.input_size_mismatch_event)
    end

    it "goes into exception if the input size is above the expected" do
        input = { on_durations: [0, 1, 2] }
        expect_execution { syskit_write task.pwm_port, input }
            .to_emit(task.input_size_mismatch_event)
    end

    it "converts the PWMs into positions" do
        input = { on_durations: [250, 950] }
        sample =
            expect_execution { syskit_write task.pwm_port, input }
            .to_have_one_new_sample(task.joints_port)

        [-0.45, -0.3].zip(sample.elements) do |a, j|
            assert_in_delta a, j.position
        end
    end

    it "forwards the timestamp" do
        time = Time.now.floor(6)
        pwm = { time: time, on_durations: [0, 0] }
        joints =
            expect_execution { syskit_write task.pwm_port, pwm }
            .to_have_one_new_sample(task.joints_port)

        assert_equal time, joints.time
    end
end