#include "include/physics.h"

bool CollisionObject::isColliding(const CollisionObject &other) const
{
    return x < other.x + other.width && x + width > other.x && y < other.y + other.height && y + height > other.y;
}

void CollisionManager::addObject(const CollisionObject &object)
{
    objects.push_back(object);
}

CollisionObject *CollisionManager::checkCollision(const CollisionObject &object)
{
    if (objects.empty()) return nullptr;

    for (auto &other: objects) if (&object != &other && object.isColliding(other)) return &other;
    return nullptr;
}
