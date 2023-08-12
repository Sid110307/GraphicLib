#include "include/application.h"

Application::Application() : x11Window(inputFlags), x(0), y(0), yVelocity(0)
{
    randomizePosition();
    x11Window.draw(x, y);
}

void Application::randomizePosition()
{
    std::mt19937 mt((std::random_device()()));

    x = std::uniform_int_distribution<int>(0, X11Window::WIDTH - 100)(mt);
    y = std::uniform_int_distribution<int>(0, X11Window::HEIGHT - 100)(mt);
}

void Application::updatePhysics()
{
    yVelocity += 1;
    y += yVelocity;

    if (y + 100 > X11Window::HEIGHT)
    {
        y = X11Window::HEIGHT - 100;
        yVelocity = (int) (-yVelocity * (bounceEnabled ? bounceFactor : 0));
    }

    if (x + 100 > X11Window::WIDTH) x = X11Window::WIDTH - 100;
    if (x < 0) x = 0;

    if (y < 0)
    {
        y = 0;
        yVelocity = (int) (-yVelocity * (bounceEnabled ? bounceFactor : 0));
    }

    if (x11Window.collisionManager.checkCollision(CollisionObject(x, y, 100, 100)))
    {
        CollisionObject *object = x11Window.collisionManager.checkCollision(CollisionObject(x, y, 100, 100));

        y = object->y - 100;
        yVelocity = (int) (-yVelocity * (bounceEnabled ? bounceFactor : 0));
    }

    x11Window.draw(x, y);
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
                    x11Window.draw(x, y);
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

                    x11Window.draw(x, y);
                    break;
                case KeyRelease:
                    x11Window.draw(x, y);
                    break;
                default:
                    break;
            }
        }

        if (physicsEnabled) updatePhysics();
        x11Window.variableManager.setVariable("physicsEnabled", physicsEnabled);
        x11Window.variableManager.setVariable("bounceEnabled", bounceEnabled);
        x11Window.variableManager.setVariable("bounceFactor", bounceFactor);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
