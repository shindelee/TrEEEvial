function triangulate(beacon1, beacon2, angle1, angle2) {
    // convert angles to radians
    let angle1Rad = Math.PI / 180 * angle1;
    let angle2Rad = Math.PI / 180 * angle2;

    // find the coordinates of the current location
    let x_current = ((beacon1.y - beacon2.y) + beacon2.x * Math.tan(angle2Rad) - beacon1.x * Math.tan(angle1Rad)) / (Math.tan(angle2Rad) - Math.tan(angle1Rad));
    let y_current = ((beacon1.y * Math.tan(angle2Rad) - beacon2.y * Math.tan(angle1Rad)) - ((beacon1.x - beacon2.x) * Math.tan(angle2Rad) * Math.tan(angle1Rad))) / (Math.tan(angle2Rad) - Math.tan(angle1Rad));

    return {x: x_current, y: y_current};
}

// Testing: (Works!)
// Example coordinates of beacons:
let beacon1 = {x: 0, y: 0};
let beacon2 = {x: 5, y: 0};

let angle1 = 60;  // angle in degrees
let angle2 = 120;  // angle in degrees

let position = triangulate(beacon1, beacon2, angle1, angle2);
console.log(position);  // {x: ..., y: ...}
