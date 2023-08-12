#include <iostream>
#include <random>
#include <chrono>
#include <thread>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class CollisionObject
{
public:
    int x, y, width, height;
    CollisionObject(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

    [[nodiscard]] bool isColliding(const CollisionObject &other) const
    {
        return x < other.x + other.width && x + width > other.x && y < other.y + other.height && y + height > other.y;
    }
};

class CollisionManager
{
public:
    void addObject(const CollisionObject &object) { objects.push_back(object); }

    bool checkCollision(const CollisionObject &object)
    {
        if (objects.empty()) return false;

        for (const auto &other: objects) return &object != &other && object.isColliding(other);
        return false;
    }

    CollisionObject *getCollidingObject(const CollisionObject &object)
    {
        if (objects.empty()) return nullptr;

        for (auto &other: objects) if (&object != &other && object.isColliding(other)) return &other;
        return nullptr;
    }

private:
    std::vector<CollisionObject> objects;
};

class X11Window
{
public:
    static constexpr int WIDTH = 800, HEIGHT = 600;

    Display *display;
    Window window;
    CollisionManager collisionManager;

    explicit X11Window(long inputFlags)
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

    ~X11Window()
    {
        XFreeGC(display, gc);
        XCloseDisplay(display);
    }

    void draw(int x, int y)
    {
        // Screen
        XSetForeground(display, gc, WhitePixel(display, screen));
        XFillRectangle(display, window, gc, 0, 0, WIDTH, HEIGHT);

        // Help text
        XSetForeground(display, gc, 0x000000);
        XDrawString(display, window, gc, 10, 20, "Press Space to move the rectangle to a random position.", 55);
        XDrawString(display, window, gc, 10, 40, "Press Up, Down, Left, Right to move the rectangle.", 50);
        XDrawString(display, window, gc, 10, 80, "Press Enter to reset the rectangle's position.", 46);
        XDrawString(display, window, gc, 10, 100, "Press P to toggle physics.", 26);
        XDrawString(display, window, gc, 10, 120, "Press ESC to exit.", 18);

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

private:
    int screen;
    GC gc;
    XFontStruct *font;
};

class Application
{
public:
    explicit Application() : x11Window(inputFlags), x(0), y(0), yVelocity(0)
    {
        randomizePosition();
        x11Window.draw(x, y);
    }

    void randomizePosition()
    {
        std::mt19937 mt((std::random_device()()));

        x = std::uniform_int_distribution<int>(0, X11Window::WIDTH - 100)(mt);
        y = std::uniform_int_distribution<int>(0, X11Window::HEIGHT - 100)(mt);
    }

    void run()
    {
        XEvent event;
        while (true)
        {
            while (XCheckWindowEvent(x11Window.display, x11Window.window, inputFlags, &event))
            {
                switch (event.type)
                {
                    case Expose:
                    {
                        x11Window.draw(x, y);
                        break;
                    }
                    case KeyPress:
                        switch (XLookupKeysym(&event.xkey, 0))
                        {
                            case XK_space:
                                randomizePosition();
                                x11Window.draw(x, y);

                                break;
                            case XK_Up:
                                y -= 10;
                                x11Window.draw(x, y);

                                break;
                            case XK_Down:
                                y += 10;
                                x11Window.draw(x, y);

                                break;
                            case XK_Left:
                                x -= 10;
                                x11Window.draw(x, y);

                                break;
                            case XK_Right:
                                x += 10;
                                x11Window.draw(x, y);

                                break;
                            case XK_Return:
                                x = 0;
                                y = 0;

                                x11Window.draw(x, y);
                                break;
                            case XK_p:
                                physicsEnabled = !physicsEnabled;
                                break;
                            case XK_Escape:
                                return;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            if (physicsEnabled) updatePhysics();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void updatePhysics()
    {
        yVelocity += 1;
        y += yVelocity;

        if (y + 100 > X11Window::HEIGHT)
        {
            y = X11Window::HEIGHT - 100;
            yVelocity = (int) (-yVelocity * bounceFactor);
        }

        if (x + 100 > X11Window::WIDTH) x = X11Window::WIDTH - 100;
        if (x < 0) x = 0;

        if (y < 0)
        {
            y = 0;
            yVelocity = (int) (-yVelocity * bounceFactor);
        }

        if (x11Window.collisionManager.checkCollision(CollisionObject(x, y, 100, 100)))
        {
            CollisionObject *object = x11Window.collisionManager.getCollidingObject(CollisionObject(x, y, 100, 100));

            y = object->y - 100;
            yVelocity = (int) (-yVelocity * bounceFactor);
        }

        x11Window.draw(x, y);
    }

private:
    long inputFlags = ExposureMask | KeyPressMask;
    X11Window x11Window;

    int x, y, yVelocity;
    bool physicsEnabled = false;
    double bounceFactor = 0.6;
};

int main()
{
    Application app;
    app.run();

    return EXIT_SUCCESS;
}
