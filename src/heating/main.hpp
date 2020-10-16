/*
 * Author: HYPED
 * Organisation: HYPED
 * Date: 03/10/2020
 * Description:
 * Main initialises and manages sensor drivers. Main is not responsible for initialisation
 * of supporting io drivers (spi, can, adc). This should be done by the sensor
 * drivers themselves.
 *
 *    Copyright 2020 HYPED
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 *    except in compliance with the License. You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software distributed under
 *    the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 *    either express or implied. See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef SENSORS_MAIN_HPP_
#define SENSORS_MAIN_HPP_

#include <cstdint>
#include "heating/heating.hpp"
#include "utils/system.hpp"
#include "utils/concurrent/thread.hpp"


namespace hyped {
using data::Data;
using data::State;
using hyped::data::ModuleStatus;
using hyped::data::StateMachine;
using utils::Logger;
using utils::System;
using utils::concurrent::Thread;

namespace heating {

/**
 * @brief Initialise sensors, data instances to be pulled in managers
 *        gpio threads and adc checks declared in main
 */
class Main : public Thread {
 public:
  explicit Main(uint8_t id, Logger& log);
  void run() override;


 private:
  Logger& log_;
  System& sys_;
  Heating heat_;
};
}}
  // namespace hyped::sensors

#endif  // SENSORS_MAIN_HPP_
