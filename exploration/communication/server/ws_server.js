// const WebSocket = require('ws');
// const wss = new WebSocket.Server({ port: 7071 });

// const clients = new Map(); //store client Metadata

// wss.on('connection', (ws) => {
//     const id = uuidv4(); //assign new client id
//     const join_time = Date.now(); // assign cursor value
//     const metadata = { id, join_time };

//     clients.set(ws, metadata);



//     ws.on('message', (messageAsString) => {
//         // const message = JSON.parse(messageAsString);
//         // const metadata = clients.get(ws);
//         console.log("Received: " + messageAsString);

//         const outbound = "turn left";
//         // const outbound = JSON.stringify(message);

//         [...clients.keys()].forEach((client) => { //multicast!
//             client.send(outbound); 
//         });
//     });

//     ws.on("close", () => {
//         clients.delete(ws);
//         console.log("closed connection");
//     });

// });

// function uuidv4() { //generate unique ID
//     return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
//       var r = Math.random() * 16 | 0, v = c == 'x' ? r : (r & 0x3 | 0x8);
//       return v.toString(16);
//     });
//   }
//   console.log("wss up");

var WebSocketServer = require('websocket').server;
var http = require('http');

var server = http.createServer(function(request, response) {
    console.log((new Date()) + ' Received request for ' + request.url);
    response.writeHead(404);
    response.end();
});
server.listen(5000, function() {
    console.log((new Date()) + ' Server is listening on port 5000');
});

wsServer = new WebSocketServer({
    httpServer: server,
    autoAcceptConnections: false
});

function originIsAllowed(origin) {
  return true;
}

wsServer.on('request', function(request) {
    console.log(request)
    if (!originIsAllowed(request.origin)) {
      // Make sure we only accept requests from an allowed origin
      request.reject();
      console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
      return;
    }
    
    var connection = request.accept(null, request.origin)
    console.log((new Date()) + ' Connection accepted.');

    connection.on('message', function(message) {
        if (message.type === 'utf8') {
            received_message = message.utf8Data;
            json = JSON.parse(received_message);
            console.log('Received Message lw: ' + json.lw);
            console.log('Received Message rw: ' + json.rw);
            sum = parseInt(json.lw) + parseInt(json.rw);
            console.log("sum = " + sum)

            //connection.sendUTF(message.utf8Data); this resend the reseived message, instead of it i will send a custom message. hello from nodejs
            // connection.sendUTF("Turn left!");
        }
        else if (message.type === 'binary') {
            console.log('Received Binary Message of ' + message.binaryData.length + ' bytes');
            connection.sendBytes(message.binaryData);
        }
    });



    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' has disconnected.');
    });
});