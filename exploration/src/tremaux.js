import {create_table} from "../src/table_ops/create_table.js";
import {delete_table} from "../src/table_ops/delete_table.js";
import {add_node} from "../src/table_ops/add_node.js";
import {table_list} from "../src/table_ops/list_tables.js";
import {query_visited} from "../src/table_ops/query_visited.js";
import {update_visited} from "../src/table_ops/update_visited.js";
import {get_coord} from "../src/table_ops/get_coord.js";


// {x,y,left,right,forward, turning_angle}
//node_info(x,y,parent_x,parent_y,unvisited,visited_count)
// 0 means path, 1 means wall

export async function Initialise () {
    // console.log("breakpoint 1");
    let tables = await table_list();
    // console.log("breakpoint 2");
    if (tables.TableNames.includes("Node_Information")) {
        // console.log("breakpoint 3");
        await delete_table();
        // console.log("breakpoint 4");
        await create_table();
    }
    else {
        create_table();
    }
};



export async function Tremaux (X,Y, parent_x, parent_y,r ,l, f, heading){
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
    const delta_y = Y - parent_y;
    const delta_x = X - parent_x;
    
    console.log("X is type " + typeof(X) );
    console.log("parent X is of type " + typeof(parent_x));
    const rad2deg = 57.2957795130823209;
    var theta = heading;
    if (theta < 0 ) {
        theta = theta + 360;
    }
    console.log("theta = " + theta);
    const visited = await get_coord(X,Y);
    console.log("have we visited here? " + visited);
    


    

    if (visited ==false) { //if we have not visited this node before
        if (theta <=45 || (theta > 315 && theta <=360)) { // origin path is north
            paths[0] = 1;
            paths[1] = l;
            paths[2] = f;
            paths[3] = r;
        }

        if (theta <=135 && theta > 45) { // origin path is east
            paths[0] = r;
            paths[1] = 1;
            paths[2] = l;
            paths[3] = f;
        }

        if (theta <= 225 && theta >135) { // origin path is south
            paths[0] = f;
            paths[1] = r;
            paths[2] = 1;
            paths[3] = l;
        }

        if (theta <=315 && theta > 225) { // origin path is west
            paths[0] = l;
            paths[1] = f;
            paths[2] = r;
            paths[3] = 1;
        }
    }
    else { //if visited before
        paths = await query_visited(X,Y);
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
    for (let i = 1; i < paths.length; i++) {
        if (paths[i] < paths[least_travelled_path]) {
            least_travelled_path = i;
        }
    }
    console.log("let's go down path #" + least_travelled_path);

    if (paths[least_travelled_path] == 2) {
        return "done traversing"; //done!!
    }
    //increment the minimum path
    paths[least_travelled_path] = paths[least_travelled_path] + 1;
    // console.log("new updated paths array: " + paths);

    if (visited) {
        await update_visited(X,Y,paths);
    }
    else {
        console.log (paths);
        await add_node(X,Y,parent_x, parent_y, paths);
    }

    //TODO: store more than one parent!!
    

    // //tell robot to turn and move there
    // console.log("my theta = " + theta + " of type " + typeof theta);
    if (theta <=45 || theta > 315) { // origin path is north
        // console.log("i am here!")
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

// Tremaux(5,4,0,0,1,1,0,67);