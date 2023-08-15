#include "include/application.h"

Application::Application() : x11Window(inputFlags), x(0), y(0), yVelocity(0)
{
    randomizePosition();
    x11Window.draw(x, y, rotationAngle);
}

void Application::run()
{
    XEvent event;
    while (true)
    {
        while (XCheckWindowEvent(x11Window.display, x11Window.window, inputFlags, &event))
        {
            switch (event.type)
            {
                case Expose:
                    x11Window.draw(x, y, rotationAngle);
                    break;
                case KeyPress:
                    switch (XLookupKeysym(&event.xkey, 0))
                    {
                        case XK_space:
                            randomizePosition();
                            break;
                        case XK_Up:
                            y -= 10;
                            break;
                        case XK_Down:
                            y += 10;
                            break;
                        case XK_Left:
                            x -= 10;
                            break;
                        case XK_Right:
                            x += 10;
                            break;
                        case XK_Return:
                            x = 0;
                            y = 0;
                            rotationAngle = 0;
                            yVelocity = 0;

                            break;
                        case XK_p:
                            physicsEnabled = !physicsEnabled;
                            break;
                        case XK_b:
                            bounceEnabled = !bounceEnabled;
                            break;
                        case XK_Escape:
                            return;
                        default:
                            break;
                    }

                    x11Window.draw(x, y, rotationAngle);
                    break;
                case KeyRelease:
                    x11Window.draw(x, y, rotationAngle);
                    break;
                default:
                    break;
            }
        }

        if (physicsEnabled) updatePhysics();
        x11Window.variableManager.set("physicsEnabled", physicsEnabled);
        x11Window.variableManager.set("bounceEnabled", bounceEnabled);
        x11Window.variableManager.set("maxTiltAngle", maxTiltAngle);
        x11Window.variableManager.set("bounceFactor", bounceFactor);
        x11Window.variableManager.set("yVelocity", yVelocity);
        x11Window.variableManager.set("playerMass", playerMass);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Application::randomizePosition()
{
    std::mt19937 mt((std::random_device()()));

    x = std::uniform_int_distribution<int>(0, X11Window::WIDTH - 100)(mt);
    y = std::uniform_int_distribution<int>(0, X11Window::HEIGHT - 100)(mt);
}

void Application::updatePhysics()
{
    yVelocity += static_cast<int>(std::round(1 * playerMass));
    y += yVelocity;

    if (y + X11Window::PLAYER_HEIGHT > X11Window::HEIGHT)
    {
        y = X11Window::HEIGHT - X11Window::PLAYER_HEIGHT;
        yVelocity = static_cast<int>(-yVelocity * (bounceEnabled ? bounceFactor : 0));
    }
    if (y < 0)
    {
        y = 0;
        yVelocity = static_cast<int>(-yVelocity * (bounceEnabled ? bounceFactor : 0));
    }

    if (x + X11Window::PLAYER_WIDTH > X11Window::WIDTH) x = X11Window::WIDTH - X11Window::PLAYER_WIDTH;
    if (x < 0) x = 0;

    if (rotationAngle > 360) rotationAngle -= 360;
    if (rotationAngle < -360) rotationAngle += 360;

    CollisionObject player(x, y, X11Window::PLAYER_WIDTH, X11Window::PLAYER_HEIGHT, 0, yVelocity, playerMass);
    if (x11Window.collisionManager.checkCollision(player))
    {
        CollisionObject *object = x11Window.collisionManager.checkCollision(player);

        y = object->y - X11Window::PLAYER_HEIGHT;
        yVelocity = static_cast<int>(-yVelocity * (bounceEnabled ? bounceFactor : 0));

        int tiltValue = static_cast<int>(std::clamp(
                std::abs(object->x + object->width / 2 - (x + X11Window::PLAYER_WIDTH / 2)) / 10.0, -maxTiltAngle,
                maxTiltAngle));
        rotationAngle = object->x + object->width / 2 < x + X11Window::PLAYER_WIDTH / 2 ? tiltValue : -tiltValue;
    }

    x11Window.draw(x, y, rotationAngle);
}
