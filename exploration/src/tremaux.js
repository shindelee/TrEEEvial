import {create_table} from "../src/table_ops/create_table.js";
import {delete_table} from "../src/table_ops/delete_table.js";
import {add_node} from "../src/table_ops/add_node.js";
import {table_list} from "../src/table_ops/list_tables.js";
import {query_visited} from "../src/table_ops/query_visited.js"

//0 means path
//1 means path??

export function Tremaux (message, state){
    const X = parseInt(message.x);
    const Y = parseInt(message.y);
    const Front = parseInt(message.fw);
    const Left = parseInt(message.lw);
    const Right = parseInt(message.rw);
    var msg;

    if (state == "start") {
        let tables = table_list();
        tables.then(function(result) {
        if (result.TableNames.includes("Node_Information")) {
            delete_table();
            create_table();
        }

        else {
            create_table();
        }
        })
        var unvisited = new Set(); //set used for easy access and deletion

        if (Left == 0) {
            unvisited.add("left");
        }

        if (Front == 0) {
            unvisited.add("forward");
        }

        if (Right == 0) {
            unvisited.add("forward");
        }
        add_node(X,Y,X,Y,unvisited);
    }

    // if (Front == 0 && Left == 1 && Right == 1){
    //     msg = 'f';
    //     visit_update = queryv +1;
    //     update(X, Y, visit_update);
    //     // visited[Front] = update(X, Y, visit_update);
    //     // Do you update the visited of the tile you were just on or the tile you are moving to?
    // }

    //path to the front and the left
    if (Front == 0 && Left == 0 && Right == 1){
        if (visited[Front] < visited[Left]){
            msg = forward;
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Front]);
        }
        else{
            msg = "l";
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Left]);
        }
    }

    else if (Front == 0 && Left == 1 && Right == 0){
        if (visited[Front] < visited[Right]){
            msg = 'f';
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Front]);
        }
        else{
            msg = 'r';
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Right]);
        }
    }

    else if (Front == 0 && Left == 0 && Right == 0){
        if (Math.min(visited[Front], visited[Left], visited[Right]) == visited[Front]){
            msg = 'f';
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Front]);
        }
        else if (Math.min(visited[Front], visited[Left], visited[Right]) == visited[Left]){
            msg = 'l';
            rover.send(msg);
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Left]);
        }
        else{
            msg = 'r';
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Right]);
        }
    }

    else if (Front == 1 && Left == 0 && Right == 0){
        if (visited[Left] < visited[Right]){
            msg = 'l';
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Left]);
        }
        else{
            msg = 'r';
            visit_update = queryv +1;
            update(X, Y, visit_update);
            // update(visited[Right]);
        }
    }

    else if (Front == 1 && Left == 1 && Right == 0){
        msg = 'r';
        visit_update = queryv +1;
        update(X, Y, visit_update);
        // update(visited[Right]);
    }

    else if (Front == 1 && Left == 0 && Right == 1){
        msg = 'l';
        rover.send(msg);
        visit_update = queryv +1;
        update(X, Y, visit_update);
        // update(visited[Left]);
    }

    else{
        msg = 'backtrack';
        visit_update = queryv +1;
        update(X, Y, visit_update);
        // update(visited[Front]);
    }

    return msg;

}
