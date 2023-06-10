import { Initialise, Tremaux } from "../tremaux.js";

 var WebSocketServer = require('websocket').server;
 var http = require('http');
 var start = false; //get from front end
 var parent_x;
 var parent_y;
 
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
             if (start) {
                Initialise();
                start = false;
                parent_x = json.x;
                parent_y = json.y;
             }

             var directions = Tremaux(json, parent_x, parent_y);
             parent_x = json.x;
             parent_y = json.y;
             if (directions == "done traversing") {
                console.log ("exploration done!")
             }
             connection.sendUTF(directions);
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