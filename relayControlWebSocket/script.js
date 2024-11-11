let ws = new WebSocket("ws://192.168.100.29"); // Include port

// When the WebSocket connection opens
ws.onopen = function () {
  document.getElementById("connectionStatus").innerText = "Connected";
};

// When a message is received from the WebSocket server
ws.onmessage = function (event) {
  document.getElementById("relayState").innerText = event.data;
};

// If an error occurs with the WebSocket connection
ws.onerror = function (error) {
  console.error("WebSocket Error: ", error);
  document.getElementById("connectionStatus").innerText = "Connection Error";
};

// When the WebSocket connection is closed
ws.onclose = function () {
  document.getElementById("connectionStatus").innerText = "Disconnected";
};

// Function to send messages to the WebSocket server
function sendMessage(message) {
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(message); // Send the message to the ESP32 WebSocket server
  } else {
    alert("WebSocket is not connected.");
  }
}
