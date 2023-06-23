function widenPathWithSquares(path, width) {
    const widenedPath = [];
    const squaresIncluded = [];
  
    for (let i = 0; i < path.length; i++) {
      const point = path[i];
      const nextPoint = path[(i + 1) % path.length]; // Wrap around to the start for closed paths
  
      // Calculate the angle of the path segment
      const angle = Math.atan2(nextPoint[1] - point[1], nextPoint[0] - point[0]);
  
      // Calculate the perpendicular direction
      const perpendicularAngle = angle + Math.PI / 2;
  
      // Calculate the offset in the perpendicular direction
      const offsetX = width * Math.cos(perpendicularAngle);
      const offsetY = width * Math.sin(perpendicularAngle);
  
      // Calculate the widened points on both sides of the path
      const widenedPoint1 = [
        Math.round(point[0] + offsetX),
        Math.round(point[1] + offsetY)
      ];
      const widenedPoint2 = [
        Math.round(point[0] - offsetX),
        Math.round(point[1] - offsetY)
      ];
  
      widenedPath.push(widenedPoint1, widenedPoint2);
  
      // Include all the squares within the widened path
      includeSquaresWithinPath(point[0], point[1], widenedPoint1[0], widenedPoint1[1], squaresIncluded);
      includeSquaresWithinPath(point[0], point[1], widenedPoint2[0], widenedPoint2[1], squaresIncluded);
    }
  
    return { outline: widenedPath, squares: squaresIncluded };
  }
  
  function includeSquaresWithinPath(x1, y1, x2, y2, squaresIncluded) {
    const minX = Math.min(x1, x2);
    const maxX = Math.max(x1, x2);
    const minY = Math.min(y1, y2);
    const maxY = Math.max(y1, y2);
  
    for (let x = minX; x <= maxX; x++) {
      for (let y = minY; y <= maxY; y++) {
        squaresIncluded.push([x, y]);
      }
    }
  }

/*
const path = [[3, 0], [3, 1], [3, 2], [3, 3], [3, 4]];
const path_diagonal = [[0, 0], [1, 1], [2, 2], [3, 3], [4, 4]];
const path2 = [[2, 0], [2, 1], [2, 2], [2, 3]]

const widenedPath = widenPathWithSquares(path2, 1);
console.log(widenedPath.outline);
console.log(widenedPath.squares);
*/

export { widenPathWithSquares, includeSquaresWithinPath };