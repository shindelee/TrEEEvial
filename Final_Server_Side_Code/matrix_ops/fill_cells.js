import {add_cell} from "../src/matrix_ops/add_cell.js"

export function fill_cells(x,y,parent_x,parent_y) {
    const delta_x = x-parent_x;
    const delta_y = parent_y;

    if (delta_x>delta_y) {
        const steps = Math.floor(delta_x/delta_y);
        for (let i = 0; i <steps; i++) {
            for (let j = 0 ; j < (floor(delta_x/steps)); j++) {
                // matrix[parent_x+ i*j][parent_y + i] = 2; //rover here 
                current_pos_x = parent_x + i*j;
                current_pos_y = parent_y + i;
                add_cell(current_pos_x, current_pos_y, )
            }
        }
    }
    else {

    }
}

//IDEA: give priority to nodes. Don't let nodes be altered