# Smart Plant System Control

## Overview

The Smart Plant System Control is a project designed to monitor and control the environment for plant growth. It incorporates temperature and soil moisture sensors to collect data and transmit it to a server using WebSocket communication. The server then relays this information to a web page for user visualization.

## Features

- Real-time monitoring of temperature and soil moisture levels.
- WebSocket communication for efficient and low-latency data transfer.
- Web page interface for users to visualize and control the plant environment.

## Hardware Components

1. **NodeMCU:**
   - The main microcontroller for connecting to sensors and communicating with the server.

2. **DHT11 Sensor:**
   - Measures temperature and humidity.

3. **Soil Moisture Sensor:**
   - Measures soil moisture levels.

## Setup

### Sensor Node

1. Connect the DHT11 and soil moisture sensors to the NodeMCU.
2. Update the configuration file with the appropriate WebSocket server address.

```json
{
  "server_address": "ws://your-server-address:port"
}
