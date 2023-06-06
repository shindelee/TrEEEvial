import {create_table} from "../src/table_ops/create_table.js";
import {delete_table} from "../src/table_ops/delete_table.js";
import {add_node} from "../src/table_ops/add_node.js";
import {table_list} from "../src/table_ops/list_tables.js";
import {query_visited} from "../src/table_ops/query_visited.js"

// create a new table if it already exists
// let tables = table_list();
// tables.then(function(result) {
//     if (result.TableNames.includes("Node_Information")) {
//         delete_table();
//         // create_table();
//     }

//     else {
//         create_table();
//     }
// })

// add_node(2,3,0,0);
query_visited(2,1);

// add_node("0,0","0,0",1,0,1);
// create_table();


