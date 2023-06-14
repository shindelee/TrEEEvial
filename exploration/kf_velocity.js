let x = [0, 0]; // State variable representing the true coordinates [x, y]
let P = [[1, 0], [0, 1]]; // Error covariance matrix
const Q = [[0.1, 0], [0, 0.1]]; // Process noise covariance
const R = [[1, 0], [0, 1]]; // Measurement noise covariance
let prevTime = Date.now(); // Previous timestamp

function updateKalmanFilter(coord, velocity) {
  // Calculate time difference
  const currentTime = Date.now();
  const dt = (currentTime - prevTime) / 1000; // Convert to seconds
  prevTime = currentTime;

  // State prediction step
  const F = [[1, dt], [0, 1]]; // State transition matrix
  const xPred = [
    x[0] + velocity[0] * dt,
    x[1] + velocity[1] * dt
  ];

  // Error covariance prediction step
  const PPred = Math.add(Math.multiply(Math.multiply(F, P), Math.transpose(F)), Q);

  // Measurement update step
  const H = [[1, 0], [0, 1]]; // Measurement matrix
  const y = Math.subtract(coord, Math.multiply(H, xPred));
  const S = Math.add(Math.multiply(Math.multiply(H, PPred), Math.transpose(H)), R);
  const K = Math.multiply(Math.multiply(PPred, Math.transpose(H)), Math.inv(S));

  // State update step
  x = Math.add(xPred, Math.multiply(K, y));

  // Error covariance update step
  P = Math.multiply(Math.subtract(Math.eye(2), Math.multiply(K, H)), PPred);
}

const initialCoord = [0, 0]; // Initial coordinate [x0, y0]
const initialVelocity = [1, 1]; // Initial velocity [vx0, vy0]

// Example usage:
updateKalmanFilter([10, 5], initialVelocity); // Update the Kalman Filter with the new coordinate and velocity

console.log("Filtered coordinate:", x);






// Step 1: Define state variables

let state = { x: 0, y: 0, velocity: 0, acceleration: 0 };
let covarianceMatrix = [[1, 0], [0, 1]]; // Initialize covariance matrix

// Step 2: Initialize state and covariance matrix

// Step 3: Define motion model

function predictMotion(previousState, distanceMoved) {
  // Use previousState and distanceMoved to predict new state
  // Update and return the predicted state
  const dt = 1; // Time step, assuming a fixed time interval between updates

  // Extract the previous state variables
  const { x, y, velocity, acceleration } = previousState;

  // Predict the new state based on the motion model
  const newVelocity = velocity + acceleration * dt;
  const newX = x + (distanceMoved * Math.cos(newVelocity)) * dt;
  const newY = y + (distanceMoved * Math.sin(newVelocity)) * dt;

  // Update and return the predicted state
  return {
    x: newX,
    y: newY,
    velocity: newVelocity,
    acceleration,
  };
}

// Step 4: Predict step

function predict() {
  // Use the motion model to predict the new state and covariance matrix
  // Update state and covarianceMatrix
  // Use the motion model to predict the new state and covariance matrix
  const predictedState = predictMotion(state, distanceMoved);

  // Update the state
  state = predictedState;

  // Update the covariance matrix based on the motion model
  const { velocity, acceleration } = state;
  const dt = 1; // Time step, assuming a fixed time interval between updates
  const processNoise = 0.1; // Adjust this value based on your system's characteristics

  // Update the covariance matrix
  covarianceMatrix.x += (velocity * Math.cos(velocity) * dt) + processNoise;
  covarianceMatrix.y += (velocity * Math.sin(velocity) * dt) + processNoise;
  covarianceMatrix.velocity += acceleration * dt + processNoise;
  covarianceMatrix.acceleration += processNoise;
}

// Step 5: Measurement update step

function updateMeasurement(triangulationResult, distanceMoved) {
    // Extract the coordinates from the triangulation result
    const { x, y } = triangulationResult;
  
    // Define the measurement noise covariance matrix
    const measurementNoise = 0.1; // Adjust this value based on the accuracy of your triangulation
  
    // Define the measurement matrix H
    const H = [
      [1, 0, 0, 0], // Measurement of x coordinate
      [0, 1, 0, 0]  // Measurement of y coordinate
    ];
  
    // Compute the innovation (measurement residual)
    const predictedMeasurement = [state.x, state.y];
    const innovation = [
      x - predictedMeasurement[0],
      y - predictedMeasurement[1]
    ];
  
    // Compute the innovation covariance
    const innovationCovariance = [
      [covarianceMatrix.x, 0],
      [0, covarianceMatrix.y]
    ];
  
    // Compute the Kalman gain
    const transposeH = Math.transpose(H);
    const tmp1 = Math.multiply(innovationCovariance, transposeH);
    const tmp2 = Math.inv(Math.multiply(H, tmp1).add(measurementNoise));
    const kalmanGain = Math.multiply(tmp1, tmp2);
  
    // Update the state estimate using the innovation and Kalman gain
    const stateUpdate = Math.multiply(kalmanGain, innovation);
    state.x += stateUpdate[0];
    state.y += stateUpdate[1];
  
    // Update the covariance matrix using the Kalman gain
    const identityMatrix = Math.identity(4); // Assuming 4 state variables
    const kalmanGainH = Math.multiply(kalmanGain, H);
    covarianceMatrix = Math.multiply(Math.subtract(identityMatrix, kalmanGainH), covarianceMatrix);
  }
  

// Step 6: Repeat steps 4 and 5

function kalmanFilter(beacon1, beacon2, beacon3, distanceMoved) {
  predict();
  updateMeasurement(beacon1, beacon2, beacon3);
}

// Usage

kalmanFilter(beacon1, beacon2, beacon3, distanceMoved);

