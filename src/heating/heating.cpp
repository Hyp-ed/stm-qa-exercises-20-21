#include "heating/heating.hpp"


namespace hyped {
  using data::State;
  using data::Data;
namespace heating {

Heating::Heating(Logger& log):
            log_(log),
            data_(Data::getInstance()),
            module_status_(ModuleStatus::kStart)
      {
        log_.INFO("HEAT", "Starting Heating Module");
        module_status_ = ModuleStatus::kInit;
        log_.INFO("HEAT", "Initialised Heating Module");
      }

ModuleStatus Heating::getHeatingStatus() const{
  return module_status_;
}

void Heating::applyHeat(){

  Data& data = Data::getInstance();
  data::HeatingElement heat_data = data.getHeatingElementData();
  heat_data.current_temp += heat_data.heat_increment;
  data.setHeatingElementData(heat_data);

}

void Heating::removeHeat(){

  Data& data = Data::getInstance();
  data::HeatingElement heat_data = data.getHeatingElementData();
  heat_data.current_temp -= heat_data.heat_increment;
  data.setHeatingElementData(heat_data);

}


} //namespace heating
} //namespace hyped
