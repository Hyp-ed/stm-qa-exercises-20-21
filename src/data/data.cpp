/*
 * Organisation: HYPED
 * Date:
 * Description:
 *
 *    Copyright 2019 HYPED
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain a
 * copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include "data/data.hpp"

namespace oven {
using hyped::utils::concurrent::ScopedLock;

namespace data {

Data &Data::getInstance() {
  static Data d;
  return d;
}

StateMachine Data::getStateMachineData() {
  ScopedLock L(&lock_state_machine_);
  return state_machine_;
}

void Data::setStateMachineData(const StateMachine &sm_data) {
  ScopedLock L(&lock_state_machine_);
  state_machine_ = sm_data;
}

UserInterface Data::getUserInterfaceData() {
  ScopedLock L(&lock_user_interface_);
  return user_interface_;
}

void Data::setUserInterfaceData(const UserInterface &ui_data) {
  ScopedLock L(&lock_user_interface_);
  user_interface_ = ui_data;
}

Thermometer Data::getThermometerData() {
  ScopedLock L(&lock_thermometer_);
  return thermometer_;
}

void Data::setThermometerData(const Thermometer &tm_data) {
  ScopedLock L(&lock_thermometer_);
  thermometer_ = tm_data;
}

Heating Data::getHeatingData() {
  ScopedLock L(&lock_heating_);
  return heating_;
}

void Data::setHeatingData(const Heating &ht_data) {
  ScopedLock L(&lock_heating_);
  heating_ = ht_data;
}
}  // namespace data

}  // namespace oven
