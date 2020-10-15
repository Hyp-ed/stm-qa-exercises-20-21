#include "heating\main.hpp"
namespace hyped{

  namespace heating{
  Main::Main(uint8_t id, Logger& log)
      : Thread(id, log),
        log_(log),
        sys_(System::getSystem()),
        heat_(Heating(log)) {}
  void Main::run(){
    data::Data& d_ = data::Data::getInstance();
    data::State current_state = d_.getStateMachineData().current_state;
    while(sys_.running_){
      data::State current_state = d_.getStateMachineData().current_state;
      switch(current_state){
        case State::kOff:
        case State::kEmergency:
          break;
        case State::kHeating:
          heat_.applyHeat();
          break;
        case State::kIdling:
          heat_.removeHeat();
          break;
        default:
          break;




      }
    }


  }
  }
  }
