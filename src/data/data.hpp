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

};

// -------------------------------------------------------------------------------------------------
// Thermometer
// -------------------------------------------------------------------------------------------------

struct Thermometer : public Module {
  double current_temp;
  bool critical_failure;
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
struct SensorData {
  bool operational;
};

struct ImuData : public SensorData {
  float acc_x;
};

struct Sensors : public Module {
  static constexpr int kNumImus = 1;
  DataPoint<ImuData> imu;
};

struct BatteryData {
  static constexpr int kNumCells = 36;
  uint16_t  voltage;                    // dV
  int16_t   current;                    // dA
  uint8_t   charge;                     // %
  int8_t    average_temperature;        // C
  // below only for BMSHP! Value for BMSLP = 0
  uint16_t  cell_voltage[kNumCells];    // mV
  int8_t    low_temperature;            // C
  int8_t    high_temperature;           // C
  uint16_t  low_voltage_cell;           // mV
  uint16_t  high_voltage_cell;          // mV
  bool      imd_fault;
};

struct Batteries : public Module {
  static constexpr int kNumBatteries = 3;
  array<BatteryData, kNumBatteries> readings;
  };



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
   * @brief      Retrieves data related to the state machine. Data has high priority.
   */

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
   * @brief Get the Thermometer Data object
   *
   * @return Thermometer
   */
  Thermometer getThermometerData();

  /**
   * @brief Set the Thermometer Data object
   *
   * @param thermo_data
   */
  void setThermometerData(const Thermometer& thermo_data);

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


  StateMachine getStateMachineData();

  Batteries getBatteriesData();

  void setBatteriesData(Batteries& battery);

  /**
   * @brief      Should be called by state machine team to update data.
   */
  void setStateMachineData(const StateMachine& sm_data);

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





 private:
  StateMachine state_machine_;
  Sensors sensors_;
  HeatingElement heating_element_;
  Thermometer thermometer_;
  UserInterface user_interface_;

  // locks for data substructures
  Lock lock_state_machine_;
  Lock lock_sensors_;
  Lock lock_batteries_;
  Lock lock_heating_element_;
  Lock lock_thermometer_;
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
