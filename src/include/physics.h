#pragma once

#include <vector>

class CollisionObject
{
public:
    int x, y, width, height;

    CollisionObject(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
    [[nodiscard]] bool isColliding(const CollisionObject &other) const;
};

class CollisionManager
{
public:
    void addObject(const CollisionObject &object);
    CollisionObject *checkCollision(const CollisionObject &object);

private:
    std::vector<CollisionObject> objects;
};
