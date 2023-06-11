//test script


import {create_table} from "../src/table_ops/create_table.js";
import {delete_table} from "../src/table_ops/delete_table.js";
import {add_node} from "../src/table_ops/add_node.js";
import {table_list} from "../src/table_ops/list_tables.js";
import {query_visited} from "../src/table_ops/query_visited.js";
import {update_visited} from "../src/table_ops/update_visited.js";
import {get_coord} from "../src/table_ops/get_coord.js";
import { Initialise, Tremaux } from "../src/tremaux.js";
// import { get_coord } from "./table_ops/get_coord.js";


// var start = true;

// if (start) {
// await Initialise();
// start = false;
var parent_x = 0;
var parent_y = 0;
// }


const message = '{"x":"1", "y": "3", "l": "1", "f": "2", "r": "0"}'
const json = JSON.parse(message);
var directions = await Tremaux(json, parent_x, parent_y);
parent_x = json.x;
parent_y = json.y;
if (directions == "done traversing") {
console.log ("exploration done!")
}
console.log(directions);


