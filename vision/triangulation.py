import math

def triangulate(beacon1, beacon2, angle1, angle2):
    
    # beacon1 and beacon2 are objects {x: x_coordinate, y: y_coordinate} -> this may be RED or BLUE or YELLOW beacons
    # angles are in degrees

    # convert angles to radians
    angle1Rad = math.radians(angle1)
    angle2Rad = math.radians(angle2) 

    # find the coordinates of the current location
    x_current = ((beacon1['y'] - beacon2['y']) + beacon2['x'] * math.tan(angle2Rad) - beacon1['x'] * math.tan(angle1Rad)) / (math.tan(angle2Rad) - math.tan(angle1Rad))
    y_current = ((beacon1['y'] * math.tan(angle2Rad) - beacon2['y'] * math.tan(angle1Rad)) - ((beacon1['x'] - beacon2['x']) * math.tan(angle2Rad) * math.tan(angle1Rad))) / (math.tan(angle2Rad) - math.tan(angle1Rad))

    return x_current, y_current

# Testing: (Works!)
# Example coordinates of beacons:
beacon1 = {'x': 0, 'y': 0}
beacon2 = {'x': 5, 'y': 0}

angle1 = 60;  # angle in degrees
angle2 = 120;  # angle in degrees

position = triangulate(beacon1, beacon2, angle1, angle2);
print(position);  # (x_current, y_current)
