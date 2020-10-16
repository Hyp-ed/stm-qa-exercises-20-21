
#include "ui/ui.hpp";

namespace hyped {

using data::State;
using data::Data;
using hyped::utils::Logger;
using utils::System;
namespace user_interface {

User_Interface::User_Interface(Logger& log):
                    log_(log),
                    data_(Data::getInstance()),
                    module_status_(ModuleStatus::kStart)
            {
              log_.INFO("UI","Starting User-Interface Module");
              module_status_ = ModuleStatus::kInit;
              log_.INFO("UI","Initalised User-Interface Module");

            };

void printData(){

    //Print Information about Temperatures, current state machine state
    Logger log = System::getLogger();
    Data& data = Data::getInstance();
    data::HeatingElement heat_data = data.getHeatingElementData();
}


} //namespace user_interface
} //namespace hyped
