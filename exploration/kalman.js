let x = [0, 0]; // State variable representing the true coordinates [x, y]
let P = [[1, 0], [0, 1]]; // Error covariance matrix
const Q = [[0.1, 0], [0, 0.1]]; // Process noise covariance
const R = [[1, 0], [0, 1]]; // Measurement noise covariance
let prevTime = Date.now(); // Previous timestamp

function updateKalmanFilter(estimate1, estimate2) {
  // Calculate time difference
  const currentTime = Date.now();
  const dt = (currentTime - prevTime) / 1000; // Convert to seconds
  prevTime = currentTime;

  // Calculate the weights
  const alpha = 0.5; // Weight for the first estimate
  const beta = 1 - alpha; // Weight for the second estimate

  // Combine the estimates to obtain the predicted state
  const xPred = [
    alpha * estimate1[0] + beta * estimate2[0],
    alpha * estimate1[1] + beta * estimate2[1]
  ];

  // Combine the estimates to obtain the predicted measurement
  const zPred = xPred;

  // Calculate the Kalman gain
  const PPlusR = [
    [P[0][0] + R[0][0], P[0][1] + R[0][1]],
    [P[1][0] + R[1][0], P[1][1] + R[1][1]]
  ];
  const K = [
    [P[0][0] / PPlusR[0][0], P[0][1] / PPlusR[0][0]],
    [P[1][0] / PPlusR[1][1], P[1][1] / PPlusR[1][1]]
  ];

  // Update the state estimate
  const innovation = [
    zPred[0] - xPred[0],
    zPred[1] - xPred[1]
  ];
  x = [
    xPred[0] + K[0][0] * innovation[0] + K[0][1] * innovation[1],
    xPred[1] + K[1][0] * innovation[0] + K[1][1] * innovation[1]
  ];

  // Update the error covariance
  P = [
    [(1 - K[0][0]) * P[0][0] + (0 - K[0][1]) * P[1][0], (1 - K[0][0]) * P[0][1] + (0 - K[0][1]) * P[1][1]],
    [(0 - K[1][0]) * P[0][0] + (1 - K[1][1]) * P[1][0], (0 - K[1][0]) * P[0][1] + (1 - K[1][1]) * P[1][1]]
  ];
}

const estimate1 = [10, 5]; // First estimated coordinates [x1, y1]
const estimate2 = [12, 7]; // Second estimated coordinates [x2, y2]

updateKalmanFilter(estimate1, estimate2);

console.log("Filtered estimate:", x);