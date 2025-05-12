// SOPHIA K INTAKE ASSIGNMENT FOR BUAS 2025-26
#define NOMINMAX

#include "game.h"
#include "surface.h"
#include "template.h"
#include "windows.h"
#include "tree.hpp"
#include "Bird.hpp"
#include "cloud.hpp"

#include <cstdio> //printf
#include <ctime> // s(rand()) for clouds and trees
#include <string>
#include <iostream>

using namespace std;

int score = 0;
int highScore = 0;

// BIRD GLOBAL VARIABLES
float frame = 0.0f;
float frameRate = 6.0f; // framerate used for wings flapping 
float birdY = 50.0f; 
const float BIRDX = 100.0f; // bird x position and size

// TREES GLOBAL VARIABLES
float treeX = ScreenWidth;
const float LGTREEWIDTH = 136.0f, LGTREELENGTH = 191.0f, MEDTREELENGTH = 141.0f, SHTREELENGTH = 114.0f; // tall tree width + all lengths

// CLOUDS GLOBAL VARIABLES
float cloudY, cloudX = ScreenWidth;
const float  LGCLOUDWIDTH = 164.0f; // lg cloud length + width

// BASE SPEEDS
float birdBaseSpeed = 200.0f, cloudBaseSpeed = 300.0f, treeBaseSpeed = 450.0f; // speed value remains the same throughout the game, only deltaTime is changed

namespace Tmpl8
{
	// ASSETS + SPRITES


	// BIRDS
	Sprite chickadeeSprite(new Surface("assets/chickadee.tga"), 3);
	Sprite ospreySprite(new Surface("assets/osprey.tga"), 3);
	Sprite puffinSprite(new Surface("assets/puffin.tga"), 3);

	Bird chickadee{ BIRDX, birdY, &chickadeeSprite, birdBaseSpeed };
	Bird puffin{ BIRDX, birdY, &puffinSprite, birdBaseSpeed };
	Bird osprey{ BIRDX, birdY, &ospreySprite, birdBaseSpeed };

	Bird birds[4] = {
		chickadee,
		puffin,
		osprey,
		{}
	};

	Bird& myBird = birds[3]; // reference to whatever bird the player chooses

	// TREES
	Sprite treeLg(new Surface("assets/treelg.png"), 1);
	Sprite treeMe(new Surface("assets/treeme.png"), 1);
	Sprite treeSh(new Surface("assets/treesm.png"), 1);

	const tree smallTree{ treeX, ScreenHeight - SHTREELENGTH, &treeSh, treeBaseSpeed };
	const tree mediumTree{ treeX, ScreenHeight - MEDTREELENGTH, &treeMe, treeBaseSpeed };
	const tree largeTree{ treeX, ScreenHeight - LGTREELENGTH, &treeLg, treeBaseSpeed };

	tree trees[5] = { // randomzize first and last, but have pre-set sequence for middle 3
		   {},
		   smallTree,
		   mediumTree,
		   largeTree,
		   {},
	};

	// CLOUDS
	Sprite cloudLg(new Surface("assets/ogcloud1.png"), 1);
	Sprite cloudSm(new Surface("assets/cloud1.png"), 1);

	const cloud smallCloud{ cloudX, cloudY, &cloudLg, cloudBaseSpeed };
	const cloud largeCloud{ cloudX, cloudY, &cloudSm, cloudBaseSpeed };

	cloud clouds[4] = // randomzize first and last, but have pre-set sequence for middle 2
	{
		{},
		smallCloud,
		largeCloud,
		{},
	};

	// SCREENS / BOXES
	Sprite background(new Surface("assets/static-background.png"), 1);
	Sprite fadedBG(new Surface("assets/faded-background.png"), 1);
	Sprite welcomeBox(new Surface("assets/welcome.png"), 1);
	Sprite gameoverBox(new Surface("assets/gameover.png"), 1);
	Sprite scoreCounter(new Surface("assets/score.png"), 1);
	Sprite birdSelection(new Surface("assets/birds.png"), 1);

