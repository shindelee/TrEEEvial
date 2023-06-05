
import math

# Assuming all variables are defined and initialized
x1, y1, x2, y2, theta1, theta2 = 2, 0, 0, 0, 240, 120 # Replace with actual values

# Convert angles from degrees to radians
theta1_rad = math.radians(theta1)
theta2_rad = math.radians(theta2)

# Calculate x3 and y3
x3 = ((y1 - y2) + x2 * math.tan(theta2_rad) - x1 * math.tan(theta1_rad)) / (math.tan(theta2_rad) - math.tan(theta1_rad))

y3 = ((y1 * math.tan(theta2_rad) - y2 * math.tan(theta1_rad)) - ((x1 - x2) * math.tan(theta2_rad) * math.tan(theta1_rad))) / (math.tan(theta2_rad) - math.tan(theta1_rad))

print('x3:', x3)
print('y3:', y3)
