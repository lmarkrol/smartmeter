
#include <PZEM004Tv30.h>

#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

//DHT11
//#include "DHT.h"
//#define DHTPIN 15
//#define DHTTYPE DHT11
//DHT dht(DHTPIN, DHTTYPE);

//LED
String fireStatus = "";   // led status received from firebase
int led = LED_BUILTIN; 

//PZEM
#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#endif

#if !defined(PZEM_SERIAL)
#define PZEM_SERIAL Serial2
#endif


#if defined(ESP32)
/*************************
 *  ESP32 initialization
 * ---------------------
 * 
 * The ESP32 HW Serial interface can be routed to any GPIO pin 
 * Here we initialize the PZEM on Serial2 with RX/TX pins 16 and 17
 */
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
#elif defined(ESP8266)
/*************************
 *  ESP8266 initialization
 * ---------------------
 * 
 * Not all Arduino boards come with multiple HW Serial ports.
 * Serial2 is for example available on the Arduino MEGA 2560 but not Arduino Uno!
 * The ESP32 HW Serial interface can be routed to any GPIO pin 
 * Here we initialize the PZEM on Serial2 with default pins
 */
//PZEM004Tv30 pzem(Serial1);
#else
/*************************
 *  Arduino initialization
 * ---------------------
 * 
 * Not all Arduino boards come with multiple HW Serial ports.
 * Serial2 is for example available on the Arduino MEGA 2560 but not Arduino Uno!
 * The ESP32 HW Serial interface can be routed to any GPIO pin 
 * Here we initialize the PZEM on Serial2 with default pins
 */
PZEM004Tv30 pzem(PZEM_SERIAL);
#endif


// Set these to run example.
#define FIREBASE_HOST "host"
#define FIREBASE_AUTH "secret"
#define WIFI_SSID "id"
#define WIFI_PASSWORD "pw"



void setup() {
    // Debugging Serial port
    Serial.begin(115200);
    // Uncomment in order to reset the internal energy counter
    // pzem.resetEnergy()

    // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    //Initial Firebase
//    Firebase.setString("LED", "0"); 
//    Firebase.setString("Temperature", ""); 
//    Firebase.setString("Humidity", ""); 
    
    // DHT11 sensor begin
//    Serial.println("DHT11 test!");
//    dht.begin();

    //LED initialization
    pinMode(LED_BUILTIN, OUTPUT);


}

void loop() {
    // Print the custom address of the PZEM
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {

        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);

    }


    Serial.println();
//    delay(2000);


// set value
  Firebase.setFloat("(R)-Voltage", voltage);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting/number Voltage failed:");
      Serial.println(Firebase.error());  
      return;
  }
//  Serial.println ("Data has been send to Firebase!");
  delay(1000);

  Firebase.setFloat("(R)-Current", current);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting/number Current failed:");
      Serial.println(Firebase.error());  
      return;
  }
//  Serial.println ("Data has been send to Firebase!");
  delay(1000);
  
  Firebase.setFloat("(R)-Power", power);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting/number Power failed:");
      Serial.println(Firebase.error());  
      return;
  }
//  Serial.println ("Data has been send to Firebase!");
  delay(1000);
    
  Firebase.setFloat("(R)-Energy", energy);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting/number Energy failed:");
      Serial.println(Firebase.error());  
      return;
  }
//  Serial.println ("Data has been send to Firebase!");
  delay(1000);
    
  Firebase.setFloat("(R)-Frequency", frequency);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting/number Frequency failed:");
      Serial.println(Firebase.error());  
      return;
  }
//  Serial.println ("Data has been send to Firebase!");
  delay(1000);

  Firebase.setFloat("(R)-PowerFactor", pf);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting/number PowerFactor failed:");
      Serial.println(Firebase.error());  
      return;
  }
//  Serial.println ("Data has been send to Firebase!");
  delay(1000);


// DHT 11 sensor
//reading temperature & humidity
//  delay(2000);
//  float h = dht.readHumidity();
//  float t = dht.readTemperature();
//    if (isnan(h) || isnan(t)) {
//    Serial.println("Failed to read from DHT sensor!");
//    return;
//  }
//  Serial.print("Humidity: ");
//  Serial.print(h);
//  Serial.print(" %\t");
//  Serial.print("Temperature: ");
//  Serial.print(t);
//  Serial.println(" *C ");
  
//  // set value
//  fireStatus = Firebase.getString("Temperature");
//  Firebase.setFloat("Temperature", t);
//  // handle error
//  if (Firebase.failed()) {
//      Serial.print("setting /number failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
//  delay(1000);
//
//  fireStatus = Firebase.getString("Humidity");
//  Firebase.setFloat("Humidity", h);
//  // handle error
//  if (Firebase.failed()) {
//      Serial.print("setting /number failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
////  Serial.println ("DHT Sensor data has been send");
//  delay(1000);


//LED
  fireStatus = Firebase.getString("(R)--LEDstatus"); // get led status input from firebase
  if (fireStatus == "1") {               // compare the input of led status received from firebase
    Serial.println("Led Turned ON");                 
    digitalWrite(LED_BUILTIN, HIGH);      // make output led ON
  }
  else if (fireStatus == "0") {         // compare the input of led status received from firebase
    Serial.println("Led Turned OFF");
    digitalWrite(LED_BUILTIN, LOW);       // make output led OFF
  }
  else {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }  

  Serial.println ("Data has been send to Firebase!");
  delay(1000);

} //end of void loop
