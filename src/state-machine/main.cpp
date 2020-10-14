#include "main.hpp"

namespace oven {
namespace state_machine {

Main::Main(uint8_t id, Logger& log) : Thread(id, log)
{
  off_ = new Off(log_, this);  // constructing state object for Off
  heating_ = new Heating(log_, this);  // constructing state object for Heating
  idling_ = new Idling(log_, this);  // constructing state object for Idling

  current_state_ = off_;  // set current state to point to Idle
}

/**
  *  @brief  Runs state machine thread.
  */
void Main::run()
{
  utils::System& sys = utils::System::getSystem();
  data::Data& data = data::Data::getInstance();

  data::StateMachine sm_data = data.getStateMachineData();
  sm_data.current_state = data::State::kOff;  // set current state in data structure
  data.setStateMachineData(sm_data);

  while (sys.running_) {
    current_state_->checkEmergencyStop();
    current_state_->transitionCheck();
  }

  sm_data = data.getStateMachineData();
  log_.INFO("STM", "Exiting. Current state: %s", data::states[sm_data.current_state]);
}

}  // namespace state_machine
}  // namespace hyped