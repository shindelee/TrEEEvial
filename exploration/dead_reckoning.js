const g = 9.81;
const R_wheel = 0.035;
const d_width = 0.165;

// Used to calculate the initial orientaion from the outputs of the inertial sensors when the rover is stationary
// Assuming starting point is origin
function InitialAlignment(Theta, Phi, Hx, Hy){
    // fb is acceleration on the body frame
    // Phi is the roll angle
    // Theta is the pitch angle
    fb = [[g*Math.sin(Theta)]
          [-g*Math.cos(Theta) * Math.sin(Phi)]
          [-g*Math.cos(Theta) * Math.cos(Phi)]
    ];

    // (Hx, Hy) are the dual axial magnetic fields that are measured by the magnetic sensors
    // phi is the yaw angle which is the right-handed rotation about true north
    // lambda is the declination angle
    const lambda = 0.76;
    const alpha = Math.atan(Hy/Hx);
    const phi = lambda - alpha;
}

function PositionAlgorithm(){
    InitialAlignment(Theta, Phi, Hx, Hy);

    Cb = [[Math.cos(phi) * Math.cos(Theta), Math.cos(Phi) * Math.sin(Theta) * Math.sin(Phi) - Math.sin(phi) * Math.cos(Phi), Math.cos(Phi) * Math.sin(Theta) * Math.cos(Phi) - Math.sin(phi) * Math.sin(Phi)]
          [Math.sin(phi) * Math.cos(Theta), Math.sin(Phi) * Math.sin(Theta) * Math.sin(Phi) + Math.cos(phi) * Math.cos(Phi), Math.sin(Phi) * Math.sin(Theta) * Math.cos(Phi) - Math.cos(phi) * Math.sin(Phi)]
          [-Math.sin(Theta),                Math.cos(Theta) * Math.sin(Phi),                                                 Math.cos(Theta) * Math.cos(Phi)                                                ]
    ];

    // velocity vector
    v[k] = [[vN[k]]
            [vE[k]]
            [vD[k]]
        ];
    v[k+1] = v[k] + (Cb * fk + g) * delta_t;

    // position vector
    P[k] = [[PN[k]]
            [PE[k]]
            [PD[k]]
        ];
    P[k+1] = P[k] + v[k] * delta_t + 1/2 * (Cb * fk + g) * Math.pow(delta_t);

    // the rotor encoder generates N pulses while the wheel rotates 360 degrees
    // if the measured pulses are M counts, each wheel's rotary angle becomes:
    eta_l[k] = Ml/N * 2 * Math.PI;
    eta_r[k] = Mr/N * 2 * Math.PI;
    // eta_kl and eta_kr are the left and right wheel's rotary angles in radians
    // Ml and Mr are the measured pulses on the left and right encoders

    a_l[k] = R_wheel * eta_l[k];
    a_r[k] = R_wheel * eta_r[k];
    a[k] = (a_kl + a_kr)/2;

    // delta_phi_k is the robot's yaw angle rate
    delta_phi[k] = (a_l[k] - a_r[k])/d_width;

    r[k] = a[k]/delta_phi[k];

    // delta_lambda is the robot's position rate
    delta_lamda[k] = Math.sqrt(2 * (1 - Math.cos(delta_phi_k)) *  Math.pow(rk));

    delta_PN[k] = delta_lamda[k] * Math.cos(phi[k-1] + delta_phi[k]/2);
    delta_PE[k] = delta_lamda[k] * Math.sin(phi[k-1] + delta_phi[k]/2);

    // if roll and pitch are experienced by the mobile robot, the position rate that compensates the attitude is:
    delta_mod_PN[k] = delta_PN[k] * Math.cos(Theta) + delta_PEk * Math.sin(Phi) * Math.cos(Theta);
    delta_mod_PE[k] = delta_PE[k] * Math.cos(Phi);

    // the mobile robot's position and yaw direction are defined as:
    PN[k+1] = PN[k] + delta_PN[k];
    PE[k+1] = PE[k] + delta_PE[k];
    phi[k+1] = phi[k] + delta_phi[k];

    // the velocity is a differential of the mobile robot's quantity of change in position
    vN[k] = delta_mod_PN[k]/delta_t;
    vE[k] = delta_mod_PE[k]/delta_t;
}