	void Game::Init()
	{
		highScore = std::max(score, highScore); // updates highscore if new score is higher
		score = 0;
		treeX = ScreenWidth; // make trees and clouds start offscreen 
		cloudX = ScreenWidth + 100.0f;

		// initialize/reset bird
		myBird.setY(birdY);
		myBird.setSpeed(birdBaseSpeed);

		// (re)set keys
		sKey = false;
		space = false;
		oneKey = false;
		twoKey = false;
		threeKey = false;

		srand(static_cast<unsigned int>(time(nullptr))); // generates random number based on current time
		int randInt = rand();

		// Randomize/initiate trees
		int randTree = (randInt % 3);
		switch (randTree)
		{
		case 0:
			trees[0] = smallTree;
			trees[4] = largeTree;
			break;
		case 1:
			trees[0] = mediumTree;
			trees[4] = smallTree;
			break;
		case 2:
			trees[0] = largeTree;
			trees[4] = mediumTree;
			break;
		}

		// Randomize tree offsets.
		for (tree& tree : trees)
		{
			float randNum = Rand(100.0f);
			treeX += (randNum + LGTREEWIDTH);
			tree.setX(treeX); // initializes each tree on a random x value past the screen
		}

		// Randomize/initiate clouds
		int randCloud = (randInt % 4);
		switch (randCloud)
		{
		case 0:
			clouds[0] = largeCloud;
			clouds[3] = largeCloud;
			break;
		case 1:
			clouds[0] = smallCloud;
			clouds[3] = smallCloud;
			break;
		case 2:
			clouds[0] = largeCloud;
			clouds[3] = smallCloud;
			break;
		case 3:
			clouds[0] = smallCloud;
			clouds[3] = largeCloud;
		}

		// Randomize cloud offsets
		for (cloud& cloud : clouds)
		{
			float randNum = Rand(100.0f);
			cloudX += (randNum + LGCLOUDWIDTH);
			cloudY = Rand(ScreenHeight / 3.2f); // clouds will only spawn in the top 1/3 of the screen
			cloud.setPosition(cloudX, cloudY);
		}
	}

	void Game::Shutdown()
	{
	}

	// CROAKINGKERO pixel collision detection
	// Source: https://croakingkero.com/tutorials/pixel_collision_detection/
	bool CheckCollision(const Sprite& a, int ax, int ay, const Sprite& b, int bx, int by) {
		int aleft, aright, abottom, atop;
		int bleft, bright, bbottom, btop;
		int left, right, bottom, top;
		int columns, rows;

		// compute axis line bounding box for sprite a
		aleft = ax;
		aright = aleft + a.GetWidth() - 1; // gets right edge of sprite a
		abottom = ay; 
		atop = abottom + a.GetHeight() - 1; // gets top edge of sprite a

		// compute axis line bounding box for sprite a
		bleft = bx; 
		bright = bleft + b.GetWidth() - 1; 
		bbottom = by; 
		btop = bbottom + b.GetHeight() - 1;

		if (aleft > bright || aright < bleft || abottom > btop || atop < bbottom) // check to see if they overlap at all
			return false; // Collision box areas do not overlap

		// Bounding box of overlap
		left = std::max(aleft, bleft);
		right = std::min(aright, bright);
		bottom = std::max(abottom, bbottom);
		top = std::min(atop, btop);
		columns = right - left + 1;
		rows = top - bottom + 1;

		aleft = left - aleft;
		abottom = bottom - abottom;

		bleft = left - bleft;
		bbottom = bottom - bbottom;

		for (int y = 0; y < rows; ++y) {
			for (int x = 0; x < columns; ++x) {
				unsigned char aa = a.GetBuffer()[(aleft + x) + (abottom + y) * a.GetPitch()] >> 24;
				unsigned char ab = b.GetBuffer()[(bleft + x) + (bbottom + y) * b.GetPitch()] >> 24; // isolates alpha channel 

				if (aa > 127 && ab > 127) // compare masks, if any pixels are opaque, it detects a collision
					return true;
			}
		}

		return false;
	}

	bool CheckCollision(const Sprite& a, float ax, float ay, const Sprite& b, float bx, float by) // conversion from floats -> int
	{
		return CheckCollision(a, static_cast<int>(ax), static_cast<int>(ay), b, static_cast<int>(bx), static_cast<int>(by));
	}

	void Game::Tick(float deltaTime)
	{
		deltaTime /= 1000.0f;
		if (pause) 
		{
			deltaTime = 0.0f; // freeze game
		}
		switch (state)
		{
		case State::Welcome: // game states
			DoWelcome(deltaTime);
			break;
		case State::SelectBird:
			DoSelectBird(deltaTime);
			break;
		case State::Running:
			DoRunning(deltaTime);
			break;
		case State::GameOver:
			DoGameOver(deltaTime);
			break;
		}
	}
	
	void Game::DoWelcome(float deltaTime) // WELCOME SCREEN
	{
		fadedBG.Draw(screen, 0, 0);
		welcomeBox.Draw(screen, 170, 100);


		if (sKey)
		{
			state = State::SelectBird;
			screen->Clear(0);
			sKey = false;
		}
	}

