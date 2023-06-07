import {update} from "./table_ops/update.js";
import {queryv} from "./table_ops/query_visited.js";


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

    connection.on('message', function(received_message) {
        if (received_message.type === 'utf8') {
            const message = JSON.parse(received_message.utf8Data);
            console.log('Received Message: ' + message);
            //connection.sendUTF(message.utf8Data); this resend the reseived message, instead of it i will send a custom message. hello from nodejs
            connection.sendUTF("Turn left!");
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


export function Tremaux (){
    visited[current_pos] += 1;
    wall.Front = rec.Front;
    wall.Left = rec.Left;
    wall.Right = rec.Right;
    last_visited = current_pos;

    if (Front === 0 && Left === 1 && Right === 1){
        msg = forward;
        rover.send(msg);
        visit_update = queryv +1;
        update(X, Y, visit_update);
        // visited[Front] = update(X, Y, visit_update);
        // Do you update the visited of the tile you were just on or the tile you are moving to?
    }

    else if (Front === 0 && Left === 0 && Right === 1){
        if (visited[Front] < visited[Left]){
            msg = forward;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Front]);
        }
        else{
            msg = go_left;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Left]);
        }
    }

    else if (Front === 0 && Left === 1 && Right === 0){
        if (visited[Front] < visited[Right]){
            msg = forward;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Front]);
        }
        else{
            msg = go_right;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Right]);
        }
    }

    else if (Front === 0 && Left === 0 && Right === 0){
        if (Math.min(visited[Front], visited[Left], visited[Right]) === visited[Front]){
            msg = forward;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Front]);
        }
        else if (Math.min(visited[Front], visited[Left], visited[Right]) === visited[Left]){
            msg = go_left;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Left]);
        }
        else{
            msg = go_right;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Right]);
        }
    }

    else if (Front === 1 && Left === 0 && Right === 0){
        if (visited[Left] < visited[Right]){
            msg = go_left;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Left]);
        }
        else{
            msg = go_right;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Right]);
        }
    }

    else if (Front === 1 && Left === 1 && Right === 0){
        msg = go_right;
        rover.send(msg);
        visit_update = queryv +1;
        update(X, Y, visit_update);
        // update(visited[Right]);
    }

    else if (Front === 1 && Left === 0 && Right === 1){
        msg = go_left;
        rover.send(msg);
        visit_update = queryv +1;
        update(X, Y, visit_update);
        // update(visited[Left]);
    }

    else{
        msg = backtrack;
        rover.send(msg);
        visit_update = queryv +1;
        update(X, Y, visit_update);
        // update(visited[Front]);
    }
}
