#pragma once
#include "surface.h"

class tree 
{ 
public:
	tree() = default;
	tree(float x, float y, Tmpl8::Sprite* sprite, float speed);
	~tree() = default;
	void setX(float x);
	float getX() const;
	void setY(float y);
	float getY() const;
	void setSpeed(float speed);
	float getSpeed() const;
	const Tmpl8::Sprite& getSprite() const;
	void draw(Tmpl8::Surface* surface);

	void move(float deltaTime);
	void countScore(int& score); // every time the bird successfully flies over a tree, a point is added to the score

private:
	float x = 0.0f;
	float y = 0.0f;
	bool counted = false; // set to true if the score has been applied.
	float speed = 450.0f;
	Tmpl8::Sprite* sprite = nullptr;
};
