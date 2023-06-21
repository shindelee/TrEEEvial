import { getGridSquares } from './squares_intersected4.js';
import { widenPathWithSquares } from './path_widening3.js';
import { createWallAroundPath } from './add_walls55.js';

function newMazeState(mazeMatrix, prev_pos, tuple, mazeParams, startFlag){
    // Remove the rover indication from the previous position.
    mazeMatrix[prev_pos[0]][prev_pos[1]] = 0;

    var node_change_x = 0;
    var node_change_y = 0;
    // Move slightly away from the node position, if not at starting position.
    if(startFlag === 0){
      node_change_x = 3 * Math.cos(tuple[1]*2*Math.PI/360)
      node_change_y = 3 * Math.sin(tuple[1]*2*Math.PI/360)
      node_change_x = Math.round(Number((node_change_x).toFixed(8)));
      node_change_y = Math.round(Number((node_change_y).toFixed(8)));
    }

    // Extend the path slightly in the forward direction.
    var change_x = (mazeParams[1]-1) * Math.cos(tuple[1]*2*Math.PI/360)
    var change_y = (mazeParams[1]-1) * Math.sin(tuple[1]*2*Math.PI/360)
    change_x = Math.round(change_x);
    change_y = Math.round(change_y);

    // Find the path taken.
    var path = getGridSquares(prev_pos[0] + node_change_y, prev_pos[1] + node_change_x, tuple[0][0] + change_y, tuple[0][1] + change_x);
    // Widen the path.
    var widenedPath = widenPathWithSquares(path, mazeParams[0]);
    // Surround the path with walls.
    createWallAroundPath(mazeMatrix, widenedPath.squares, mazeParams[1], mazeParams[2], [prev_pos[0]+node_change_y, prev_pos[1]+node_change_x], tuple[0], tuple[1], tuple[2], tuple[3], tuple[4]);
    // Indicate the current position of the rover.
    mazeMatrix[tuple[0][0]][tuple[0][1]] = 2;
}

// [18, 24] is bottom left.
const State = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
  ];

const myTuple = [[8,0], 135, 1, 1, 1];
const Params = [1,          2,          44];
//       maze_width, proximity, range(degrees)

//newMazeState(State, [2,6], myTuple, Params);

// Straight lines case.
//newMazeState(State, [1,0], [[1, 10], 0, 1, 1, 0], Params, 1);
//newMazeState(State, [1,10], [[12, 10], 90, 0, 1, 1], Params, 0);
//newMazeState(State, [12,10], [[12, 24], 0, 1, 1, 1], Params, 0);

// Diagonal case.
//newMazeState(State, [1,0], [[10, 12], 45, 0, 1, 1], Params, 1);
//newMazeState(State, [10,12], [[1, 24], -45, 0, 1, 1], Params, 0);

// The cross junction case.
/*
newMazeState(State, [10,0], [[10, 12], 0, 0, 0, 0], Params, 1);
newMazeState(State, [10,12], [[0, 12], -90, 1, 1, 1], Params, 0);
newMazeState(State, [0,12], [[10, 12], 90, 0, 0, 0], Params, 0);
newMazeState(State, [10,12], [[17, 12], 90, 1, 1, 1], Params, 0);
newMazeState(State, [17,12], [[10, 12], -90, 0, 0, 0], Params, 0);
newMazeState(State, [10,12], [[10, 24], 0, 1, 1, 1], Params, 0);
*/

// Diagonal cross junction case.
newMazeState(State, [1,0], [[10, 12], 45, 0, 0, 0], Params, 1);
newMazeState(State, [10,12], [[1, 24], -45, 1, 1, 1], Params, 0);
newMazeState(State, [1,24], [[10, 12], 135, 0, 0, 0], Params, 0);
newMazeState(State, [10,12], [[18, 24], 45, 1, 1, 1], Params, 0);
newMazeState(State, [18,24], [[10, 12], -135, 0, 0, 0], Params, 0);
newMazeState(State, [10,12], [[18, 0], 135, 1, 1, 1], Params, 0);

const mazeString = State.map(row => row.join(' ')).join('\n');
console.log(mazeString);