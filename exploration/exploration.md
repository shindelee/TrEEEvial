# Exploration Algorithm

The exploration algorithm will work as follows.
![Exploration algorithm](algorithm_visual.png)

The maze will be split up into blox, with each block being assigned a coordinate.

At the starting position, the robot will use beacons to determine its starting position and the surrounding walls.

CONCERN: Are we always starting from the edge of a maze? If no, then need at initialisation also need to send info about the wall BEHIND the robot.

With this information, the server will send the robot directions on where to go next. The robot needs to move by one coordinate.

After moving that one coordinate, it the cycle will repeat and the rover will give the server information on the syrrounding walls etc.

The decision of when to do an accurate survey of the coordinates can be decided on the ESP32 itself. The server will assume that whatever coordinates the ESP32 sends it is correct.

The ESP32 will probably only do a proper survey of the beacons when there is a decision to be made.
