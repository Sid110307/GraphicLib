#pragma once

#include <vector>

class PhysicsObject
{
public:
    int x, y, width, height, xVelocity, yVelocity;
    double mass;

    PhysicsObject(int x, int y, int width, int height, int xVelocity, int yVelocity, double mass)
            : x(x), y(y), width(width), height(height), xVelocity(xVelocity), yVelocity(yVelocity), mass(mass) {}
};

class CollisionObject : public PhysicsObject
{
public:
    CollisionObject(int x, int y, int width, int height, int xVelocity = 0, int yVelocity = 0, double mass = 1.0)
            : PhysicsObject(x, y, width, height, xVelocity, yVelocity, mass) {}
    [[nodiscard]] bool isColliding(const CollisionObject &other) const;

    bool operator==(const CollisionObject &other) const { return isColliding(other); }
    bool operator!=(const CollisionObject &other) const { return !isColliding(other); }
};

class CollisionManager
{
public:
    void addObject(const CollisionObject &object);
    CollisionObject *checkCollision(const CollisionObject &object);

private:
    std::vector<CollisionObject> objects;
};
