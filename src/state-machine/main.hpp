#ifndef OVEN_STATE_MACHINE_MAIN_HPP_
#define OVEN_STATE_MACHINE_MAIN_HPP_

#include <cstdint>
#include "utils/concurrent/thread.hpp"
#include "data/data.hpp"
#include "utils/system.hpp"
#include "state-machine/state.hpp"
#include "utils/config.hpp"


namespace oven {

using hyped::utils::concurrent::Thread;
using hyped::utils::Logger;
using data::ModuleStatus;

namespace state_machine {

class State;
class Off;
class Heating;
class Idling;
class Main: public Thread {
 public:
  explicit Main(uint8_t id, Logger& log);
  void run() override;

  State         *current_state_;
  Off           *off_;
  Heating       *heating_;
  Idling        *idling_;
        
};

}  // namespace state_machine
}  // namespace hyped

#endif