# TEST PLAN AND TEST OUTPUT

## UNIT TEST - (LOW LEVEL TEST)

  * Each individual hardware component to be tested and calibrated. 
  * Software side program codes are tested for functionality.

|Component|Test Type|Test conducted|Result|
|:--:|:--:|:--:|:--:|
|Arduino UNO|Requirement based|sample coded uploaded|Passed|
|ESP8266|Requirement based|sample code to connect to Wi-Fi| Passed|
|XBee modules|Requirement based|establish communication between XBee modules|Passed|
|TDS sensor|Scenario based|tested with different water samples with varying TDS|Passed|
|TDS sensor|Scenario based|added salt gradually to RO water|Passed|
|pH sensor|Scenario based|tested with solutions of known pH values|Passed|
|Temperarture sensor|Scenario based|tested for boundary conditions|Passed|
|Flow sensor|Scenario based|tested by varying flow rate of water|Passed|
|Solenoid valve|Requirement based|valve opening and closing when given control signal|Passed|
|Data logging in MQTT|Requirement based|data published to MQTT broker|Passed|
|Data Analysis and Visualization|Requirement based|custom functions implemented|Passed|
|Website|Requirement based|subscribe and display live data|Passed|



## INTEGRATION TEST - (HIGH LEVEL TEST)

   * The combined working of sensors, microcontrollers and web components are tested

|Component|Test Type|Test conducted|Result|
|:--:|:--:|:--:|:--:|
|Local Node|Requirement based|Combined working of components - Data collection and Transmission|Passed|
|Coordinator Node|Requirement based|Combined working of components - Data receiving and publishing|Passed|
|Web server side|Requirement based|Datalogging, visualization, analysis and display in website|Passed|

REMOTE WATER QUALITY MONITORING SYSTEM - remote monitoring of live sensor data - PASSED
