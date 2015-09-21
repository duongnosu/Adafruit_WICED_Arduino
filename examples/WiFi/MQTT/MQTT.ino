/*
  MQTT Example:
  1. Connect to pre-specified AP
  2. Set the LastWill message
  3. Connect to MQTT Broker
  4. Publish "Alive" message every 10 seconds

  author: huynguyen 
 */

#include "adafruit_wifi.h"

uint16_t wifi_error, mqtt_error;

/**************************************************************************/
/*!
    @brief  Connect to pre-specified AP
    
    @return Error code  
*/
/**************************************************************************/
uint16_t connectAP()
{
  // Connect to an AP
  char* ssid = "huy-laptop";
  char* pass = "12345678";
  uint16_t error = wiced.connectAP(ssid, pass);
  if (error == ERROR_NONE)
  {
    Serial.print("Connected to AP with SSID = ");
    Serial.print(ssid);
    Serial.print(" and PASSWORD = ");
    Serial.println(pass);
  }
  else
  {
    Serial.println("Connect AP Error!");
  }
  Serial.println("");
  return error;
}

/**************************************************************************/
/*!
    @brief  Connect to pre-specified Broker

    @return Error code
*/
/**************************************************************************/
uint16_t connectBroker()
{
  char* host = "test.mosquitto.org";
  char* clientID = "adafruit";
  uint16_t port = 1883;
  uint16_t error = wiced.mqttConnect(host, port, clientID);
  if (error == ERROR_NONE)
  {
    Serial.print("Connected to Broker = ");
    Serial.println(host);
  }
  else
  {
    Serial.println("Connect Broker Error!");
  }
  Serial.println("");
  return error;
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board    
*/
/**************************************************************************/
void setup()
{
  pinMode(BOARD_LED_PIN, OUTPUT);
  delay(1000);

  wifi_error = connectAP();

  if (wifi_error == ERROR_NONE)
  {
    // Set LastWill message
    if (wiced.mqttLastWill("topic/adafruit","Offline",1,0) == ERROR_NONE)
    {
      Serial.println("Specified LastWill Message!");
    }
    
    // Connect to MQTT server (broker)
    mqtt_error = connectBroker();
  }

  // initialize serial port for input and output
//  Serial.begin(11500);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop() {
  if (wifi_error == ERROR_NONE)
  {
    if (mqtt_error == ERROR_NONE)
    {
      // Publish a Message to a Topic
      char* topic = "topic/adafruit";
      char* value = "Alive";
      // qos = 1, retain = 0
      if (wiced.mqttPublish(topic, value, 1, 0) == ERROR_NONE)
      {
        Serial.print("Published Message! ");
        Serial.print("Value = ");
        Serial.println(value);
      }
    }
    else
    {
      // Try to reconnect to MQTT Server    
      Serial.println("Trying to reconnect to MQTT Server...");
      mqtt_error = connectBroker();
    }
  }
  else
  {
    // Try to reconnect to AP
    Serial.println("Trying to reconnect to AP...");
    wifi_error = connectAP();
  }
  
  Serial.println("");
  togglePin(BOARD_LED_PIN);
  delay(10000);   // Delay 10 seconds before next loop
}
