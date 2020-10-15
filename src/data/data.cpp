
#include "data/data.hpp"

namespace hyped {

  using hyped::utils::concurrent::ScopedLock;

  namespace data {

    Data &Data::getInstance(){
      static Data d;
      return d;
    }

    StateMachine Data::getStateMachineData(){

      ScopedLock L(&lock_state_machine_);
      return state_machine_;
    }

    void Data::setStateMachineData(const StateMachine &sm_data){
      ScopedLock L(&lock_state_machine_);
      state_machine_ = sm_data;
    }

    HeatingElement Data::getHeatingElementData(){
      ScopedLock L(&lock_heating_element_);
      return heating_element_;
    }

    void Data::setHeatingElementData(const HeatingElement &heat_data){
        ScopedLock L(&lock_heating_element_);
        heating_element_ = heat_data;
    }

    UserInterface Data::getUserInterfaceData(){
      ScopedLock L(&lock_user_interface_);
      return user_interface_;
    }

    void Data::setUserInterfaceData(const UserInterface &ui_data){
      ScopedLock L(&lock_user_interface_);
      user_interface_ = ui_data;
    }


  }
}
