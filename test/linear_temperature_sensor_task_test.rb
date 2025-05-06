# frozen_string_literal: true

using_task_library "raw_io"
using_task_library "base"

describe OroGen.raw_io.LinearTemperatureSensorTask do
    run_live

    attr_reader :task

    before do
        @task = syskit_deploy(
            OroGen.raw_io.LinearTemperatureSensorTask
        )
        sensor_config = Types.raw_io.TemperatureSensorSettings.new
        sensor_config.min_voltage = 0
        sensor_config.max_voltage = 100
        sensor_config.min_temperature = 0
        sensor_config.max_temperature = 50

        @task.properties.sensor_config = sensor_config
        syskit_configure(@task)
        expect_execution { task.start! }
            .to { emit task.start_event }

        @now = Time.now
        @analog_input = [Types.raw_io.Analog.new(time: @now, data: 0),
                         Types.raw_io.Analog.new(time: @now, data: 100),
                         Types.raw_io.Analog.new(time: @now, data: 50)]
        @temperatures = [Types.base.samples.Temperature.new(time: @now, kelvin: 273.15),
                         Types.base.samples.Temperature.new(time: @now, kelvin: 323.15),
                         Types.base.samples.Temperature.new(time: @now, kelvin: 298.15)]
    end

    it "converts voltages to temperatures" do
        result = expect_execution { syskit_write @task.analog_samples_port, @analog_input }
                 .to { have_one_new_sample task.temperatures_port }

        assert_equal @temperatures[0].kelvin, result[0].kelvin
        assert_equal @temperatures[1].kelvin, result[1].kelvin
        assert_equal @temperatures[2].kelvin, result[2].kelvin
    end

    it "does not output temperatures when there is no analog samples" do
        expect_execution.to { have_no_new_sample task.temperatures_port, at_least_during: 0.5 }
    end

    it "does not output temperatures when there is no new analog samples" do
        expect_execution { syskit_write @task.analog_samples_port, @analog_input }
                 .to { have_one_new_sample task.temperatures_port }

        expect_execution.to { have_no_new_sample task.temperatures_port, at_least_during: 0.5 }
    end
end
