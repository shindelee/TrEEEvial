import {create_table} from "../src/table_ops/create_table.js";
import {delete_table} from "../src/table_ops/delete_table.js";
import {add_node} from "../src/table_ops/add_node.js";
import {table_list} from "../src/table_ops/list_tables.js";
import {query_visited} from "../src/table_ops/query_visited.js";
import {update_visited} from "../src/table_ops/update_visited.js";
import {get_coord} from "../src/table_ops/get_coord.js";
import { path } from "express/lib/application.js";


// {x,y,left,right,forward, turning_angle}
//node_info(x,y,parent_x,parent_y,unvisited,visited_count)
// 0 means path, 1 means wall

export function Initialise () {
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
};

/*
in unvisited dictionary, unvisited.Direction stores type int.
0: completely unexplored path
1: explored once before, allowed to backtrack there.
2: wall or visited twice- not allowed to go down the path anymore
 */


export function Tremaux (message, parent_x, parent_y){
    const X = parseInt(message.x);
    const Y = parseInt(message.y);
    var msg;
    var paths = [];
    /* 
    unvisited maintains number of path visitations + wall info
    The indices are arranged as follows
                0 (north)
                |
  3 (east) ----------- 1 (west)
                |
                2 (south)
    
    The array itself stores integers in the range of 0 and 2
    0 - path unexplored
    1 - path explored once, can explore once more if no othr choice
    2 - path explored twice, or wall present - cannot use it
     */

    //calculate direction of origin path
    const delta_y = y - parent_y;
    const delta_x = x - parent_x;
    const rad2deg = 57.2957795130823209;
    var theta = rad2deg * Math.atan2(delta_x,delta_y);
    if (theta < 0 ) {
        theta = theta + 360;
    }

    const visited = get_coord;

    if (visited ==false) { //if we have not visited this node before
        if (theta <=45 && theta > 315) { // origin path is north
            paths[0] = 1;
            paths[1] = parseInt(message.l);
            paths[2] = parseInt(message.f);
            paths[3] = parseInt(message.r);
        }

        if (theta <=135 && theta > 45) { // origin path is east
            paths[0] = parseInt(message.r);
            paths[1] = 1;
            paths[2] = parseInt(message.l);
            paths[3] = parseInt(message.f);
        }

        if (theta <= 225 && theta >135) { // origin path is south
            paths[0] = parseInt(message.f);
            paths[1] = parseInt(message.r);
            paths[2] = 1;
            paths[3] = parseInt(message.l);
        }

        if (theta <=315 && theta > 225) { // origin path is west
            paths[0] = parseInt(message.l);
            paths[1] = parseInt(message.f);
            paths[2] = parseInt(message.r);
            paths[3] = 1;
        }
    }
    else { //if visited before
        paths = query_visited(X,Y);
        if (theta <=45 && theta > 315) { // origin path is north
            paths[0] = paths[0] + 1;
        }

        if (theta <=135 && theta > 45) { // origin path is east
            paths[1] = paths[0] + 1;
        }

        if (theta <= 225 && theta >135) { // origin path is south
            paths[2] = paths[2] + 1;
        }

        if (theta <=315 && theta > 225) { // origin path is west
            paths[3] = paths[3] + 1;
        }
    }

    //find the path least travelled
    var least_travelled_path = 0;
    for (let i = 1; i < paths.length(); i++) {
        if (paths[i] < paths[least_travelled_path]) {
            least_travelled_path = i;
        }
    }

    if (paths[least_travelled_path] == 2) {
        return "done traversing"; //done!!
    }
    //increment the minimum path
    paths[least_travelled_path] = paths[least_travelled_path] + 1;

    if (visited) {
        update_visited(X,Y,paths);
    }
    else {
        add_node(X,Y,parent_x, parent_y, path);
    }
    

    //tell robot to turn and move there
    if (theta <=45 && theta > 315) { // origin path is north
        if (least_travelled_path==0) {
            return  "u-turn";
        }
        else if (least_travelled_path==1) {
            return "left";
        }
        else if (least_travelled_path==2) {
            return "straight";
        }
        else {
            return "right";
        }
    }

    if (theta <=135 && theta > 45) { // origin path is east
        if (least_travelled_path==0) {
            return  "right";
        }
        else if (least_travelled_path==1) {
            return "u-turn";
        }
        else if (least_travelled_path==2) {
            return "left";
        }
        else {
            return "straight";
        }
    }

    if (theta <= 225 && theta >135) { // origin path is south
        if (least_travelled_path==0) {
            return  "straight";
        }
        else if (least_travelled_path==1) {
            return "right";
        }
        else if (least_travelled_path==2) {
            return "u-turn";
        }
        else {
            return "left";
        }
    }

    if (theta <=315 && theta > 225) { // origin path is west
        if (least_travelled_path==0) {
            return  "left";
        }
        else if (least_travelled_path==1) {
            return "straight";
        }
        else if (least_travelled_path==2) {
            return "right";
        }
        else {
            return "u-turn";
        }
    }

    return "undefined";
}





    
