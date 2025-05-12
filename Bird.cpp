#include "Bird.hpp"

#include <cassert>

Bird::Bird(float x, float y, Tmpl8::Sprite* sprite, float speed)
{
    this->x = x;
    this->y = y;
    this->sprite = sprite;
}

void Bird::setX(float x)
{
    this->x = x;
}

float Bird::getX() const
{
    return x;
}

void Bird::setY(float y)
{
    this->y = y;
}

float Bird::getY() const
{
    return y;
}

void Bird::setSpeed(float speed)
{
    this->speed = speed;
}

float Bird::getSpeed() const
{
    return speed;
}

Tmpl8::Sprite& Bird::getSprite() 
{
    assert(sprite != nullptr);

    return *sprite;
}

void Bird::draw(Tmpl8::Surface* surface)
{
    sprite->Draw(surface, static_cast<int>(x), static_cast<int>(y));
}

void Bird::gravity(float deltaTime) // bird always pulled down
{
    speed += 140.0f * deltaTime; // add velocity to speed
    y += speed * deltaTime; // add speed to y coord
}

void Bird::flap(float deltaTime) // user hits space to flap wings
{
    speed = -120.0f; // turns speed negative, goes up
}



