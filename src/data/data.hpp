/*
 * Organisation: HYPED
 * Date: 2020-10-10
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


// -------------------------------------------------------------------------------------------------
// State Machine States
// -------------------------------------------------------------------------------------------------
enum State {
  num_states
};

extern const char* states[num_states];

struct StateMachine {
  bool critical_failure;
  State current_state;
};

//
// Telemetry
//

struct Telemetry {
  bool critical_failure;
  bool is_on;
  uint32_t target_temperature;
}

//
// Temperature
//
struct Sensors {
  bool critical_failure;
  double temperature;
}

// Motor
//
struct Motor{
  bool critical_failure;
  float rate;
}

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
   * @brief      Retrieves data related to the state machine. Data has high priority.
   */
  StateMachine getStateMachineData();

  /**
   * @brief      Should be called by state machine team to update data.
   */
  void setStateMachineData(const StateMachine& sm_data);

  /**
   * @brief      Retrieves data produced by navigation sub-team.
   */
  Navigation getNavigationData();

  /**
   * @brief      Should be called by navigation sub-team whenever they have new data.
   */
  void setNavigationData(const Navigation& nav_data);

  /**
   * @brief      Retrieves data from all sensors
   */
  Sensors getSensorsData();

  /**
   * @brief retrieves imu data from Sensors
   */
  DataPoint<ImuData> getSensorsImuData();

  /**
   * @brief      Should be called to update sensor data.
   */
  void setSensorsData(const Sensors& sensors_data);
  /**
   * @brief      Should be called to update sensor imu data.
   */
  void setSensorsImuData(const DataPoint<ImuData>& imu);

  /**
   * @brief      Retrieves data from the batteries.
   */
  Batteries getBatteriesData();

  /**
   * @brief      Should be called to update battery data
   */
  void setBatteriesData(const Batteries& batteries_data);

  /**
   * @brief      Retrieves data from the brakes.
   */
  Brakes getBrakesData();

  /**
   * @brief      Should be called to update brakes data
   */
  void setBrakesData(const Brakes& brakes_data);

  /**
   * @brief      Retrieves data produced by each of the four motors.
   */
  Motors getMotorData();

  /**
   * @brief      Should be called to update motor data.
   */
  void setMotorData(const Motors& motor_data);

  /**
   * @brief      Retrieves data on whether stop/kill power commands have been issued.
   */
  Telemetry getTelemetryData();

  /**
   * @brief      Should be called to update communications data.
   */
  void setTelemetryData(const Telemetry& telemetry_data);

 private:
  StateMachine state_machine_;
  Sensors sensors_;
  Motors motors_;
  Telemetry telemetry_;


  // locks for data substructures
  Lock lock_state_machine_;
  Lock lock_navigation_;
  Lock lock_sensors_;
  Lock lock_motors_;

  Lock lock_telemetry_;
  Lock lock_batteries_;
  Lock lock_brakes_;

  Data() {}

 public:
  Data(const Data&) = delete;
  Data& operator=(const Data &) = delete;
  Data(Data &&) = delete;
  Data & operator=(Data &&) = delete;
};

}}  // namespace hyped::data

#endif  // DATA_DATA_HPP_