let x = 0; // State variable representing the true value
let P = 1; // Error covariance matrix
const Q = 0.1; // Process noise covariance
const R = 1; // Measurement noise covariance
let prevTime = Date.now(); // Previous timestamp

function updateKalmanFilter(z1, z2) {
    // Calculate time difference
    const currentTime = Date.now();
    const dt = (currentTime - prevTime) / 1000; // Convert to seconds
    prevTime = currentTime;
  
    // Calculate the weights
    const alpha = 0.5; // Weight for the first estimate
    const beta = 1 - alpha; // Weight for the second estimate
  
    // Combine the estimates to obtain the predicted state
    const xPred = alpha * z1 + beta * z2;
  
    // Combine the estimates to obtain the predicted measurement
    const zPred = xPred;
  
    // Calculate the Kalman gain
    const K = P / (P + R);
  
    // Update the state estimate
    x = xPred + K * (zPred - xPred);
  
    // Update the error covariance
    P = (1 - K) * P + Q;
}

const estimate1 = 10; // First estimate
const estimate2 = 12; // Second estimate

updateKalmanFilter(estimate1, estimate2);

console.log("Filtered estimate:", x);