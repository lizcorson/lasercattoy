/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <PubSubClient.h>
#include <WiFi101.h>

/************************* Constants *****************************/
#define XPOT A0
#define YPOT A1
#define BUTTON 12
#define LED 13
long lastMsg = 0;
const int msgDelay = 1000; //how often to send data

/************************* WiFI Setup *****************************/
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2     // or, tie EN to VCC

char ssid[] = "221B Baker Street";     //  your network SSID (name)
char pass[] = "perfectcarlos";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

const char* mqtt_server = "192.168.0.221";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

char msg_x[50];
char msg_y[50];
char msg_posn[50];
char msg_button[50];
int value = 0;

void setup() {
  pinMode(LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  WiFi.setPins(WINC_CS, WINC_IRQ, WINC_RST, WINC_EN);
  while (!Serial);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  digitalWrite(LED, HIGH);
}

void setup_wifi() {
  // Initialise the Client
  Serial.print(("\nInit the WiFi module..."));
  // check for the presence of the breakout
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WINC1500 not present");
    // don't continue:
    while (true);
  }
  Serial.println("ATWINC OK!");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(5000);
  }

  // you're connected now, so print out the data:
  digitalWrite(LED, LOW);
  Serial.println("You're connected to the network!");
  printCurrentNet();
  printWifiData();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Joystick")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if ((unsigned long)(millis() - lastMsg) >= msgDelay) {
    lastMsg = millis();

    int xval = analogRead(XPOT);
    int yval = analogRead(YPOT);
    int buttonval = digitalRead(BUTTON);

    String posnString = String(xval) + "," + String(yval) + ",";
    posnString.toCharArray(msg_posn, posnString.length() + 1);
    Serial.print("Publish message: ");
    Serial.println(msg_posn);
    client.publish("joystick/posn", msg_posn);
    
    
//    String x_str = String(xval);
//    x_str.toCharArray(msg_x, x_str.length() + 1);
//    Serial.print("Publish message X: ");
//    Serial.println(msg_x);
//    client.publish("joystick/x", msg_x);
//
//    String y_str = String(yval);
//    y_str.toCharArray(msg_y, y_str.length() + 1);
//    Serial.print("Publish message Y: ");
//    Serial.println(msg_y);
//    client.publish("joystick/y", msg_y);
  }
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
}

