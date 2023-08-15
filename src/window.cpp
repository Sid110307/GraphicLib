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

void X11Window::draw(int x, int y, int angle)
{
    // Screen
    XSetForeground(display, gc, WhitePixel(display, screen));
    XFillRectangle(display, window, gc, 0, 0, WIDTH, HEIGHT);

    // Help text
    XSetForeground(display, gc, 0x000000);
    XDrawString(display, window, gc, 10, 20, "Press ESC to exit.", 18);
    XDrawString(display, window, gc, 10, 40, "Press Space to move the rectangle to a random position.", 55);
    XDrawString(display, window, gc, 10, 60, "Press Up, Down, Left, Right to move the rectangle.", 50);
    XDrawString(display, window, gc, 10, 80, "Press Enter to reset the rectangle.", 35);
    XDrawString(display, window, gc, 10, 100, "Press P to toggle physics.", 26);
    XDrawString(display, window, gc, 10, 120, "Press B to toggle bounce.", 25);

    // Variables
    std::string physicsEnabledStr =
            "Physics: " + std::string(variableManager.get<bool>("physicsEnabled") ? "yes" : "no");
    std::string bounceEnabledStr = "Bounce: " + std::string(variableManager.get<bool>("bounceEnabled") ? "yes" : "no");
    std::string maxTiltAngleStr = "Max tilt angle: " + std::to_string(variableManager.get<double>("maxTiltAngle"));
    std::string bounceFactorStr = "Bounce factor: " + std::to_string(variableManager.get<double>("bounceFactor"));
    std::string positionStr = "Position: (" + std::to_string(x) + ", " + std::to_string(y) + ")";
    std::string angleStr = "Angle: " + std::to_string(angle);
    std::string yVelocityStr = "Y velocity: " + std::to_string(variableManager.get<int>("yVelocity"));
    std::string playerMassStr = "Player mass: " + std::to_string(variableManager.get<double>("playerMass"));

    XDrawString(display, window, gc, WIDTH - 200, 20, physicsEnabledStr.c_str(),
                static_cast<int>(physicsEnabledStr.length()));
    XDrawString(display, window, gc, WIDTH - 200, 40, bounceEnabledStr.c_str(),
                static_cast<int>(bounceEnabledStr.length()));
    XDrawString(display, window, gc, WIDTH - 200, 60, maxTiltAngleStr.c_str(),
                static_cast<int>(maxTiltAngleStr.length()));
    XDrawString(display, window, gc, WIDTH - 200, 80, bounceFactorStr.c_str(),
                static_cast<int>(bounceFactorStr.length()));
    XDrawString(display, window, gc, WIDTH - 200, 100, positionStr.c_str(), static_cast<int>(positionStr.length()));
    XDrawString(display, window, gc, WIDTH - 200, 120, angleStr.c_str(), static_cast<int>(angleStr.length()));
    XDrawString(display, window, gc, WIDTH - 200, 140, yVelocityStr.c_str(), static_cast<int>(yVelocityStr.length()));
    XDrawString(display, window, gc, WIDTH - 200, 160, playerMassStr.c_str(), static_cast<int>(playerMassStr.length()));

    updatePlayer(x, y, angle);

    // Objects
    XSetForeground(display, gc, 0xFF0000);
    XDrawRectangle(display, window, gc, 200, 200, 100, 100);
    collisionManager.addObject(CollisionObject(200, 200, 100, 100));

    XSetForeground(display, gc, 0xFF0000);
    XDrawRectangle(display, window, gc, 350, 350, 100, 100);
    collisionManager.addObject(CollisionObject(350, 350, 100, 100));

    XSetForeground(display, gc, 0xFF0000);
    XDrawRectangle(display, window, gc, 500, 500, 100, 100);
    collisionManager.addObject(CollisionObject(500, 500, 100, 100));
}

void X11Window::updatePlayer(int x, int y, int angle)
{
    XPoint points[4] = {
            {static_cast<short>(x),                static_cast<short>(y)},
            {static_cast<short>(x + PLAYER_WIDTH), static_cast<short>(y)},
            {static_cast<short>(x + PLAYER_WIDTH), static_cast<short>(y + PLAYER_HEIGHT)},
            {static_cast<short>(x),                static_cast<short>(y + PLAYER_HEIGHT)}
    };

    for (auto &point: points)
    {
        double rad = angle * M_PI / 180;
        double x1 = point.x - x, y1 = point.y - y;

        point.x = static_cast<short>(x1 * cos(rad) - y1 * sin(rad) + x);
        point.y = static_cast<short>(x1 * sin(rad) + y1 * cos(rad) + y);
    }

    XSetForeground(display, gc, 0x0000FF);
    XFillPolygon(display, window, gc, points, 4, Convex, CoordModeOrigin);
}
