import {create_table} from "../src/table_ops/create_table.js";
import {delete_table} from "../src/table_ops/delete_table.js";
import {add_node} from "../src/table_ops/add_node.js";
import {table_list} from "../src/table_ops/list_tables.js";
import {query_visited} from "../src/table_ops/query_visited.js"


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
            Tremaux(message);
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


export function Tremaux (message){
    const X = message.x;
    const Y = message.y;
    const Front = message.fw;
    const Left = message.lw;
    const Right = message.rw;

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
