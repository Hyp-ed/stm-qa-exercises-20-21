#include "main.hpp"
#include <iostream>
namespace oven {
using data::Data;
using data::State;
using thermometer::Main;

Main::Main(uint8_t id, Logger &log)
    : Thread(id, log),
      log_(log),
      sys_(System::getSystem()),
      thermometer_(log) {
}

void Main::run() {
  log_.DBG("TRM", "Thermometer done.");
}
}  // namespace oven