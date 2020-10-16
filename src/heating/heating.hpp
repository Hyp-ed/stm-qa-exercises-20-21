

#include "data/data.hpp"
#include "utils/logger.hpp"


namespace hyped {

using hyped::utils::Logger;
using hyped::data::Data;
using hyped::data::ModuleStatus;

namespace heating {

  class Heating {
    public:
      explicit Heating(Logger &logger);
      ModuleStatus getHeatingStatus() const;
      void applyHeat();
      void removeHeat();


    private:
      ModuleStatus module_status_;
      Logger &log_;
      Data &data_;
      unsigned int update_counter_;
  };
} //namespace heating
} //namespace hyped
