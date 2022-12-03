#include <ESP8266WiFi.h> 
#include <Adafruit_Fingerprint.h>
#include <Servo.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

#define FIREBASE_HOST "nodeatt-2225b-default-rtdb.firebaseio.com/" // realtime database url
#define FIREBASE_AUTH "dGMPr6Z9T6jf1Ms2D5J8GGbQ29HXdLDC0NLjkGqN" // database key
const long utcOffsetInSeconds = 20700; // UTC time of Nepal in seconds
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; //list of days of the week
SoftwareSerial mySerial(13, 15);
// 13 --> D7 
// 15 --> D8
const int b = 5; //buzzer
const char* ssid = "Cench";
const char* password = "aaaaaaaa";
const char* host = "maker.ifttt.com"; 

#else
 
#define mySerial Serial1

#endif
Servo servo;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
void setup()
{
  Serial.begin(9600);
  pinMode(b, OUTPUT);
  servo.attach(4);
  servo.write(0);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    digitalWrite(b, HIGH);
    delay(1000);
    digitalWrite(b, LOW); 
    delay(3000);
  }
  timeClient.begin();
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}

void loop()                     // run over and over again
{
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    WiFiClient client ;
  const int httpPort = 80;
  if (!client.connect(host, httpPort )) {
    Serial.println("connection failed");
    delay(1000);
  
  }
 
    String url = "/trigger/Unknown/with/key/llx2jUpzA84qJD8hIH3nFOXl-_U2XMDoorZcDWQ9sFj";
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host +  "\r\n" + 
                              "Connection: close\r\n\r\n");
 
    digitalWrite(b, HIGH); 
    delay(1000);
    digitalWrite(b, LOW); 
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
      WiFiClient client ;
  const int httpPort = 80;
  if (!client.connect(host, httpPort )) {
    Serial.println("connection failed");
    delay(1000);
  }
 
    String url = "/trigger/Shreeti/with/key/llx2jUpzA84qJD8hIH3nFOXl-_U2XMDoorZcDWQ9sFj";
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host +  "\r\n" + 
                              "Connection: close\r\n\r\n");
  servo.write(180);
  delay(10000);
  servo.write(0);
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  servo.write(90);
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
