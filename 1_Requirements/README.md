# REQUIREMENTS 

## INTRODUCTION

  * It is a well-known fact that about 71 percent of the Earth’s surface is covered by water, in which about 97.5 percent is salt water and only 2.5 percent is fresh water. Of the fresh water, only 0.3 percent is in liquid form on the surface, in the forms of underground water, aquifers, fresh water lakes and rivers. 
  * The high use of chemicals in manufacturing, construction, over-use of fertilizers in farms and also directly leaving the effluent water from industries into nearby water bodies have made a huge contribution to global water quality reduction.
  * Urbanisation and growing population have increased the demand for drinking water while the quality of water has been degrading continuously.
  * The WHO says that:
       > contaminated drinking water is estimated to cause 4,85,000 diarrhoeal deaths each year.
  * Continuous water quality monitoring is an important tool for the management authorities to ensure safe drinking water supply to people. 
  * **The proposed water quality monitoring system consisting of basic sensors, microcontrollers, RF modules, is a compact system capable of collecting real-time data and transmitting it via wireless technology through Internet and displaying using a web application and also ensure fail-safe operation using Real-Time Operating System incorporated with the microcontroller.**

## LITERATURE REVIEW:
   * To quantify water quality, we need some parameters, measuring which we can come to a conclusion about the water quality. These parameters can be physical, chemical or biological. 
   * Indian Standard IS 10500:2012 specifies such parameters for checking drinking water’s quality. IS 10500:2012 gives several parameters, mentioning acceptable limits for each parameter and also permissible limits in the absence of alternate sources.
    
  |Parameter|Acceptable limit| 
  |:--:|:--:|
  |Total Dissolved Solids, ppm|500|
  |pH Value|6.5-8.5|
  |Turbidity, NTU|1|
  |Colour, Hazen units|5|
  |Odour| Agreeable|
  |Taste|Agreeable|

# High Level Requirements

|ID|Description|Status|
|:--:|:--:|:--:|
|HR_01|Collect sensor data|Implemented|
|HR_02|Remote Monitoring| Implemented|
|HR_03|Robust System|Implemented|
|HR_04|Modular design|Implemented|


# Low Level Requirements

|ID|Description|Status|
|:--:|:--:|:--:|
|LR_01|Control Action|Implemented|
|LR_02|Analysis of sensor data|Partially Implemented|
|LR_03|Clean interface for website|Implemented|
|LR_04|Email alert|Implemented|
|LR_05|Scalabity|Implemented|
|LR_06|Prognostic function|Future|

# Design of System
![System Design](https://github.com/Vigneshwar73/LTTS_270403_MiniProject/blob/main/1_Requirements/System%20design.jpg)

System design is based on IoT Level-5 Architecture 

<img src="https://github.com/Vigneshwar73/LTTS_270403_MiniProject/blob/main/1_Requirements/iot.png" alt="IOT LEVEL 5" width="500" height="500">


# SWOT Analysis
![SWOT](https://github.com/Vigneshwar73/LTTS_270403_MiniProject/blob/main/1_Requirements/SWOT.png)

# 4W's and 1H

## What:
This system can effectively monitor the quality of water with the help of several sensors and transmit the sensor data over Internet for remote monitoring and also ensure fail-safe operation of the system using RTOS.

## Who:
This system can be used by people who needs to monitor water quality to ensure the water is safe to drink or to utilize for other purposes.

## Where:
This system can be used in places where we need to measure and monitor water quality like water reserviours(lakes, ponds, rivers, aquifers etc.,) and water purification units.

## When:
This system is useful when we need to monitor water quality parameters of multiple water bodies/ reserviours remotely which also shows trends of past data collected and shows visualisations of those data for easy monitoring. 

## How:
This system has two separate nodes, Local node (or End node) and Coordinator Node. Local node consists of the sensors, such as TDS sensor, pH sensor, temperature sensor and flow sensor, which measures the parameters and send the data to Arduino UNO. Arduino UNO is programmed with the help of FreeRTOS, a Real Time Operating System, which adds important multi-tasking and control functionality to Arduino. Arduino is interfaced with XBee module which acts as end node and transmits data wirelessly to XBee coordinator interfaced with ESP8266 microcontroller, which has built-in Wi-Fi to publish data securely to MQTT broker. Web application which we designed will then subscribe to the topic in MQTT broker and displays it in the website. 
