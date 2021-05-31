#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "GravityTDS.h"
#define PH_PIN A0
float calibration_value = 22.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;


byte statusLed    = 13;

byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;  

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;
float flowValue;

void TaskSensors( void *pvParameters );
void Tasktds( void *pvParameters );
void TaskPH(void *pvParameters);

// Data wire is plugged into digital pin 4 on the Arduino
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  
SoftwareSerial xbee(8, 9); // RX, TX
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

 
#define TdsSensorPin A1
GravityTDS gravityTds;
 
float temperature = 25,tdsValue = 0, phValue = 6.8, voltage,avgTDS = 0,avgPH = 0;
float tdsArray[10], phArray[5];
float sum = 0, sumPH = 0;
int index = 0, ind = 0;


TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

SemaphoreHandle_t  sema_1;
SemaphoreHandle_t  sema_2; 


void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  xbee.begin(9600);
   
   pinMode(12, OUTPUT);
   
     // Set up the status LED line as an output
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  sensors.begin();  // Start up the library
    
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
    
  xTaskCreate(
    TaskSesnors
    ,  "task1"   
    ,  150  
    ,  NULL
    ,  3  
    ,  &Task1);
  xTaskCreate(
    Tasktds
    ,  "task2"
    ,  80  
    ,  NULL
    ,  1  
    ,  &Task2);
   
  xTaskCreate(
    TaskPH
    ,  "task3"
    ,  80  
    ,  NULL
    ,  2  
    ,  &Task3);
    sema_1 = xSemaphoreCreateBinary();
    sema_2 = xSemaphoreCreateBinary();
    xSemaphoreGive(sema_1);
    //openValve();
  
}
void loop()
{
}

float readpH()
{
   for(int i=0;i<10;i++) 
 { 
    buffer_arr[i]=analogRead(PH_PIN);
    delay(30);
 }
 for(int i=0;i<9;i++)
 {
    for(int j=i+1;j<10;j++)
    {
        if(buffer_arr[i]>buffer_arr[j])
        {
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
        }
    }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 {  
    avgval+=buffer_arr[i];
 }
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;
 return ph_act;
  }  
void TaskSesnors(void *pvParameters)  {
  
  while(1)
  {
        xSemaphoreTake(sema_1,portMAX_DELAY);
    Serial.println("Task 1 running...");
    // Send the command to get temperatures
    sensors.requestTemperatures(); 
    temperature = sensors.getTempCByIndex(0);
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    phValue = readpH(); 
    flowValue = calcFlow();
    Serial.print("TDS Value: ");
    Serial.print(tdsValue,2);
    Serial.println("ppm");
    Serial.print("Temperature: ");
    Serial.print(temperature,2);
    Serial.println("°C ");
    Serial.print("pH: ");
    Serial.println(phValue,2);
    Serial.print("Flow Rate: ");
    Serial.println(flowValue,2);
    
    //xbee.print("<");
    xbee.print(tdsValue);
    xbee.print(',');
    xbee.print(temperature);
    xbee.print(',');
    xbee.print(phValue);
    xbee.print(',');
    xbee.print(flowValue);
    xbee.print('>');

    xSemaphoreGive(sema_1);
      
       
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
    
  }
}
void Tasktds(void *pvParameters)  
{
  
  while(1)
  {
        xSemaphoreTake(sema_2,portMAX_DELAY);
      
        Serial.println("   ....Task 2 running");
        if(index != 5)
         {
          tdsArray[index] += tdsValue;
          index++;
         }
         if(index==5)
         {
          for(int i = 0; i<=index;i++)
          {
            sum +=tdsArray[i];
            }
            avgTDS = sum/10;
            
            if(avgTDS>500)
             {
                Serial.println("Water not potable");
                closeValve();
                }
              else{
                openValve();
              }
              index = 0;
            sum = 0;
            memset(tdsArray, 0, sizeof(tdsArray));
             
          }
          
         
       xSemaphoreGive(sema_2);
       
       
       vTaskDelay( 2000 / portTICK_PERIOD_MS );  
  }
}
void TaskPH(void *pvParameters)
{
  while(1)
  {
    xSemaphoreTake(sema_1,portMAX_DELAY);
    Serial.println("   ....Task 3 running.....");
    if(ind != 5)
     {
      phArray[ind] += phValue;
      ind++;
     }
     if(ind==5)
     {
      for(int i = 0; i<ind;i++)
      {
        sumPH += phArray[i];
        }
        avgPH = sumPH/5;
         if((avgPH < 6.5) || (avgPH > 8.5))
        {
          Serial.println("Water's pH is not suitable for drinking");
          closeValve();        
        }
        else{
          openValve();
        }
        ind = 0;
        sumPH = 0;
        memset(phArray, 0, sizeof(phArray));
      }
     xSemaphoreGive(sema_2);
     vTaskDelay(1);
     xSemaphoreGive(sema_1);
        
       vTaskDelay( 1000 / portTICK_PERIOD_MS );  
  }
  
  }
  /*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
float calcFlow()
{
  if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute
    /*Serial.print("Flow rate: ");
    Serial.print(flowRate);  // Print the integer part of the variable
    Serial.println(" L/min");
    

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");        
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 
    
  Serial.print(totalMilliLitres/1000);
  Serial.println("L");*/
    

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    return flowRate;
  }
}
void closeValve()
{
  if(flowValue!=0)
  {
   digitalWrite(12, LOW);
  }
}
void openValve()
{
  
    digitalWrite(12, HIGH);
  
}