	void Game::DoSelectBird(float deltaTime) // BIRD SELECTION SCREEN
	{
		// draw bg and bird selection screen
		fadedBG.Draw(screen, 0, 0);
		birdSelection.Draw(screen, 0, 0);
		// print indicators for difficulty selection
		screen->PrintScaled("PRESS 1 TO SELECT", 155, 165, 1, 1, 0x456829);
		screen->PrintScaled("PRESS 2 TO SELECT", 350, 170, 1, 1, 0x0D5C47);
		screen->PrintScaled("PRESS 3 TO SELECT", 550, 165, 1, 1, 0x8C2D36);

		if (oneKey) // set difficulty and bird type based on number pressed
		{
			difficulty = 1;
			birds[3] = chickadee;
			birdChosen = true;
			oneKey = false;
		}

		if (twoKey)
		{
			difficulty = 2;
			birds[3] = puffin;
			birdChosen = true;
			twoKey = false;
		}

		if (threeKey)
		{
			difficulty = 3;
			birds[3] = osprey;
			birdChosen = true;
			threeKey = false;
		}

		if (difficulty == 1) // indicate a bird has been selected
			screen->PrintScaled("SELECTED", 130, 340, 3, 3, 0x589400);
		if (difficulty == 2)
			screen->PrintScaled("SELECTED", 330, 340, 3, 3, 0x018F9C);
		if (difficulty == 3)
			screen->PrintScaled("SELECTED", 530, 340, 3, 3, 0xA4054F);
		if (birdChosen) // once selected, ask to confirm
			screen->PrintScaled("PRESS S TO CONFIRM", 300, 485, 2, 2, 0x654b3d);
		if (sKey && birdChosen) // once confirmed, run game
		{
			state = State::Running;
			screen->Clear(0);
			sKey = false;
		}
	}

	void Game::DoRunning(float deltaTime)
	{ 

		if (difficulty == 2) // change speed of game based on difficulty chosen
		{
			deltaTime = deltaTime * 1.5f;
		}
		if (difficulty == 3)
		{
			deltaTime = deltaTime * 2.75f;
		}
		// animate bird
		Sprite& mySprite = myBird.getSprite();
		frame += deltaTime * frameRate;
		frame = std::fmod(frame, 3.0f);
		mySprite.SetFrame(static_cast<int>(frame));

		// UPDATE GAME STATE
		
		if ((myBird.getY() > ScreenHeight) || (myBird.getY() < -50.0f)) // game over if bird hits ground or goes above screen
			state = State::GameOver;

		bool collision = false;
		for (cloud& cloud : clouds) // check for collision happening to ANY cloud
		{
			if (CheckCollision(myBird.getSprite(), myBird.getX(), myBird.getY(), cloud.getSprite(), cloud.getX(), cloud.getY())) // per pixel collision check between bird and cloud
			{
				collision = true;
				if (difficulty == 2) // in medium mode, bird hitting cloud removes a point
					cloud.decreaseScore(&score);
				break;
			}
		}
		if (collision)
		{
			if (difficulty == 3) // in hard mode, bird hitting cloud = gameover
			{
				state = State::GameOver;
				return;
			}
			if (difficulty <= 2) // if in easy or medium mode, everything slows down while bird is in cloud
				deltaTime = deltaTime / 2.0f; 
		}
		for (cloud& cloud : clouds) // move clouds
		{
			cloud.setSpeed(cloudBaseSpeed);
			cloud.move(deltaTime);
		}
		
		// spawn trees 
		for (tree& tree : trees)
		{
			tree.setSpeed(treeBaseSpeed);
			tree.move(deltaTime);
			tree.countScore(&score); // adds 1 to score if the bird passes a tree

			if (CheckCollision(myBird.getSprite(), myBird.getX(), myBird.getY(), tree.getSprite(), tree.getX(), tree.getY()))
				state = State::GameOver; // if bird hits tree; gameover
		}

		myBird.gravity(deltaTime); // bird is always being pulled down by gravity

 	  // PLAYER INPUT (space) makes bird fly up
		if (space)
		{
			myBird.flap(deltaTime);
			space = false;
		}

		// DRAW FUNCTIONS
		background.Draw(screen, 0, 0);
		
		for (tree& tree : trees)
		{
			tree.draw(screen);
		}

		myBird.draw(screen);

		for (cloud& cloud : clouds)
		{
			cloud.draw(screen);
		}

		// controls and score box
		screen->PrintScaled("PRESS SPACE TO FLAP", 300, 485, 2, 2, 0xcf993e);
		screen->PrintScaled("PRESS P TO PAUSE OR ESC TO EXIT", 600, 10, 1, 1, 0xB5E6F1);
		scoreCounter.Draw(screen, 0, 0);
		std::string scoreStr = std::to_string(score); // take score and convert to string
		screen->PrintScaled(scoreStr.c_str(), 90, 12, 3, 3, 0x5E6C44); // print score as string

	}

	void Game::DoGameOver(float deltaTime)
	{
		fadedBG.Draw(screen, 0, 0);
		gameoverBox.Draw(screen, 170, 100);

		std::string scoreStr = std::to_string(score); // take score and convert to string
		screen->PrintScaled(scoreStr.c_str(), 300, 235, 3, 3, 0x79354d); // print score as string

		std::string highScoreStr = std::to_string(highScore); // take high score and convert to string
		screen->PrintScaled(highScoreStr.c_str(), 360, 275, 3, 3, 0x79354d); // print highscore as string

		if (highScore < score)
			screen->PrintScaled("NEW HIGH SCORE!", 240, 200, 3, 3, 0x79354d);

		if (sKey) // instantly restarts game
		{
			Init();
			state = State::Running;
			sKey = false;
		}
		if (rKey) // returns to bird selection screen
		{
			Init();
			state = State::SelectBird;
			rKey = false;
		}
	}
};
