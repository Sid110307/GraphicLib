#pragma once

#include <random>
#include <chrono>
#include <thread>

#include "window.h"

class Application
{
public:
    Application();

    void run();
    void randomizePosition();
    void updatePhysics();

private:
    static constexpr double maxTiltAngle = 45.0, bounceFactor = 0.6, playerMass = 1.25;

    long inputFlags = ExposureMask | KeyPressMask | KeyReleaseMask;
    X11Window x11Window;

    int x, y, rotationAngle = 0, yVelocity;
    bool physicsEnabled = false, bounceEnabled = true;
};
