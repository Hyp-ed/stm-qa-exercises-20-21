#ifndef OVEN_THERMOMETER_MAIN_HPP_
#define OVEN_THERMOMETER_MAIN_HPP_

#include "thermometer.hpp"
#include "utils/concurrent/thread.hpp"
#include "utils/logger.hpp"
#include "utils/system.hpp"
namespace oven {
using hyped::utils::Logger;
using hyped::utils::System;
using hyped::utils::concurrent::Thread;

namespace thermometer {
class Main : public Thread {
 public:
  explicit Main(uint8_t id, Logger &log);
  void run() override;

 private:
  Logger &log_;
  System &sys_;
  Thermometer thermometer_;
};
}  // namespace thermometer

}  // namespace oven

#endif