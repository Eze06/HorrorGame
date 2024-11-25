#pragma once
#ifndef LIGHT_H
#define LIGHT_H

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct Light
{
    enum LIGHT_TYPE
    {
        LIGHT_POINT = 0,
        LIGHT_DIRECTIONAL,
        LIGHT_SPOT,
    };

    LIGHT_TYPE type;
    glm::vec3 position;	// Define the light location
    glm::vec3 color;  	// Define the light color
    float power;  	// Define the light power
    float kC, kL, kQ;	// Variables to calculate attenuation
    glm::vec3 spotDirection;
    float cosCutoff;
    float cosInner;
    float exponent;


    Light() : position(0, 20, 0), color(1, 1, 1), power(1),
        kC(1), kL(0.1f), kQ(0.001f), 
        spotDirection(1.f), cosCutoff(45.f), cosInner(30.f), exponent(1.f)
    {
    }
};

#endif
