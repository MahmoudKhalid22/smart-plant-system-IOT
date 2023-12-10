"use strict";
const temp = document.querySelector(".temperature");
const hum = document.querySelector(".humidity");
const soil = document.querySelector(".soilTitle");

// const btn = document.getElementById("btn");
const btn = document.querySelector(".box");
const btnOff = document.querySelector(".off");
const pump = document.querySelector(".pump");
const box = document.querySelector(".main-box");
// const soil = document.querySelector(".soil");
const humBg = document.querySelector(".background");
const soilBg = document.querySelector(".soilBg");

let information = {
  value: "off",
};
const ws = new WebSocket("ws://192.168.1.2:3000");
ws.addEventListener("open", () => {
  console.log("We are connected");
  ws.send(JSON.stringify(information));
});

ws.addEventListener("message", function (event) {
  const info = JSON.parse(event.data);
  console.log(info);
  // TEMPERATURE
  // broadcast(JSON.stringify(info));
  temp.textContent = info.temperature;
  hum.textContent = info.humidity;
  soil.textContent = info.soil;

  humBg.style.height = info.humidity + "%";
  soilBg.style.height = info.soil + "%";

  let color = "hsl(" + (200 - info.humidity * 2) + ", 100%, 50%)";
  let colorSoil = "hsl(" + (30 + info.soil * 2) + ", 100%, 50%)";

  var hue = Math.min(240 + info.temperature * 2, 360); // Adjust the multiplier as needed
  let colorTemp = "hsl(" + hue + ", 100%, 50%)";
  humBg.style.backgroundColor = color;
  soilBg.style.backgroundColor = colorSoil;

  box.style.backgroundColor = colorTemp;

  console.log(info);
  // PUMP
  if (info.pump === "on") {
    pump.textContent = "pump is running now";
  } else {
    pump.textContent = "pump is off now";
  }
});
btn.onclick = function () {
  if (information.value === "off") {
    information.value = "on";
    // console.log(information.value);
    ws.send(JSON.stringify(information));
    ws.onmessage = (e) => {
      console.log(e.data);
      if (e.data === "on") {
        pump.textContent = "pump is running now";
      }
    };
  } else {
    information.value = "off";
    // console.log(information.value);

    ws.send(JSON.stringify(information));
    ws.onmessage = (e) => {
      console.log(e.data);
      if (e.data === "off") {
        pump.textContent = "pump is off now";
      }
    };
  }

  // const turnOff = document.querySelector(".off");
  // turnOff.onclick = function () {
  //
  //   };
};
