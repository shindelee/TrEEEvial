function average(estimate1, estimate2) {

  // Calculate the weights
  const alpha = 0.5; // Weight for the first estimate
  const beta = 1 - alpha; // Weight for the second estimate

  // Combine the estimates to obtain the predicted state
  x = [
    alpha * estimate1[0] + beta * estimate2[0],
    alpha * estimate1[1] + beta * estimate2[1]
  ];
 
}

const estimate1 = [10, 5]; // First estimated coordinates [x1, y1]
const estimate2 = [12, 7]; // Second estimated coordinates [x2, y2]

average(estimate1, estimate2);

console.log("Average estimate:", x);