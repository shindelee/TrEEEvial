function getGridSquares(x1, y1, x2, y2) {
    const squaresPassed = [];
  
    const dx = Math.abs(x2 - x1);
    const dy = Math.abs(y2 - y1);
    const sx = (x1 < x2) ? 1 : -1;
    const sy = (y1 < y2) ? 1 : -1;
  
    let err = dx - dy;
    let currentX = x1;
    let currentY = y1;
  
    while (true) {
      squaresPassed.push([ currentX, currentY ]);
  
      if (currentX === x2 && currentY === y2) {
        break;
      }
  
      const err2 = 2 * err;
  
      if (err2 > -dy) {
        err -= dy;
        currentX += sx;
      }
  
      if (err2 < dx) {
        err += dx;
        currentY += sy;
      }
    }
  
    return squaresPassed;
  }

//const squares = getGridSquares(0, 0, 27, 41);
//console.log(squares);

export { getGridSquares };