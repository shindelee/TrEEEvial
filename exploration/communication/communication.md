# ESP32 - Local Host Communication

In this communication, ESP32 will act as the server and the host, running the exploration algorithm will be the client.

Client asks rover to move forward, gets a response from rover of new coordinates + wall information.

## Contents

The "client" folder contains code for the client-side code, to be loaded onto the host.

The "server" folder contains the Arduino code to be loaded onto the ESP32.

## Software Dependencies

On client side,

```
npm install socket.io --save
```

This is already listed in package.json
