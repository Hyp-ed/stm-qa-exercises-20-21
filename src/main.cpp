#include "main.hpp"

#include "thermometer/main.hpp"
#include "utils/concurrent/thread.hpp"
#include "utils/logger.hpp"
#include "utils/system.hpp"

using hyped::utils::Logger;
using hyped::utils::System;
using hyped::utils::concurrent::Thread;

int main(int argc, char **argv) {
  System::parseArgs(argc, argv);
  System &sys = System::getSystem();
  sys.debug = 4;
  sys.verbose = true;
  Logger log(sys.verbose, sys.debug);

  log.DBG("MAIN", "DBG0");
  log.DBG1("MAIN", "DBG1");
  log.DBG2("MAIN", "DBG2");
  log.DBG3("MAIN", "DBG3");

  Thread *trm = new oven::thermometer::Main(0, log);
  trm->start();
  trm->join();
}