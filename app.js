// declaring libraries that we use in our app
const express = require("express");
const ws = require("ws");
const path = require("path");
const { json } = require("express");

// creating app from express library to create a server
const app = express();

// This is port number
const PORT = process.env.PORT || 3000;

// Declaring variables to know pump,temperature and humidity state
let info = {
  pump: "off",
  temperature: 10,
  humidity: 40,
};

let prevTemperature = info.temperature;
let prevHumidity = info.humidity;
// linking with static path(frontend directory)
app.use(express.static(path.join(__dirname, "./front")));

// This is main page
app.get("/", (req, res) => {
  res.sendFile(__dirname + "/index.html");
});

// create a server from our app var name
const server = app.listen(PORT, () => {
  console.log("Server is running now");
});

// declaring our socketServer from ws library to connect between client and sever
const socketServer = ws.Server;
const wss = new socketServer({ server });

// When client open and server open that they are connected

wss.on("connection", (ws) => {
  console.log("New Client Connected");
  // When there is messege from client that server do actions
  const information = JSON.stringify(info);
  broadcast(information);
  console.log(info);
  setInterval(function () {
    if (prevTemperature !== info.temperature) {
      broadcast(`Temperature ${info.temperature}`);
    }
    if (prevHumidity !== info.humidity) {
      broadcast(`Humidity ${info.humidity}`);
    }
  }, 500);

  ws.on("message", (msg) => {
    const message = JSON.parse(msg);
    console.log(`pump is now ${message.value}`);
    // console.log(`temperature is ${}`);
    if (message.value === "on") {
      info.pump = "on";
      broadcast(info.pump);
    } else {
      info.pump = "off";
      broadcast(info.pump);
    }
  });
});

// sending message to all clients function
function broadcast(msg) {
  wss.clients.forEach((client) => {
    if (client.readyState === client.OPEN) {
      client.send(msg);
    }
  });
}
