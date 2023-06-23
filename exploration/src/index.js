import { Initialise, Tremaux, myCallback } from "../src/tremaux.js";
import { server as WebSocketServer } from 'websocket';
import { Odometry } from "../src/dead_reckoning.js";
import { triangulation } from "../src/Triangulation.js";
import * as http from 'http';

import { newMazeState } from "./newMazeState.js";
import { aStarAlgorithm } from "./astar.js";
import express from 'express';
import cors from 'cors';
import { Console } from "console";

const PORT = process.env.PORT || 3001;
const app = express();

 var parent_x;
 var parent_y;
//  var seq_no = 0;
 var directions = "";
 var buffer = "";
 var x_accurate = 0;
 var y_accurate = 0;
 var x_coord = 0;
 var y_coord = 0;
 var heading = 0;
 var server;

 // Prerequisites for newMazeState polling.
// 366x225 maze for cross junction large case.
const State = [];
for(let j = 0; j < 100; j++){
    let row = []
    for(let j = 0; j < 150; j++){
        row.push(1);
    }
    State.push(row);
}

const widenBy = 6;

 // Start of Anlan's bit.

 app.use(cors({
    origin: '*'
}));

//app.use(express.static(path.resolve(__dirname, './updated2.0/build')));

app.use(cors({
    methods: ['GET','POST','DELETE','UPDATE','PUT','PATCH']
}));

app.get("/initial", (req, res) => {
    setTimeout(()=>{
        res.json(
            {
            "tableData33":[
            ]
        })
    }, 1000);
});

app.get("/shortestPath", (req, res) => {
    var coordinates = aStarAlgorithm(20, 20, 150, 155, State);
    for(let k = 0; k < coordinates.length; k++){
        State[coordinates[k][0]][coordinates[k][1]] = 4;
    }
    setTimeout(()=>{
        res.json(
            {
            "shortestPath": State
        })
    }, 1000);
});

app.get("/pollServer", (req, res) => {
    var d = new Date();

    var x = Math.round(parseInt(x_coord)) + 75;
    var y = Math.round(parseInt(y_coord));
    var p_x = Math.round(parseInt(parent_x)) + 75;
    var p_y = Math.round(parseInt(parent_y));
    newMazeState(State, [p_y, p_x], [y, x], widenBy);
    // newMazeState(State, [10, 10], [100, 100], widenBy);
    var myArray = [];
    for(let i = 0; i < 100; i++){
        myArray.push(State[i]);
    }
    const json_res = {
    "time" : myArray
    };
    res.send(json_res);
    }); 

/*
app.get('*', (req, res) => {
    res.sendFile(path.resolve(__dirname, './updated2.0/build', 'index.html'));
});
*/

// End of Anlan's bit.

 var server = http.createServer(async function(request, response) {
     console.log((new Date()) + ' Received request for ' + request.url);
     response.writeHead(404);
     response.end();
 });

 server.listen(5000, function() {
     console.log((new Date()) + ' Server is listening on port 5000');
 });

 
 //await Initialise();
 parent_x = 0;
 parent_y = -0.1;
 
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
                console.log("message received" + received_message);

                console.log("received message!!");
                console.log("left wheel revolutions = " + json.x);
                console.log("right wheel revolutions = " + json.y);

                // var coords_odo = await Odometry(-(parseInt(json.x)) , parseInt(json.y));
                var coords_odo = await Odometry((parseInt(json.x)) , parseInt(json.y));
                heading = heading + coords_odo[2];
                if (heading < 0 ) {
                    heading = heading + 360;
                }
                if (heading > 360) {
                    heading = heading - 360;
                }
                console.log(coords_odo);
                //0th element is x coordinate, 1st is y
                
                //need to remember previous x, y and theta, and add together
                    
                // if (parseInt(json.alpha) == 0 || parseInt(json.beta) == 0){
                    var hyp = Math.sqrt(Math.pow(coords_odo[0], 2) + Math.pow(coords_odo[1], 2));
                    console.log(coords_odo[0]*100*Math.sin(heading*Math.PI/180));
                    console.log(coords_odo[1]*100*Math.cos(heading*Math.PI/180));
                    x_accurate = coords_odo[0]*100*Math.sin(heading*Math.PI/180) + x_accurate;
                    y_accurate = coords_odo[1]*100*Math.cos(heading*Math.PI/180) + y_accurate;
                // }
                // else{
                //     // weighted median average filter, need to update triangulation
                //     var coords_triangulation = await triangulation([0, 0], [1, 7], [5, 3], parseInt(json.alpha), parseInt(json.beta));
                //     x_accurate = (coords_odo[0]*100 * 0.7 + coords_triangulation[0] * 0.3 * 100) + x_accurate;
                //     y_accurate = (coords_odo[1]*100 * 0.7 + coords_triangulation[1] * 0.3) + y_accurate;
                }

                console.log("x_accurate = " + x_accurate);
                console.log("y_accurate = " + y_accurate);

                x_coord = Math.floor(x_accurate/3);
                y_coord = Math.floor(y_accurate/3);


                var left_wall = parseInt(json.l);
                var right_wall = parseInt(json.r);
                var front_wall = parseInt(json.f);
                console.log("left_wall: " + left_wall);
                console.log("right_wall: " + right_wall);
                
                console.log("x-coord = " + x_coord + " , y-coord = " +y_coord);
        
                //directions = await Tremaux(x_coord,y_coord, parent_x, parent_y, right_wall, left_wall,front_wall, heading);
                parent_x = x_coord;
                parent_y = y_coord;

                parent_x =0;
                parent_y = 0;

             }

             console.log("sent direction = " + directions);
             connection.sendUTF(directions);
        //  }
        
     });
 
 
 
     connection.on('close', function(reasonCode, description) {
         console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' has disconnected.');
     });
 });

 app.listen(PORT, () => {
    console.log(`Server listening on ${PORT}`);
   });