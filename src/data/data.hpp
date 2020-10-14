/*
 * Organisation: HYPED
 * Date: 2020-10-10
 * Description: Class for data exchange between sub-team threads and structures
 * for holding data produced by each of the sub-teams.
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

#ifndef DATA_DATA_HPP_
#define DATA_DATA_HPP_

#include "data/data_point.hpp"
#include "utils/concurrent/lock.hpp"

namespace oven {

// imports
using hyped::utils::concurrent::Lock;

namespace data {

// -------------------------------------------------------------------------------------------------
// Global Module States
// -------------------------------------------------------------------------------------------------
enum class ModuleStatus {
  kStart,  // Initial module state
  kInit,   // SM transistions to Calibrating if all modules have Init status.
  kReady,  // SM transistions to Ready if Motors and Navigation have the Ready
           // status.
  kCriticalFailure  // SM transitions to EmergencyBraking/FailureStopped
};

// -------------------------------------------------------------------------------------------------
// State Machine States
// -------------------------------------------------------------------------------------------------
enum State { 
  kOff,
  kHeating,
  kIdling,
  num_states 
  };

struct StateMachine {
  bool critical_failure;
  State current_state;
};

// -------------------------------------------------------------------------------------------------
// Telemetry
// -------------------------------------------------------------------------------------------------

struct UserInterface {
  bool critical_failure;
  bool is_on;
  uint32_t target_temperature;
};

// -------------------------------------------------------------------------------------------------
// Temperature
// -------------------------------------------------------------------------------------------------

struct Thermometer {
  bool critical_failure;
  double temperature;
};

// -------------------------------------------------------------------------------------------------
// Motor
// -------------------------------------------------------------------------------------------------

struct Heating {
  bool critical_failure;
  float rate;
};

// -------------------------------------------------------------------------------------------------
// Common Data structure/class
// -------------------------------------------------------------------------------------------------
/**
 * @brief      A singleton class managing the data exchange between sub-team
 * threads.
 */
class Data {
 public:
  /**
   * @brief   Always returns a reference to the only instance of `Data`.
   */
  static Data &getInstance();

  /**
   * @brief   Retrieves data related to the state machine.
   */
  StateMachine getStateMachineData();

  /**
   * @brief   Should be called by the state machine team to update data.
   */
  void setStateMachineData(const StateMachine &sm_data);

  /**
   * @brief   Retrieves data related to the user interface.
   */
  UserInterface getUserInterfaceData();

  /**
   * @brief   Should be called by the user interface team to update data.
   */
  void setUserInterfaceData(const UserInterface &ui_data);

  /**
   * @brief   Retrieves data related to the thermometer.
   */
  Thermometer getThermometerData();

  /**
   * @brief   Should be called by the thermometer team to update data.
   */
  void setThermometerData(const Thermometer &tm_data);

  /**
   * @brief   Retrieves data related to the heating controller.
   */
  Heating getHeatingData();

  /**
   * @brief   Should be called by the heating controller team to update data.
   */
  void setHeatingData(const Heating &ht_data);

 private:
  StateMachine state_machine_;
  UserInterface user_interface_;
  Thermometer thermometer_;
  Heating heating_;

  // locks for data substructures
  Lock lock_state_machine_;
  Lock lock_user_interface_;
  Lock lock_thermometer_;
  Lock lock_heating_;

  Data() {
  }

 public:
  Data(const Data &) = delete;
  Data &operator=(const Data &) = delete;
  Data(Data &&)                 = delete;
  Data &operator=(Data &&) = delete;
};

}  // namespace data
}  // namespace oven

#endif  // DATA_DATA_HPP_