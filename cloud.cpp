#include "cloud.hpp"

#include <cassert>

cloud::cloud(float x, float y, Tmpl8::Sprite* sprite, float speed)
{
    this->x = x;
    this->y = y;
    this->sprite = sprite;
    this->speed = speed;
}

void cloud::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

float cloud::getX() const
{
    return x;
}

float cloud::getY() const
{
    return y;
}

void cloud::setSpeed(float speed)
{
    this->speed = speed;
}


float cloud::getSpeed() const
{
    return speed;
}

const Tmpl8::Sprite& cloud::getSprite() const
{
    assert(sprite);
    return *sprite;
}

void cloud::draw(Tmpl8::Surface* surface)
{
    if (sprite)
    {
        sprite->Draw(surface, static_cast<int>(x), static_cast<int>(y));
    }
}

void cloud::move(float deltaTime)
{
    x -= speed * deltaTime; // subtract velocity from speed to move right to left
    if (x <= -136.0f) // wrap around
    {
        x += 950.0f;
        counted = false; // reset so it can be counted again
    }
}

void cloud::decreaseScore(int& score)
{
    if (!counted)
    {
        if (score <= 2) // make it so you can't get a negative score
            score = 0;
        else // subtract score by 2 ONCE if bird is in cloud
        score -= 2;
        counted = true;
    }
}
