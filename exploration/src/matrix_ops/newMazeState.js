import { getGridSquares } from './squares_intersected4.js';
import { widenPathWithSquares } from './path_widening3.js';
import { createWallAroundPath } from './add_walls55.js';

function newMazeState(mazeMatrix, prev_pos, tuple, mazeParams){
    // Remove the rover indication from the previous position.
    mazeMatrix[prev_pos[0]][prev_pos[1]] = 0;
    // Find the path taken.
    var path = getGridSquares(prev_pos[0], prev_pos[1], tuple[0][0], tuple[0][1]);
    // Widen the path.
    var widenedPath = widenPathWithSquares(path, mazeParams[0]);
    // Surround the path with walls.
    createWallAroundPath(mazeMatrix, widenedPath.squares, mazeParams[1], mazeParams[2], tuple[0], tuple[1], tuple[2], tuple[3], tuple[4]);
    // Indicate the current position of the rover.
    mazeMatrix[tuple[0][0]][tuple[0][1]] = 2;
}

const State = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [2, 0, 0, 0, 0, 0, 0, 0, 0]
  ];

const myTuple = [[2,6], 55, 1, 1, 1];
const Params = [1,          2,          45];
//       maze_width, proximity, range(degrees)

newMazeState(State, [8,0], myTuple, Params);
const mazeString = State.map(row => row.join(' ')).join('\n');
console.log(mazeString);