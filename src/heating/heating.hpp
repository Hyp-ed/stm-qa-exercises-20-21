

#include "data/data.hpp"
#include "utils/logger.hpp"


namespace hyped {

using hyped::utils::Logger;
using hyped::data::Data;
using hyped::data::ModuleStatus;

namespace heating {

  class Heating {
    public:
      void HeatingState(Logger &logger);
      ModuleStatus Heating::getHeatingState() const;

    private:
      ModuleStatus module_status_;
      Logger &logger_;
      Data &data_;
  };
} //namespace heating
} //namespace hyped
