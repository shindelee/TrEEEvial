//test script


import {create_table} from "../src/table_ops/create_table.js";
import {delete_table} from "../src/table_ops/delete_table.js";
import {add_node} from "../src/table_ops/add_node.js";
import {table_list} from "../src/table_ops/list_tables.js";
import {query_visited} from "../src/table_ops/query_visited.js";
import {increment_visited} from "../src/table_ops/update_visited.js";
import {get_coord} from "../src/table_ops/get_coord.js";


const hi = await get_coord(2,3);
 if (hi.response.Item) {
    console.log("the coordinate is in the table!");
    console.log(hi.response.Item.parent);
  }

  else {
    console.log("the coordinate is NOT in the table!");
  }


// add_node("0,0","0,0",1,0,1);
// create_table();


