#include "data/data.hpp"
#include "utils/logger.hpp"
#include "utils/system.hpp"

namespace hyped {

using hyped::utils::Logger;
using hyped::data::Data;
using hyped::data::ModuleStatus;

namespace user_interface {

  class User_Interface {
    public:
      explicit User_Interface(Logger &logger);
      ModuleStatus getUiStatus(Logger &logger) const;
      void printData();

    private:
      ModuleStatus module_status_;
      utils::Logger &log_;
      Data &data_;
  };
} //namespace heating
} //namespace hyped
