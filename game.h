#pragma once
#include <SDL_events.h>
#include "Bird.hpp"
#include "cloud.hpp"
#include "tree.hpp"

namespace Tmpl8 {

    class Surface;
    class Game
    {
    public:
        enum class State // game states
        {
            Welcome,
            SelectBird,
            Running,
            GameOver,
            Pause,
        };

        void SetTarget(Surface* surface) { screen = surface; }
        void Init();
        void Shutdown();
        void Tick(float deltaTime);

        void KeyDown(SDL_KeyboardEvent key) // key input
        {
            switch (key.keysym.sym)
            {
            case SDLK_SPACE:
                space = true;
                break;
            case SDLK_s:
                sKey = true;
                break;
            case SDLK_r:
                rKey = true;
                break;
            case SDLK_p:
                pause = !pause;
                break;
            case SDLK_1:
                oneKey = true;
                break;
            case SDLK_2:
                twoKey = true;
                break;
            case SDLK_3:
                threeKey = true;
                break;
            }
        }

    private:
        // game state functions
        void DoWelcome(float deltaTime);
        void DoSelectBird(float deltaTime);
        void DoRunning(float deltaTime);
        void DoGameOver(float deltaTime);
        
        Surface* screen;
        bool space = false;
        bool sKey = false;
        bool rKey = false;
        bool pause = false;
        bool oneKey = false;
        bool twoKey = false;
        bool threeKey = false;

        State state = State::Welcome; // game always starts at the welcome screen
        bool birdChosen = false;
        int difficulty = 0;
    };

}; 