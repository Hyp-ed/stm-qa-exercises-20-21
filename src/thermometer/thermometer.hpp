#ifndef OVEN_THERMOMETER_THERMOMETER_HPP_
#define OVEN_THERMOMETER_THERMOMETER_HPP_

#include "data/data.hpp"
#include "utils/logger.hpp"

namespace oven {

using hyped::utils::Logger;
using oven::data::Data;
using oven::data::ModuleStatus;

namespace thermometer {
class Thermometer {
 public:
  explicit Thermometer(Logger &log);
  ModuleStatus getModuleStatus() const;

 private:
  ModuleStatus status_;
  Logger &log_;
  Data &data_;
};
}  // namespace thermometer

}  // namespace oven

#endif