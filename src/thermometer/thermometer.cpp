#include "thermometer.hpp"

namespace oven {
namespace thermometer {
Thermometer::Thermometer(Logger &log)
    : log_(log),
      data_(Data::getInstance()),
      status_(ModuleStatus::kStart) {
  log_.INFO("TRM", "Thermometer module started");
  status_ = ModuleStatus::kInit;
  log_.INFO("TRM", "Thermometer module initialised");
}
}  // namespace thermometer
}  // namespace oven