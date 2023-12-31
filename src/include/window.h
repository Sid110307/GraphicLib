#pragma once

#include <iostream>
#include <map>
#include <cmath>
#include <any>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "physics.h"

class X11Window
{
public:
    class VariableManager
    {
    public:
        template<typename T>
        void set(const std::string &name, T value)
        {
            variables[name] = value;
        }

        template<typename T>
        T get(const std::string &name)
        {
            auto it = variables.find(name);
            if (it != variables.end())
            {
                try
                {
                    return std::any_cast<T>(it->second);
                } catch (const std::bad_any_cast &)
                {
                    std::cerr << "Bad cast: " << name << std::endl;
                }
            }

            return T();
        }

    private:
        std::map<std::string, std::any> variables;
    };

    static constexpr int WIDTH = 800, HEIGHT = 600;
    static constexpr int PLAYER_WIDTH = 50, PLAYER_HEIGHT = 50;

    explicit X11Window(long inputFlags);
    ~X11Window();

    void draw(int x, int y, int angle);

    Display *display;
    Window window;
    CollisionManager collisionManager;
    VariableManager variableManager;

private:
    int screen;
    GC gc;
    XFontStruct *font;

    void updatePlayer(int x, int y, int angle);
};
