#pragma once

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class AltAzCamera
{
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    AltAzCamera();
    ~AltAzCamera();

    void Init(float theta, float phi, float distance);
    void Reset();
    void Update(double dt);

private:
    float phi;      // elevation
    float theta;    // azimuth
    float distance;

    bool isDirty;   // indicate if there is a need to recalculate the camera attributes

    void Refresh();
};

