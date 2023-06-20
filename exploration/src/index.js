import { Initialise, Tremaux } from "../src/tremaux.js";
import { server as WebSocketServer } from 'websocket';
import { Odometry } from "../src/dead_reckoning.js";
import { triangulation } from "../src/Triangulation.js";
import * as http from 'http';


 var start = true; //get from front end
 console.log(start);
 var parent_x;
 var parent_y;
//  var seq_no = 0;
 var directions = "";
 var buffer = "";
 var_x_accurate = 0;
 var y_accurate = 0;
 var x_coord;
 var y_coord;

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
                console.log("message received" + json);
             
                // console.log("starting? " + start);

                if (start) { //at the beginning
                    console.log("breakpoint... Initialising")
                    await Initialise();
                    directions = await Tremaux(0, 0, 0, 0, right_wall, left_wall,front_wall, 0);
                    start = false;
                    parent_x = 0;
                    parent_y = 0;
                }

                else{
                console.log("received message!!");
                console.log("left wheel revolutions = " + json.x);
                console.log("right wheel revolutions = " + json.y);

                const coords_odo = await Odometry(parseInt(json.x),parseInt(json.y));
                console.log(coords_odo);
                //0th element is x coordinate, 1st is y
                
                //need to remember previous x, y and theta, and add together
                    
                if (parseInt(json.alpha) == 0 || parseInt(json.beta) == 0){
                    x_accurate = coords_odo[0] + x_accurate;
                    y_accurate = coords_odo[1] + y_accurate;
                }
                else{
                    // weighted median average filter, need to update triangulation
                    var coords_triangulation = await triangulation([0, 0], [1, 7], [5, 3], json.alpha, json.beta);
                    x_accurate = (coords_odo[0] * 0.7 + coords_triangulation[0] * 0.3) + x_accurate;
                    y_accurate = (coords_odo[1] * 0.7 + coords_triangulation[1] * 0.3) + y_accurate;
                }

                x_coord = Math.floor(x_accurate/3);
                y_coord = Math.floor(y_accurate/3);


                var left_wall = parseInt(json.l);
                var right_wall = parseInt(json.r);
                var front_wall = parseInt(json.f);
                
                console.log("x-coord = " + x_coord + "y-coord = " +y_coord);
                
                if(x_coord == parent_x && y_coord == parent_y) {
                    directions = await Tremaux(x_coord,y_coord, parent_x, parent_y, right_wall, left_wall,front_wall, coords_odo[2]);
                    parent_x = x_coord;
                    parent_y = y_coord;
                }

             }

             console.log("sent direction = " + directions);
             connection.sendUTF(directions);
         }
        }
     });
 
 
 
     connection.on('close', function(reasonCode, description) {
         console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' has disconnected.');
     });
 });