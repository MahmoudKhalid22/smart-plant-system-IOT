"use strict";
const temp = document.querySelector(".temperature");
const hum = document.querySelector(".humidity");
// const btn = document.getElementById("btn");
const btn = document.querySelector(".box");
const btnOff = document.querySelector(".off");
const pump = document.querySelector(".pump");

const information = {
  value: "off",
};
const ws = new WebSocket("ws://localhost:3000");
ws.addEventListener("open", () => {
  console.log("We are connected");
  //ws.send(JSON.stringify(information));
});

ws.addEventListener("message", function (event) {
  const info = JSON.parse(event.data);
  console.log(info);
  // TEMPERATURE
  // broadcast(JSON.stringify(info));
  temp.textContent = info.temperature;
  hum.textContent = info.humidity;
  // setInterval(function() {
  //   if(prevTemperature !== info.temperature){
  //     broadcast(`Temperature ${info.temperature}`);
  //   }
  // if(prevHumidity !== info.humidity){
  // broadcast(`Humidity ${info.humidity}`);
  //   }
  // }, 500);
  // PUMP
  if (info.pump === "on") {
    pump.textContent = "pump is running now";
  } else {
    pump.textContent = "pump is off now";
  }
});
btn.onclick = function () {
  if (information.value === "on") {
    information.value = "off";
    // console.log(information.value);
    // ws.send(JSON.stringify(information));
    // ws.onmessage = (e) => {
    //   console.log(e.data);
    //   if (e.data === "on") {
    //     pump.textContent = "pump is running now";
    //   }
    // };
  } else {
    information.value = "on";
    // console.log(information.value);

    // ws.send(JSON.stringify(information));
    // ws.onmessage = (e) => {
    //   console.log(e.data);
    //   if (e.data === "off") {
    //     pump.textContent = "pump is off now";
    //   }
    // };
  }
  ws.send(JSON.stringify(information));
  // const turnOff = document.querySelector(".off");
  // turnOff.onclick = function () {
  //
  //   };
};
