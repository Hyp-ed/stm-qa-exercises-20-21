/*
 * Author:Kornelija Sukyte
 * Organisation: HYPED
 * Date:
 * Description:
 *
 *    Copyright 2020 HYPED
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#include <cstdint>
#include "state_machine/main.hpp"

namespace hyped {
namespace state_machine {

Main::Main(uint8_t id, Logger& log) : Thread(id, log)
{
  off_ = new Off(log_, this);  // constructing state object for Off
  heating_ = new Heating(log_, this);  // constructing state object for Heating
  idle_ = new Idle(log_, this);  // constructing state object for Idle

  current_state_ = off_;  // set current state to point to off
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
    //current_state_->checkEmergencyStop();
    current_state_->transitionCheck();
  }

  sm_data = data.getStateMachineData();
  log_.INFO("STM", "Exiting. Current state: %s", data::states[sm_data.current_state]);
}

}  // namespace state_machine
}  // namespace hyped