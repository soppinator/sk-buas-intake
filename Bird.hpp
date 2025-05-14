#pragma once
#include "surface.h"

class Bird
{
public:
	Bird() = default;
	Bird(float x, float y, Tmpl8::Sprite* sprite, float speed);
	~Bird() = default;
	void setY(float y);
	float getY() const;
	void setX(float x);
	float getX() const;
    void setSpeed(float speed);
    float getSpeed() const;
	Tmpl8::Sprite& getSprite();

	void draw(Tmpl8::Surface* surface);
	void gravity(float deltaTime); // bird is constantly pulled down by "gravity"
	void flap(float deltaTime); // user input, when space is pressed, gravity turns negative so the bird flies up


private:
	float x = 100.0f;
	float y = 0.0f;
	float speed = 200.0f;
	float halfspeed = speed / 100.0f;
	Tmpl8::Sprite* sprite = nullptr;
};