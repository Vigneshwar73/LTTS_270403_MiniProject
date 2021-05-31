#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
String myString, l,m,n,o;
float tdsValue, temperature, pH, flow;
char rdata;
SoftwareSerial xbee (D1, D2); //Rx, Tx

String api_key = "82AH367TQBXF8F8F";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "Karunanithi";     // replace with your wifi ssid and wpa2 key
const char *password =  "9443470772";
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup() {
    Serial.begin(9600);// put your setup code here, to run once:
    xbee.begin(9600);
     WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
}

void loop() {
  while(xbee.available()>0)
  {
    
    rdata = xbee.read();
    if(rdata=='>'){break;}
    else{myString = myString + rdata;}
   
  }
    if(rdata=='>')
    {
      parseData();
      send_thingspeak();
      showData();
      
      }
}

void parseData()
{   
      l = getValue(myString, ',', 0);
      m = getValue(myString, ',', 1);
      n = getValue(myString, ',', 2);
      o = getValue(myString, ',', 3);

      tdsValue = l.toFloat();
      temperature = m.toFloat();
      pH = n.toFloat();
      flow = o.toFloat();

      myString = "";
      rdata = 0;
}
void showData()
{
  Serial.print("The TDS Value is: ");
  Serial.print(tdsValue);
  Serial.println("ppm");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C ");
  Serial.print("pH: ");
  Serial.println(pH);
  Serial.print("FlowRate: ");
  Serial.println(flow);
  Serial.println("=================");
  }
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
  void send_thingspeak()
  {           
              if (isnan(tdsValue) || isnan(temperature) || isnan(pH)) 
                 {
                     Serial.println("Failed to read from sensor!");
                      return;
                 }

               if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
            {  
                   String data_to_send = api_key;
                    data_to_send += "&field1=";
                    data_to_send += temperature;
                    data_to_send += "&field2=";
                    data_to_send += tdsValue;
                    data_to_send += "&field3=";
                    data_to_send += pH;
                    data_to_send += "&field4=";
                    data_to_send += flow;
                    data_to_send += "\r\n\r\n";

                   client.print("POST /update HTTP/1.1\n");
                   client.print("Host: api.thingspeak.com\n");
                   client.print("Connection: close\n");
                   client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
                   client.print("Content-Type: application/x-www-form-urlencoded\n");
                   client.print("Content-Length: ");
                   client.print(data_to_send.length());
                   client.print("\n\n");
                   client.print(data_to_send);
                   delay(1000);
                   Serial.print("Temperature: ");
                   Serial.print(temperature);
                   Serial.println("°C ");
                   Serial.print("TDS Value: ");
                   Serial.print(tdsValue);
                   Serial.println("ppm.");
                   Serial.print("pH Value: ");
                   Serial.println(pH);
                   Serial.print("Flowrate: ");
                   Serial.println(flow);
                   Serial.println("Sent to Thingspeak.");
                   Serial.println("===================");
              }
          client.stop();
 
          Serial.println("Waiting...");
  
  
    
    }
