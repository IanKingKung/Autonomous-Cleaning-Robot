#ifndef COORDINATE_H
#define COORDINATE_H

struct POINT2D {
    float x;
    float y;
};

//convert stepper motor rotation steps to cartesion coordinates
POINT2D steps_to_cartesian(float r, int step, int total_steps_per_revolution);

//convert polar coordinates to cartesian
POINT2D polar_to_cartesian(float r, float theta_rad);

#endif // COORDINATE_H