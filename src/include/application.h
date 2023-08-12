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
    long inputFlags = ExposureMask | KeyPressMask | KeyReleaseMask;
    X11Window x11Window;

    int x, y, yVelocity;
    bool physicsEnabled = false, bounceEnabled = true;
    double bounceFactor = 0.6;
};
