 /*
 * Organisation: HYPED
 * Date:
 * Description: Class for data exchange between sub-team threads and structures
 * for holding data produced by each of the sub-teams.
 *
 *    Copyright 2019 HYPED
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


#ifndef DATA_DATA_HPP_
#define DATA_DATA_HPP_

#include <cstdint>
#include <array>
#include "data/data_point.hpp"
#include "utils/concurrent/lock.hpp"

using std::array;

namespace hyped {

// imports
using utils::concurrent::Lock;

namespace data {

// -------------------------------------------------------------------------------------------------
// Global Module States
// -------------------------------------------------------------------------------------------------
enum class ModuleStatus {
  kStart,   // Initial module state
  kInit,  // SM transistions to Calibrating if all modules have Init status.
  kReady,  // SM transistions to Ready if Motors and Navigation have the Ready status.
  kCriticalFailure  // SM transitions to EmergencyBraking/FailureStopped
};

struct Module {
  ModuleStatus module_status = ModuleStatus::kStart;
};

// -------------------------------------------------------------------------------------------------
// Heating Element Controller
// -------------------------------------------------------------------------------------------------

struct HeatingElement : public Module {
  float heat_increment;
  bool is_heating;
  bool critical_failure;
  double current_temp;

};

// -------------------------------------------------------------------------------------------------
// User Interface
// -------------------------------------------------------------------------------------------------

struct UserInterface {
  bool is_on;
  bool critical_failure;
  uint16_t target_temp;
};

// -------------------------------------------------------------------------------------------------
// Raw Sensor data
// -------------------------------------------------------------------------------------------------








// -------------------------------------------------------------------------------------------------
// State Machine States
// -------------------------------------------------------------------------------------------------
enum State {
  kOff,
  kHeating,
  kIdling,
  kEmergency,
  num_states

};

extern const char* states[num_states];

struct StateMachine {
  bool critical_failure;
  State current_state;
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
   * @brief      Always returns a reference to the only instance of `Data`.
   */
  static Data& getInstance();


  /**
   * @brief      Retrieves Heating data
   * @returns    Heating Data
  */
  HeatingElement getHeatingElementData();

  /**
   *@brief       Should be called to update Heating data
  */
  void setHeatingElementData (const HeatingElement& heating_data);

  /**
   * @brief Get the User Interface Data object
   *
   * @return UserInterface
   */
  UserInterface getUserInterfaceData();

  /**
   * @brief Set the User Interface Data object
   *
   * @param ui_data
   */
  void setUserInterfaceData(const UserInterface& ui_data);

  /**
   * @brief      Should be called by state machine team to get data.
   */
  StateMachine getStateMachineData();


  /**
   * @brief      Should be called by state machine team to update data.
   */
  void setStateMachineData(const StateMachine& sm_data);






 private:
  StateMachine state_machine_;
  HeatingElement heating_element_;
  UserInterface user_interface_;

  // locks for data substructures
  Lock lock_state_machine_;
  Lock lock_heating_element_;
  Lock lock_user_interface_;


  Data() {}

 public:
  Data(const Data&) = delete;
  Data& operator=(const Data &) = delete;
  Data(Data &&) = delete;
  Data & operator=(Data &&) = delete;
};

}}  // namespace hyped::data

#endif  // DATA_DATA_HPP_
