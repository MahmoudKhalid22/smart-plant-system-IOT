const express = require("express");
const ws = require("ws");

const app = express();

const PORT = process.env.PORT || 3000;

let pump = 'off';

app.get("/", (req, res) => {
  res.sendFile(__dirname + "/index.html");
});

const server = app.listen(PORT, () => {
  console.log("Server is running now");
});

const socketServer = ws.Server;
const wss = new socketServer({ server });

wss.on("connection", (ws) => {
  console.log("New Client Connected");
  ws.on("message", (msg) => {
    const message = JSON.parse(msg);
    console.log(message);
    
    if (message.value === 'on') {
        pump = 'on';
        broadcast(pump);
      }
     else{
      pump = 'off';
      broadcast(pump);
     } 
  });
});


function broadcast(msg) {
    wss.clients.forEach(client =>{
      if(client.readyState === client.OPEN){
        client.send(msg);
      }
    })
}