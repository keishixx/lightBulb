#include <WebSocketsServer.h>
#include <WiFi.h>

const char *ssid = "HUAWEI-2.4G-Ne85";         // Replace with your network SSID
const char *password = "BWJ3ZsZb";             // Replace with your network password

const int relay1 = 25;       // Relay 1 connected to GPIO 27
const int relay2 = 26;       // Relay 2 connected to GPIO 26
const int relay3 = 27;       // Relay 3 connected to GPIO 25

const int buttonPin1 = 32;   // Button 1 connected to GPIO 34
const int buttonPin2 = 35;   // Button 2 connected to GPIO 35
const int buttonPin3 = 34;   // Button 3 connected to GPIO 32

WebSocketsServer webSocket(81); // Create WebSocket server on port 81

bool relayStates[3] = {LOW, LOW, LOW};    // Array to store each relay state
bool buttonPressed[3] = {false, false, false}; // Array to track button states

void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  
  digitalWrite(relay1, relayStates[0]);
  digitalWrite(relay2, relayStates[1]);
  digitalWrite(relay3, relayStates[2]);

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();  // Start the WebSocket server
  webSocket.onEvent(webSocketEvent); // Attach event handler
}

void loop() {
  handleButtonPress(0, buttonPin1, relay1); // Check button 1 and control relay 1
  handleButtonPress(1, buttonPin2, relay2); // Check button 2 and control relay 2
  handleButtonPress(2, buttonPin3, relay3); // Check button 3 and control relay 3

  webSocket.loop(); // Process WebSocket events
  delay(100); // Reduced delay for responsiveness
}

// Function to handle button press and toggle the corresponding relay
void handleButtonPress(int index, int buttonPin, int relayPin) {
  int buttonState = digitalRead(buttonPin); // Read button state
  
  // Toggle relay state when button is pressed
  if (buttonState == LOW && !buttonPressed[index]) { // Button is pressed (LOW because of pull-up)
    relayStates[index] = !relayStates[index];         // Toggle relay state
    digitalWrite(relayPin, relayStates[index]);       // Update relay with new state
    buttonPressed[index] = true;                      // Mark button as pressed
    Serial.print("Relay ");
    Serial.print(index + 1);
    Serial.println(relayStates[index] ? " ON - Current Flowing (Button Pressed)" : " OFF - Current not Flowing (Button Released)");
    sendRelayStates(); // Send the new relay states to all connected clients
  } else if (buttonState == HIGH && buttonPressed[index]) {
    buttonPressed[index] = false; // Reset buttonPressed when button is released
  }
}

// Function to handle WebSocket events
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  String message = String((char*)payload).substring(0, length);
  
  // Check the received message and control relays accordingly
  if (message == "ON1") {
    relayStates[0] = HIGH;
    digitalWrite(relay1, relayStates[0]);
    Serial.println("Relay 1 ON - Current Flowing (WebSocket Control)");
  } else if (message == "OFF1") {
    relayStates[0] = LOW;
    digitalWrite(relay1, relayStates[0]);
    Serial.println("Relay 1 OFF - Current not Flowing (WebSocket Control)");
  } else if (message == "ON2") {
    relayStates[1] = HIGH;
    digitalWrite(relay2, relayStates[1]);
    Serial.println("Relay 2 ON - Current Flowing (WebSocket Control)");
  } else if (message == "OFF2") {
    relayStates[1] = LOW;
    digitalWrite(relay2, relayStates[1]);
    Serial.println("Relay 2 OFF - Current not Flowing (WebSocket Control)");
  } else if (message == "ON3") {
    relayStates[2] = HIGH;
    digitalWrite(relay3, relayStates[2]);
    Serial.println("Relay 3 ON - Current Flowing (WebSocket Control)");
  } else if (message == "OFF3") {
    relayStates[2] = LOW;
    digitalWrite(relay3, relayStates[2]);
    Serial.println("Relay 3 OFF - Current not Flowing (WebSocket Control)");
  }

  // Send the current relay states back to the client
  sendRelayStates();
}

// Function to send the current relay states to all clients
void sendRelayStates() {
  String stateMessage = String(relayStates[0] ? "ON1" : "OFF1") + "," +
                        String(relayStates[1] ? "ON2" : "OFF2") + "," +
                        String(relayStates[2] ? "ON3" : "OFF3");
  webSocket.broadcastTXT(stateMessage); // Send the state to all connected clients
}
