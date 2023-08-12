#include "include/window.h"

X11Window::X11Window(long inputFlags)
{
    display = XOpenDisplay(nullptr);
    if (!display)
    {
        std::cerr << "Failed to open X11 display." << std::endl;
        exit(EXIT_FAILURE);
    }

    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, WIDTH, HEIGHT, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    gc = XCreateGC(display, window, 0, nullptr);
    font = XLoadQueryFont(display, "fixed");
    XSetFont(display, gc, font->fid);

    XSelectInput(display, window, inputFlags);
    XStoreName(display, window, "X11 Window");
    XMapWindow(display, window);
}

X11Window::~X11Window()
{
    XFreeGC(display, gc);
    XCloseDisplay(display);
}

void X11Window::draw(int x, int y)
{
    // Screen
    XSetForeground(display, gc, WhitePixel(display, screen));
    XFillRectangle(display, window, gc, 0, 0, WIDTH, HEIGHT);

    // Help text
    XSetForeground(display, gc, 0x000000);
    XDrawString(display, window, gc, 10, 20, "Press ESC to exit.", 18);
    XDrawString(display, window, gc, 10, 40, "Press Space to move the rectangle to a random position.", 55);
    XDrawString(display, window, gc, 10, 60, "Press Up, Down, Left, Right to move the rectangle.", 50);
    XDrawString(display, window, gc, 10, 80, "Press Enter to reset the rectangle's position.", 46);
    XDrawString(display, window, gc, 10, 100, "Press P to toggle physics.", 26);
    XDrawString(display, window, gc, 10, 120, "Press B to toggle bounce.", 25);

    // Variables
    std::string physicsEnabledStr =
            "Physics: " + std::string(variableManager.getVariable<bool>("physicsEnabled") ? "yes" : "no");
    std::string bounceEnabledStr =
            "Bounce: " + std::string(variableManager.getVariable<bool>("bounceEnabled") ? "yes" : "no");
    std::string bounceFactorStr =
            "Bounce factor: " + std::to_string(variableManager.getVariable<double>("bounceFactor"));

    XDrawString(display, window, gc, WIDTH - 200, 20, physicsEnabledStr.c_str(), (int) physicsEnabledStr.length());
    XDrawString(display, window, gc, WIDTH - 200, 40, bounceEnabledStr.c_str(), (int) bounceEnabledStr.length());
    XDrawString(display, window, gc, WIDTH - 200, 60, bounceFactorStr.c_str(), (int) bounceFactorStr.length());

    // Player
    XSetForeground(display, gc, 0x0000FF);
    XFillRectangle(display, window, gc, x, y, 100, 100);

    // Objects
    XSetForeground(display, gc, 0xFF0000);
    XDrawRectangle(display, window, gc, 200, 200, 100, 100);
    collisionManager.addObject(CollisionObject(200, 200, 100, 100));

    XSetForeground(display, gc, 0xFF0000);
    XDrawRectangle(display, window, gc, 380, 380, 100, 100);
    collisionManager.addObject(CollisionObject(380, 380, 100, 100));

    XSetForeground(display, gc, 0xFF0000);
    XDrawRectangle(display, window, gc, 500, 500, 100, 100);
    collisionManager.addObject(CollisionObject(500, 500, 100, 100));
}
