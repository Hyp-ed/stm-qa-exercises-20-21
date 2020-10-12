#ifndef OVEN_HEATING_HEATING_HPP_
#define OVEN_HEATING_HEATING_HPP_

#include "data/data.hpp"
#include "utils/logger.hpp"

namespace oven {

using hyped::utils::Logger;
using oven::data::Data;
using oven::data::ModuleStatus;

namespace heating {
class Heating {
 public:
  explicit Heating(Logger &log);
  ModuleStatus getModuleStatus() const;

 private:
  ModuleStatus status_;
  Logger &log_;
  Data &data_;
};
}  // namespace heating

}  // namespace oven

#endif