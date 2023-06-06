global.WebSocket = require('ws'); //remove if deploying on web browser

// const Sockette = require('sockette');

// const client = new Sockette('ws://172.20.10.5/ws', {
//   timeout: 5e3,
//   maxAttempts: 10,
//   onopen: message => console.log('Connection established!', message),
//   onmessage: message => console.log("Current time on server is: '" + message.data + "'"),
//   onreconnect: message => console.log('Reconnecting...', e),
//   onmaximum: message => console.log('Connection failed!', e),
//   onclose: message => console.log('Connection closed!', message.code, message.reason),
//   onerror: error => console.log("Connection error: " + error.toString())
// });

// client.open();

// // client.send('Tell me the time!');
// client.json({type: 'ping'});
// // client.close(); // graceful shutdown




// // Reconnect 10s later
// // setTimeout(ws.reconnect, 10e3);

var gateway = `ws://172.20.10.5/ws`;
  var websocket;
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }

  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000); //handle disconnection
  }
  function onMessage(event) {
    console.log(event.data);
    websocket.send('turn left!');
  }

initWebSocket();

setInterval(()=>{
  const start = Date.now();

  websocket.send("PING",()=>{
    const duration = Date.now() - start;
    console.log (duration);
  });
}, 1000);

