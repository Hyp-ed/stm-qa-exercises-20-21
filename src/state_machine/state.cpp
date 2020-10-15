#include "state.hpp"
namespace hyped{
  namespace state_machine{
  State::State(Logger& log, Main* state_machine):
    data_(data::Data::getInstance()),
    log_(log),
    state_machine_(state_machine)
    {}

  void Off::transitionCheck(){
    user_interface_data = data_.getUserInterfaceData();
    if(user_interface_data.is_on){
      log_.INFO("STM", "launch command received");
      sm_data_.current_state = data::State::kHeating;
      data_.setStateMachineData(sm_data_);
      state_machine_->current_state_ = state_machine_->heating_;
      log_.DBG("STM", "Transitioned to 'heating'");
     }



    }
  void Heating::transitionCheck(){
    heating_data = data_.getHeatingElementData();
    sm_data_ =data_.getStateMachineData();
    user_interface_data = data_.getUserInterfaceData();
    if(!user_interface_data.is_on){
      log_.INFO("STM", "Off command received");
      sm_data_.current_state = data::State::kOff;
      data_.setStateMachineData(sm_data_);
      state_machine_->current_state_ = state_machine_->off_;
      log_.DBG("STM", "Transitioned to 'Off'");
      }
    else if(heating_data.current_temp>user_interface_data.target_temp){
      log_.INFO("STM", "Target Temperature surpassed");
      sm_data_.current_state = data::State::kIdling;
      data_.setStateMachineData(sm_data_);
      state_machine_->current_state_ = state_machine_->idle_;
      log_.DBG("STM", "Transitioned to 'Idling'");
      }
    }
  void Idle::transitionCheck(){
    heating_data = data_.getHeatingElementData();
    sm_data_ = data_.getStateMachineData();
    user_interface_data = data_.getUserInterfaceData();
    if (!user_interface_data.is_on) {
      log_.INFO("STM", "Off command received");
      sm_data_.current_state = data::State::kOff;
      data_.setStateMachineData(sm_data_);
      state_machine_->current_state_ = state_machine_->off_;
      log_.DBG("STM", "Transitioned to 'Off'");
      }
     else if (heating_data.current_temp <= user_interface_data.target_temp) {
      log_.INFO("STM", "Temperature too low");
      sm_data_.current_state = data::State::kIdling;
      data_.setStateMachineData(sm_data_);
      state_machine_->current_state_ = state_machine_->idle_;
      log_.DBG("STM", "Transitioned to 'Heating'");
      }
    }
  }
}
