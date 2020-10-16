#include "state-machine/state.hpp"
using hyped::utils::Logger;
using hyped::utils::System;
using oven::data::Data;
using oven::data::ModuleStatus;

namespace oven {

namespace state_machine {

State::State(Logger& log, Main* state_machine)
  : log_(log),
    data_(data::Data::getInstance()),
    state_machine_(state_machine)
{}

void State::checkEmergencyStop()
{
  telemetry_data_ = data_.getTelemetryData();
  sm_data_        = data_.getStateMachineData();

  if (telemetry_data_.emergency_stop_command) {
    log_.ERR("STM", "STOP command received");
    telemetry_data_.emergency_stop_command = false;
    data_.setTelemetryData(telemetry_data_);

    sm_data_.current_state = data::State::kFinished;
    data_.setStateMachineData(sm_data_);

    state_machine_->current_state_ = state_machine_->finished_;
  }
}

// Off state

void Off::transitionCheck()
{
  user_interface_data_ = data_.getUserInterfaceData();
  sm_data_             = data_.getStateMachineData();

  if (user_interface_data_.is_on) {
    log_.INFO("STM", "Oven turned on");
    log_.DBG("STM", "launch command cleared");

    sm_data_.current_state = data::State::kHeating;
    data_.setStateMachineData(sm_data_);

    state_machine_->current_state_ = state_machine_->heating_;
    log_.DBG("STM", "Transitioned to 'Heating'");
  }
}

// Heating state

void Heating::transitionCheck()
{
  thermometer_data_       = data_.getThermometerData();
  heating_data_           = data_.getHeatingData();
  sm_data_                = data_.getStateMachineData();
  user_interface_data_    = data_.getUserInterfaceData();

  if (user_interface_data_.is_on == false) {
    log_.INFO("STM", "Oven turned off");
    log_.DBG("STM", "launch command cleared");

    sm_data_.current_state = data::State::kOff;
    data_.setStateMachineData(sm_data_);

    state_machine_->current_state_ = state_machine_->off_;
    log_.DBG("STM", "Transitioned to 'Off'");
  }

  if (thermometer_data_.temperature > user_interface_data.target_temperature) {
    log_.INFO("STM", "max temperature passed");
    log_.INFO("STM", "current temperature: %fm, target temperature: %fm",
              thermometer_data_.temperature, user_interface_data.target_temperature);

    sm_data_.current_state = data::State::kIdling;
    data_.setStateMachineData(sm_data_);

    state_machine_->current_state_ = state_machine_->idling_;
    log_.DBG("STM", "Transitioned to 'Idling'");
  }
}

// Idling state

void Idling::transitionCheck()
{
  thermometer_data_       = data_.getThermometerData();
  heating_data_           = data_.getHeatingData();
  sm_data_                = data_.getStateMachineData();
  user_interface_data_    = data_.getUserInterfaceData();

  if (user_interface_data_.is_on == false) {
    log_.INFO("STM", "Oven turned off");
    log_.DBG("STM", "launch command cleared");

    sm_data_.current_state = data::State::kOff;
    data_.setStateMachineData(sm_data_);

    state_machine_->current_state_ = state_machine_->off_;
    log_.DBG("STM", "Transitioned to 'Off'");
  }

  if (thermometer_data_.temperature <= user_interface_data.target_temperature) {
    log_.INFO("STM", "returning to normal limits");
    log_.INFO("STM", "current temperature: %fm, target temperature: %fm",
              thermometer_data_.temperature, user_interface_data.target_temperature);

    sm_data_.current_state = data::State::kHeating;
    data_.setStateMachineData(sm_data_);

    state_machine_->current_state_ = state_machine_->heating_;
    log_.DBG("STM", "Transitioned to 'Heating'");
  }

  }


}

}  // namespace state_machine
}  // namespace hyped