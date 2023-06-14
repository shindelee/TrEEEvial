import { Initialise, Tremaux } from "../src/tremaux.js";
import { server as WebSocketServer } from 'websocket';
import * as http from 'http';


 var start = true; //get from front end
 console.log(start);
 var parent_x;
 var parent_y;
//  var seq_no = 0;
 var directions = "";
 var buffer = "";

//display matrix
 
 var server = http.createServer(function(request, response) {
     console.log((new Date()) + ' Received request for ' + request.url);
     response.writeHead(404);
     response.end();
 });
 server.listen(5000, function() {
     console.log((new Date()) + ' Server is listening on port 5000');
 });
 
 const wsServer = new WebSocketServer({
     httpServer: server,
     autoAcceptConnections: false
 });
 
 function originIsAllowed(origin) {
   return true;
 }


 
 wsServer.on('request', async function(request) {
     console.log(request)
     if (!originIsAllowed(request.origin)) {
       // Make sure we only accept requests from an allowed origin
       request.reject();
       console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
    //    seq_no = 0;
       return;
     }
     
     var connection = request.accept(null, request.origin)
     console.log((new Date()) + ' Connection accepted.');
 
     connection.on('message', async function(message) {
         if (message.type === 'utf8') {
             var received_message = message.utf8Data;
             if (received_message != buffer) {
                var json = JSON.parse(received_message);
             
                // console.log("starting? " + start);

                if (start) {
                    console.log("breakpoint... Initialising")
                    await Initialise();
                    start = false;
                    parent_x = 0;
                    parent_y = 0;
                }
                console.log("received message!!");
                console.log("x = " + json.x);
                console.log("y = " + json.y);
                // console.log("front wall = " + json.f);
                //  console.log("expecting sequence number: " );
                //  if (seq_no == json.seq_no) {
                    
                directions = await Tremaux(json, parent_x, parent_y);
                parent_x = json.x;
             }

             else {
                console.log("duplicate message sent");
             }

             buffer = received_message;
             console.log("sent direction = " + directions);
             connection.sendUTF(directions);
         }
     });
 
 
 
     connection.on('close', function(reasonCode, description) {
         console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' has disconnected.');
     });
 });