function createWallAroundPath(matrix, pathIndexes, proximity, range, initialPathCell, specifiedPathCell, angularDirection, leftWall, frontWall, rightWall) {
    // Helper function to check if an index is valid within the matrix bounds
    function isValidIndex(row, col) {
      return row >= 0 && row < matrix.length && col >= 0 && col < matrix[0].length;
    }
  
    
    // Helper function to set a value at a specific index
    function setValue(row, col, value) {
      if (isValidIndex(row, col)) {
        matrix[row][col] = value;
      }
    }
  
    // Iterate over the pathIndexes and identify the edge cells
    const edgeCells = new Set();
    for (const [row, col] of pathIndexes) {
      for (let dx = -1; dx <= 1; dx++) {
        for (let dy = -1; dy <= 1; dy++) {
          const newRow = row + dx;
          const newCol = col + dy;
          if (dx === 0 && dy === 0) continue; // Skip the current cell
          if (!isValidIndex(newRow, newCol)) continue; // Skip invalid cells
          if (matrix[newRow][newCol] === 0) {
            // Neighboring cell is part of the path, mark the current cell as an edge cell
            edgeCells.add(`${row},${col}`);
          }
        }
      }
    }

  
    // Set the neighboring cells of the edge cells as walls
    for (const edgeCell of edgeCells) {
      const [row, col] = edgeCell.split(',').map(Number);
      for (let dx = -1; dx <= 1; dx++) {
        for (let dy = -1; dy <= 1; dy++) {
          const newRow = row + dx;
          const newCol = col + dy;
          if (dx === 0 && dy === 0) continue; // Skip the current cell
          if (pathIndexes.some(([r, c]) => r === newRow && c === newCol)) {
            // Skip neighboring cells that are part of the path
            continue;
          }
          setValue(newRow, newCol, 1);
        }
      }
    }

    if(frontWall === 0){
      // Find wall cells within proximity of the specified path cell and remove them based on the angular direction
      for (let row = Math.max(0, specifiedPathCell[0] - proximity); row <= Math.min(matrix.length - 1, specifiedPathCell[0] + proximity); row++) {
        for (let col = Math.max(0, specifiedPathCell[1] - proximity); col <= Math.min(matrix[row].length - 1, specifiedPathCell[1] + proximity); col++) {
          if (matrix[row][col] !== 1) continue; // Skip cells that are not wall cells
          const angle = Math.atan2(row - specifiedPathCell[0], col - specifiedPathCell[1]);
          const degrees = angle * (180 / Math.PI);
          if (isWithinAngularDirection(degrees, angularDirection, range)) {
            matrix[row][col] = 0; // Remove wall cells within proximity and specified angular direction
          }
        }
      }
    }

    if(leftWall === 0){
      // Find wall cells within proximity of the specified path cell and remove them based on the angular direction
      for (let row = Math.max(0, specifiedPathCell[0] - proximity); row <= Math.min(matrix.length - 1, specifiedPathCell[0] + proximity); row++) {
        for (let col = Math.max(0, specifiedPathCell[1] - proximity); col <= Math.min(matrix[row].length - 1, specifiedPathCell[1] + proximity); col++) {
          if (matrix[row][col] !== 1) continue; // Skip cells that are not wall cells
          const angle = Math.atan2(row - specifiedPathCell[0], col - specifiedPathCell[1]);
          const degrees = angle * (180 / Math.PI);
          if (isWithinAngularDirection(degrees, angularDirection - 90, range)) {
            matrix[row][col] = 0; // Remove wall cells within proximity and specified angular direction
          }
        }
      }
    }

    if(rightWall === 0){
      // Find wall cells within proximity of the specified path cell and remove them based on the angular direction
      for (let row = Math.max(0, specifiedPathCell[0] - proximity); row <= Math.min(matrix.length - 1, specifiedPathCell[0] + proximity); row++) {
        for (let col = Math.max(0, specifiedPathCell[1] - proximity); col <= Math.min(matrix[row].length - 1, specifiedPathCell[1] + proximity); col++) {
          if (matrix[row][col] !== 1) continue; // Skip cells that are not wall cells
          const angle = Math.atan2(row - specifiedPathCell[0], col - specifiedPathCell[1]);
          const degrees = angle * (180 / Math.PI);
          if (isWithinAngularDirection(degrees, angularDirection + 90, range)) {
            matrix[row][col] = 0; // Remove wall cells within proximity and specified angular direction
          }
        }
      }
    }

    // Remove the back wall of the inital position.
    // Find wall cells within proximity of the specified path cell and remove them based on the angular direction
    for (let row = Math.max(0, initialPathCell[0] - (proximity)); row <= Math.min(matrix.length - 1, initialPathCell[0] + proximity); row++) {
      for (let col = Math.max(0, initialPathCell[1] - (proximity)); col <= Math.min(matrix[row].length - 1, initialPathCell[1] + proximity); col++) {
        if (matrix[row][col] !== 1) continue; // Skip cells that are not wall cells
        const angle = Math.atan2(row - initialPathCell[0], col - initialPathCell[1]);
        const degrees = angle * (180 / Math.PI);
        var reversedAngularDirection = 0;
        if(angularDirection > 0){
          reversedAngularDirection = -180 + angularDirection;
        }
        else{
          reversedAngularDirection = 180 + angularDirection;
        }
        if (isWithinAngularDirection(degrees, reversedAngularDirection, range)) {
          matrix[row][col] = 0; // Remove wall cells within proximity and specified angular direction
        }
      }
    }
  }

// Helper function to check if an angle is within the specified angular direction range
function isWithinAngularDirection(angle, angularDirection, range) {
  const startAngle = angularDirection - range;
  const endAngle = angularDirection + range;
  if (startAngle < -180) {
    return angle >= startAngle + 360 || angle <= endAngle;
  } else if (endAngle >= 180) {
    return angle >= startAngle || angle <= endAngle - 360;
  } else {
    return angle >= startAngle && angle <= endAngle;
  }
}
  
/*
  // Example usage:
  const maze1 = [
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0]
  ];

  // Example usage:
const maze = [
  [0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0]
];

const finalPosition = [4, 3];
  
const path = [[1, 1], [1, 2], [1, 3], [2, 3], [3, 3]];
const path2 = [[0, 2],[1, 2],[2, 2],[3, 2],[4, 2],[0, 3],[1, 3],[2, 3],[3, 3],[4, 3],[0, 4],[1, 4], [2, 4], [3, 4], [4, 4]];

createWallAroundPath(maze, path2, 1, 0, finalPosition, 90, 1, 0, 1);
const mazeString = maze.map(row => row.join(' ')).join('\n');
console.log(mazeString);

*/

export { createWallAroundPath, isWithinAngularDirection };