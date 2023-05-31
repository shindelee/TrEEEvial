#include <math.h>

struct point {
    double x;
    double y;
};

double distance(struct point a, struct point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

struct point circle_intersection(struct point a, struct point b, double distance_a, double distance_b) {
    // Find line between points
    double dx = b.x - a.x;
    double dy = b.y - a.y;

    // Find distance between points
    double d = distance(a, b);

    // Find a1, the distance from the first point to the line between the intersection points
    double a1 = (pow(distance_a, 2) - pow(distance_b, 2) + pow(d, 2)) / (2.0 * d);

    // Find h, the distance from the line between the intersection points to each intersection point
    double h = sqrt(pow(distance_a, 2) - pow(a1, 2));

    // Find p2, the point on the line between the points that is a1 away from the first point
    struct point p2 = { a.x + (dx * a1/d), a.y + (dy * a1/d) };

    // Find intersection points
    struct point intersection1 = { p2.x + h * dy / d, p2.y - h * dx / d };
    struct point intersection2 = { p2.x - h * dy / d, p2.y + h * dx / d };

    // Assume intersection1 is the correct point for now
    // In a real implementation, you'd want to use a third measurement to resolve the ambiguity
    return intersection1;
}

int main() {
    struct point beacon1 = { 0, 0 };
    struct point beacon2 = { 10, 0 };
    struct point beacon3 = { 0, 10 };

    double distance1 = 5;
    double distance2 = 7;
    double distance3 = 8;

    struct point estimated_position = trilateration(beacon1, beacon2, beacon3, distance1, distance2, distance3);
    
    printf("Estimated position: (%f, %f)\n", estimated_position.x, estimated_position.y);

    return 0;
}
