// declaring libraries that we use in our app
const express = require("express");
const ws = require("ws");
const path = require("path");
const { json } = require("express");
const bodyParser = require("body-parser");
// creating app from express library to create a server
const app = express();

// This is port number
const PORT = process.env.PORT || 3000;

// Declaring variables to know pump,temperature and humidity state
let info = {
  pump: "off",
  temperature: 20,
  humidity: 40,
};

// linking with static path(frontend directory)
app.use(express.static(path.join(__dirname, "./front")));

// This is main page
app.get("/", (req, res) => {
  res.sendFile(__dirname + "/index.html");
});

// create a server from our app var name
const server = app.listen(PORT, () => {
  console.log("Server is running now on port " + PORT);
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

  ws.on("message", (msg) => {
    if (msg.toString().length > 10) {
      const messageFromFront = JSON.parse(msg);
      console.log(messageFromFront);
      info.pump = messageFromFront.value;
      broadcast(JSON.stringify(info));
    } else {
      const message = msg.toString();
      let temp = "";
      let hum = "";

      for (let i = 0; i < 3; i++) {
        temp += msg[i];
      }
      for (let i = 4; i < msg.length; i++) {
        hum += msg[i];
      }
      info.temperature = temp;
      info.humidity = hum;

      console.log(message);
      info.humidity = message;
    }
  });
});

// // sending message to all clients function
function broadcast(msg) {
  wss.clients.forEach((client) => {
    if (client.readyState === client.OPEN) {
      client.send(msg);
    }
  });
}
