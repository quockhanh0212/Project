
#ifndef GAME_H_
#define GAME_H_

#define WINDOW_WIDTH 1260
#define WINDOW_HEIGHT 650

#define EXPLODE_FRAME_WIDTH 250
#define EXPLODE_FRAME_HEIGHT 220
#define EXPLODE_WIDTH 25
#define EXPLODE_HEIGHT 22

#define SIDE 16

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>

#include "Background.h"
#include "Board.h"
#include "Paddle.h"
#include "Ball.h"
#include "item.h"


#define FPS_DELAY 15
#define SCORE_PER_BRICK 1

const int ITEM_FREQ = 2000;
const int COUNT_DOWN = 1000;


class Game {
public:
    Game();
    ~Game();

    bool Init();
    void Run();

    //Game statistics
    std::vector<Item*> Items;
    int LifeCount ;
    int GameScore;
    int HighScore;
    int hit_times;
    int level;
    bool onetime_1;
    bool onetime_2;
    bool onetime_3;

    SDL_Rect frame_clip [3];
    int explosion_frame;
    void set_clips ();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* texture;

    // Timing
    unsigned int last_tick, fps_tick, fps, framecount;

    // Test
    float testx, testy;

    Background* background;
    Board* board;
    Paddle* paddle;
    Ball* ball;
    Item* item;

    bool paddlestick;

    void Clean();
    void Update(float delta);
    void Render(float delta);

    void GameMenu();
    void NewGame();
    void Gameover();

    void ResetPaddle();
    void ResetBall();

    void SetPaddleX(float x);
    void isBoardCollides();
    float GetReflection(float hitx);
    void isPaddleCollides();
    void isBrickCollides();
    void isBrickCollides2();
    void BallBrickResponse(int dirindex);
    int BrickCount();

    void ScoreUpdate(int score, int highscore, int hit);

    int delay = ITEM_FREQ;
    int countDownBall = COUNT_DOWN;
    int countDownPaddle = COUNT_DOWN;
    void raiseItem();

    bool isPowerUp(Item* item);
    void powerUpChange(Item* item);
};

#endif
