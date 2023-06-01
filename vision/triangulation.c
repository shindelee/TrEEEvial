#include <math.h>

// Structure for a 2D point
typedef struct {
    double x;
    double y;
} Point;

// Calculate the distance between 2 points
double distance(Point a, Point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Find the intersection of two circles
void circle_intersection(Point c1, Point c2, double r1, double r2, Point* i1, Point* i2) {
    double d = distance(c1, c2);
    double a = (pow(r1, 2) - pow(r2, 2) + pow(d, 2)) / (2 * d);
    double h = sqrt(pow(r1, 2) - pow(a, 2));

    Point p;
    p.x = c1.x + a * (c2.x - c1.x) / d;
    p.y = c1.y + a * (c2.y - c1.y) / d;

    i1->x = p.x + h * (c2.y - c1.y) / d;
    i1->y = p.y - h * (c2.x - c1.x) / d;
    i2->x = p.x - h * (c2.y - c1.y) / d;
    i2->y = p.y + h * (c2.x - c1.x) / d;
}

Point triangulation(Point beacon1, Point beacon2, Point beacon3, double dist1, double dist2, double dist3) {
    Point intersection1, intersection2;

    circle_intersection(beacon1, beacon2, dist1, dist2, &intersection1, &intersection2);

    // fabs( ): calculate the absolute value of a floating-point number
    if (fabs(distance(intersection1, beacon3) - dist3) < fabs(distance(intersection2, beacon3) - dist3)) {
        return intersection1;
    } else {
        return intersection2;
    }
}

int main() {
    // Populate beacon locations and measured distances here
    Point beacon1, beacon2, beacon3;
    double d1, d2, d3;

    // Coordinates below are just examples:
    // Coordinates of red beacon
    beacon1.x = 0; beacon1.y = 0; 

    // Coordinates of the blue beacon
    beacon2.x = 5; beacon2.y = 0;

    // Coordinates of yellow beacon
    beacon3.x = 2.5; beacon3.y = 5;
    
    // Distances (size of image in pixels) - should get it from hardware
    dist1 = 2; 
    dist2 = 3;
    dist3 = 4;

    Point rover_location = triangulation(beacon1, beacon2, beacon3, dist1, dist2, dist3);

    // The rover_location now holds the estimated location of the rover
    // This could now be sent to the ESP32 or used for further processing

    return 0;
}
