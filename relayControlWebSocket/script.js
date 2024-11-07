let ws = new WebSocket("ws://192.168.100.29"); // Include port

ws.onopen = function () {
  document.getElementById("connectionStatus").innerText = "Connected";
};

ws.onmessage = function (event) {
  document.getElementById("relayState").innerText = event.data;
};

ws.onerror = function (error) {
  console.error("WebSocket Error: ", error);
  document.getElementById("connectionStatus").innerText = "Connection Error";
};

ws.onclose = function () {
  document.getElementById("connectionStatus").innerText = "Disconnected";
};

function sendMessage(message) {
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(message);
  } else {
    alert("WebSocket is not connected.");
  }
}
