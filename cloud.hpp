#pragma once
#include "surface.h"

class cloud
{
public:
	cloud() = default;
	cloud(float x, float y, Tmpl8::Sprite* cloudSprite, float speed);
	~cloud() = default;
	void setPosition(float x, float y);
	float getX() const;
	float getY() const;
	void setSpeed(float speed);
	float getSpeed() const;
	const Tmpl8::Sprite& getSprite() const;

	void draw(Tmpl8::Surface* surface);
	void move(float deltaTime); 
	void decreaseScore(int& score); // decreases score by 2 if bird is inside cloud in medium difficulty

private:
	float x = 0.0f;
	float y = 0.0f;
	float speed = 300.0f;
	bool counted = false;
	Tmpl8::Sprite* sprite = nullptr;
};