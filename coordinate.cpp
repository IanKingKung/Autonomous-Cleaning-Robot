#include <cmath>
#include "coordinate.h"

const float TWO_PI = 2.0f * M_PI;

POINT2D polar_to_cartesian(float r, float theta_rad) {
    POINT2D point;
    point.x = r * std::cos(theta_rad);
    point.y = r * std::sin(theta_rad);
    return point;
}

POINT2D steps_to_cartesian(float r, int step, int total_steps_per_revolution) {
    float radians_per_step = TWO_PI / static_cast<float>(total_steps_per_revolution);
    float theta_rad = static_cast<float>(step) * radians_per_step;
    return polar_to_cartesian(r, theta_rad);
}