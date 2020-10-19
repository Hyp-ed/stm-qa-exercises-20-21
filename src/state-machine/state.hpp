#ifndef STATE_MACHINE_STATE_HPP_
#define STATE_MACHINE_STATE_HPP_

#include "data/data.hpp"
#include "state-machine/main.hpp"
#include "utils/logger.hpp"
#include "utils/system.hpp"
#include "utils/timer.hpp"

namespace oven {

using hyped::utils::Logger;
using data::ModuleStatus;

namespace state_machine {

class Main;  // Forward declaration

class State {
 public:
  State(Logger& log, Main* state_machine);

  void checkEmergencyStop();

  virtual void transitionCheck() = 0;

  Logger&               log_;
  data::Data&           data_;

  data::Thermometer     thermometer_data_;
  data::Heating         heating_data_;
  data::StateMachine    sm_data_;
  data::UserInterface   user_interface_data_;
  

 protected:
  Main* state_machine_;
};

class Off : public State {
 public:
  Off(Logger& log, Main* state_machine) : State(log, state_machine) {}

  /*
   * @brief   Checks for launch command
   */
  void transitionCheck();
};

class Heating : public State {
 public:
  Heating(Logger& log, Main* state_machine) : State(log, state_machine) {}

  /*
   * @brief   Checks if max distance reached
   */
  void transitionCheck();
};

class Idling : public State {
 public:
  Idling(Logger& log, Main* state_machine) : State(log, state_machine) {}

  /*
   * @brief   Checks if command to reset was sent
   */
  void transitionCheck();
};

}  // namespace state_machine
}  // namespace hyped

#endif  // STATE_MACHINE_STATE_HPP_
