#include "tree.hpp"

tree::tree(float x, float y, Tmpl8::Sprite* sprite, float speed)
{
    this->x = x;
    this->y = y;
    this->sprite = sprite;
}
void tree::setX(float x)
{
    this->x = x;
}

float tree::getX() const
{
    return x;
}

void tree::setY(float y)
{
    this->y = y;
}

float tree::getY() const
{
    return y;
}

void tree::setSpeed(float speed)
{
    this->speed = speed; // speed += 0.1
}

float tree::getSpeed() const
{
    return speed;
}

const Tmpl8::Sprite& tree::getSprite() const
{
    return *sprite;
}

void tree::draw(Tmpl8::Surface* surface)
{
    if (sprite)
    {
        sprite->Draw(surface, static_cast<int>(x), static_cast<int>(y));
    }
}

void tree::move(float deltaTime)
{
    x -= speed * deltaTime;
    if (x <= -136.0f) // wraparound
    {
        x += 950.0f;
        counted = false; // resets so it can be reused
    }
}

void tree::countScore(int& score)
{
    if (x < 101.0f && !counted) // applies if tree passes by bird and only counts once
    {
        score += 1;
        counted = true;
    }
}
