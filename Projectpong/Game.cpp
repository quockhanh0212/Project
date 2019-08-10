
#include "Game.h"

Mix_Music *music = NULL;
Mix_Chunk *hit_sound = NULL;
Mix_Chunk *levelup = NULL;
Mix_Chunk *brickhit = NULL;
Mix_Chunk *bighit = NULL;
Mix_Chunk *welldone = NULL;

Game::Game()
{
    window = 0;
    renderer = 0;

    srand(time(0));
}

Game::~Game()
{

}

bool Game::Init()
{
    bool success = true;
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("BreakOut",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std :: cout << "Error creating window:" << SDL_GetError() << std :: endl;
        success = false;
    }


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Object::setRenderer(renderer);
    if (!renderer)
    {
        std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
        success = false;
    }

    if (TTF_Init() < 0)
    {
        std::cout << "TTF could not initialize!" << SDL_GetError() << std::endl;
        success = false;
    }

    if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 )
    {
        std::cout << "Mixer could not initialize!" << SDL_GetError() << std::endl;
        success = false;
    }
    Item::loadTexture();


    last_tick = SDL_GetTicks();
    fps_tick = last_tick;
    fps = 0;
    framecount = 0;

    //Sound load
    music = Mix_LoadMUS("Sound//awesomeness.wav");
    if (music == NULL)
        std::cout << "Error loading music: " << SDL_GetError() << std:: endl;

    hit_sound = Mix_LoadWAV("Sound//hit.wav");
    if (hit_sound == NULL )
        std::cout << "Error loading hit sound: " << SDL_GetError() << std:: endl;

    levelup = Mix_LoadWAV("Sound//levelup.wav");
    if (levelup == NULL )
        std::cout << "Error loading hit sound: " << SDL_GetError() << std:: endl;

    brickhit = Mix_LoadWAV("Sound//brickhit.wav");
    if (brickhit == NULL )
        std::cout << "Error loading hit sound: " << SDL_GetError() << std:: endl;

    bighit = Mix_LoadWAV("Sound//bighit.mp3");
    if (bighit == NULL )
        std::cout << "Error loading hit sound: " << SDL_GetError() << std:: endl;

    welldone = Mix_LoadWAV("Sound//WellDone.wav");
    if (welldone == NULL )
        std::cout << "Error loading hit sound: " << SDL_GetError() << std:: endl;

    return success;
}
void Game::Clean()
{
    // Clean resources
    SDL_DestroyTexture(texture);

    // Clean renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(hit_sound);
    Mix_FreeMusic(music);
    Mix_FreeChunk(welldone);
    Mix_FreeChunk(levelup);
    Mix_FreeChunk(brickhit);
    Mix_FreeChunk(bighit);
}

void Game::Run()
{
    //framesStart = SDL_GetTicks();
    GameMenu();
    //Sound();
    background = new Background (renderer);
    board = new Board(renderer);
    paddle = new Paddle(renderer);

    Ball* ball = new Ball(renderer);
    Balls.push_back(ball);
    NewGame();



    // Main loop
    while (1)
    {

        // Handler events
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                break;
            }
        }

        // Calculate delta and fps
        unsigned int current_tick = SDL_GetTicks();
        delta = (current_tick - last_tick) / 1000.0f;

        if (current_tick - fps_tick >= FPS_DELAY)
        {
            fps = framecount * (1000.0f / (current_tick - fps_tick));
            fps_tick = current_tick;
            framecount = 0;

            char GameTitle[50];
            snprintf(GameTitle, 100, "BreakOut (fps: %u)", fps);
            SDL_SetWindowTitle(window, GameTitle);
        }
        else
        {
            framecount++;
        }
        last_tick = current_tick;

        //In highscore
        std::ifstream HighScoreIn("Highscore//highscore.txt");
        if (!HighScoreIn.is_open())
        {
            printf("Unable to load high score file! Setting high score to 0\n");
            HighScore = 0;
        }
        else
        {
            HighScoreIn >> HighScore;
            //std::cout << HighScore;
        }

        // Update and render the game
        Update(delta);

        if (GameScore > HighScore)
        {
            std::ofstream HighScoreOut("highscore.txt");
            HighScore = GameScore;
            HighScoreOut << HighScore;
            //std::cout << HighScore;
        }

        Render(delta);

        //Gameover
        if (BrickCount() == 0 || LifeCount == 0)
        {
            Gameover();
            GameMenu();
            NewGame();

        }

    }
    delete paddle;
    delete background;
    delete board;
    Balls.erase(Balls.begin());

    for (int i = 0; i<Items.size(); i++)
    {
        delete Items[i];
    }


    Clean();

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    Mix_CloseAudio();
}


void Game::GameMenu()
{
    bool success = true;
    if (Mix_PlayingMusic() == 0 )
        Mix_PlayMusic(music, -1);
    SDL_Surface* gSurface = IMG_Load("PNG\\menu.jpg");
    texture = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    SDL_Rect dRect;

    dRect.x = 0;
    dRect.y = 0;
    dRect.w = WINDOW_WIDTH;
    dRect.h = WINDOW_HEIGHT;
    SDL_RenderCopy(renderer, texture, 0, &dRect);

    SDL_Texture* BreakOut = NULL;
    SDL_Texture* wStart = NULL;
    SDL_Texture* rStart = NULL;
    SDL_Texture* wExit = NULL;
    SDL_Texture* rExit = NULL;
    TTF_Font* TittleFont = NULL;
    TTF_Font* GameFont = NULL;


    TittleFont = TTF_OpenFont("font//pdark.ttf",800);
    if (TittleFont == NULL)
        std::cout << "Error load font: " << SDL_GetError() << std::endl;

    GameFont = TTF_OpenFont("font//neutro.ttf", 800);
    if (GameFont == NULL)
        std::cout << "Error load font: " << SDL_GetError() << std::endl;

    SDL_Color WHITE = {255,255,255,255};
    SDL_Color RED = {255,0,0,0};

    std :: string game_text = "BreakOut";
    gSurface = TTF_RenderText_Solid(TittleFont, game_text.c_str(), WHITE );
    BreakOut = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);


    dRect.x = WINDOW_WIDTH/2 - 250;
    dRect.y = 15;
    dRect.w = 500;
    dRect.h = 190;
    SDL_RenderCopy(renderer, BreakOut, 0, &dRect);

    std:: string start_text = "START";
    gSurface = TTF_RenderText_Solid(GameFont, start_text.c_str(), WHITE);
    wStart = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    gSurface = TTF_RenderText_Solid(GameFont, start_text.c_str(), RED);
    rStart = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    SDL_Rect StartRect;
    StartRect.x = WINDOW_WIDTH/2 -100;
    StartRect.y = 300;
    StartRect.w = 200;
    StartRect.h = 100;
    SDL_RenderCopy(renderer, rStart, 0, &StartRect);

    std:: string exit_text = "EXIT";
    gSurface = TTF_RenderText_Solid(GameFont, exit_text.c_str(), WHITE);
    wExit = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    gSurface = TTF_RenderText_Solid(GameFont, exit_text.c_str(), RED);
    rExit = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    SDL_Rect ExitRect;
    ExitRect.x = WINDOW_WIDTH/2 -176/2;
    ExitRect.y = 450;
    ExitRect.w = 176;
    ExitRect.h = 95;
    SDL_RenderCopy(renderer, wExit, 0, &ExitRect);

    SDL_RenderPresent(renderer);


    while (success)
    {
        SDL_Event g_event;

        while (SDL_PollEvent(&g_event))
        {
            if (g_event.type == SDL_QUIT)
            {
                SDL_DestroyWindow(window);
                SDL_Quit();
                exit(0);
                break;
            }
            else if (g_event.type == SDL_KEYDOWN)
            {
                switch (g_event.key.keysym.sym)
                {
                case SDLK_UP:
                    SDL_RenderCopy(renderer, wExit, 0, &ExitRect);
                    SDL_RenderCopy(renderer, rStart, 0, &StartRect);
                    SDL_RenderPresent(renderer);
                    start = 1;
                    break;
                case SDLK_DOWN:
                    SDL_RenderCopy(renderer, rExit, 0, &ExitRect);
                    SDL_RenderCopy(renderer, wStart, 0, &StartRect);
                    SDL_RenderPresent(renderer);
                    start = 2;
                    break;
                case SDLK_RETURN:
                    if ( start == 1)
                        success = false;
                    else if ( start == 2 )
                    {
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        exit(0);
                    }
                    else
                        continue;
                    break;
                }
            }
        }
    }

    SDL_DestroyTexture(wStart);
    SDL_DestroyTexture(BreakOut);
    SDL_DestroyTexture(wExit);
    SDL_DestroyTexture(rStart);
    SDL_DestroyTexture(rExit);
    TTF_CloseFont(TittleFont);
    TTF_CloseFont(GameFont);
    Mix_HaltMusic();

}

void Game::NewGame()
{
    paddle->frame = 0;
    explosion_frame = 0;
    LifeCount = 3;
    GameScore = 0;
    hit_times = 0;
    level = 0;
    onetime_1 = true;
    onetime_2 = true;
    onetime_3 = true;

    board->CreateLevel();
    ResetPaddle();
}

int Game::BrickCount()
{
    int brickcount = 0;
    for (int i=0; i<BRICK_PER_ROW; i++)
    {
        for (int j=0; j<BRICK_PER_COL; j++)
        {
            Brick brick = board->bricks[i][j];
            if (brick.state)
            {
                brickcount++;
            }
        }
    }

    return brickcount;
}

void Game :: Gameover()
{
    if ( Mix_PlayChannel(-1,welldone,0) == -1)
        std:: cout << "Error playing gameover sound: " << Mix_GetError << std::endl;
    SDL_Surface* gSurface= IMG_Load("PNG//gameover.png");
    SDL_Texture* gameovertexture = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    SDL_Rect drect;
    drect.x = WINDOWGAME_WIDTH/2 - GAMEOVER_WIDTH/2;
    drect.y = WINDOWGAME_HEIGHT/2 - GAMEOVER_HEIGHT/2 -100;
    drect.w = GAMEOVER_WIDTH;
    drect.h = GAMEOVER_HEIGHT;
    SDL_RenderCopy(renderer, gameovertexture, 0, &drect);

    TTF_Font* text = TTF_OpenFont("font//arcade.ttf", 60);
    if (text == NULL)
    {
        std::cout << "Error load font: " << SDL_GetError() << std::endl;
    }
    std:: string Game = "Press Space to replay";
    SDL_Color ORANGE = {255,128,0};

    gSurface = TTF_RenderText_Solid(text, Game.c_str(),ORANGE);
    SDL_Texture* gTexture = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    SDL_Rect gRect;
    gRect.x = WINDOWGAME_WIDTH/2 - 630/2;
    gRect.y = 400;
    gRect.w = 630;
    gRect.h = 60;
    SDL_RenderCopy(renderer, gTexture, 0, &gRect);

    SDL_RenderPresent(renderer);
    bool success = true;
    while (success)
    {
        SDL_RenderClear(renderer);
        SDL_Event g_event;
        while (SDL_PollEvent(&g_event))
        {
            if (g_event.type == SDL_QUIT)
            {
                SDL_DestroyWindow(window);
                SDL_Quit();
                exit(0);
                break;
            }
            else if (g_event.key.keysym.sym == SDLK_SPACE)
            {
                success = false;
                break;
            }
        }
    }
    SDL_DestroyTexture(gameovertexture);
    SDL_DestroyTexture(gTexture);
    TTF_CloseFont(text);
}


void Game::ResetPaddle()
{
    paddlestick = true;
    ResetBall(Balls[0]);
    //ResetBullet(bullet);
    hit_times = 0;
}

void Game::ResetBall(Ball* ball)
{
    ball->x = paddle->x + paddle->width/2 - ball->width/2;
    ball->y = paddle->y - ball->height;
}

void Game::Update(float delta)
{

    for (int i = 0; i < Items.size(); i++)
    {
        Items[i]->Update(delta);
        powerUpChange(Items[i]);
    }
    raiseItem();


    int mouseX, mouseY;
    Uint8 mouse_state = SDL_GetMouseState(&mouseX, &mouseY);
    SetPaddleX(mouseX - paddle->width/2.0f);

    if (mouse_state&SDL_BUTTON(1))     // Click left-mouse to start
    {
        if (paddlestick)
        {
            paddlestick = false;
            //Balls[0]->SetDirection(1, 1);
            for (int i = 0; i<Balls.size(); i++)
            {
                Balls[i]->SetDirection(1, 1);
            }
        }
    }

    if (paddlestick)
    {
        ResetBall(Balls[0]);
    }

    isBoardCollides();
    isPaddleCollides();
    isBrickCollides();

    board->Update(delta);
    paddle->Update(delta);


    if (!paddlestick)
        for (int i = 0; i<Balls.size(); i++)
        {
            Balls[i]->Update(delta);
        }

}

void Game::SetPaddleX(float x)
{
    float newx;
    if (x < board->x)
    {
        // Upper bound
        newx = board->x;
    }
    else if (x + paddle->width > board->x + board->width)
    {
        // Lower bound
        newx = board->x + board->width - paddle->width;
    }
    else
    {
        newx = x;
    }
    paddle->x = newx;
}

void Game::isBoardCollides()
{
    // Top and bottom collisions
    for (int i = 0; i<Balls.size(); i++)
    {
        if (Balls[i]->y < board->y)
        {
            // Top
            Balls[i]->y = board->y;
            Balls[i]->diry *= -1;
        }
        else if (Balls[i]->y > board->y + board->height)
        {
            // Bottom
            Balls.erase(Balls.begin()+i);
            // Ball lost
            if (Balls.size() == 0)
            {
                LifeCount--;
                board -> hearts[LifeCount].state = false;
                ResetPaddle();
                Balls.push_back(new Ball(renderer));
            }
        }
    }

    // Left and right collision
    for (int i = 0; i<Balls.size(); i++)
    {
        if (Balls[i]->x <= board->x)
        {
            // Left
            Balls[i]->x = board->x;
            Balls[i]->dirx *= -1;
        }
        else if (Balls[i]->x + Balls[i]->width >= board->x + board->width)
        {
            // Right
            Balls[i]->x = board->x + board->width - Balls[i]->width;
            Balls[i]->dirx *= -1;
        }
    }
}

float Game::GetReflection(float hitx)
{
    //  hitx within the width of the paddle
    if (hitx < 0)
    {
        hitx = 0;
    }
    else if (hitx > paddle->width)
    {
        hitx = paddle->width;
    }

    // Everything to the left of the center of the paddle is reflected to the left
    // while everything right of the center is reflected to the right
    hitx -= paddle->width / 2.0f;

    // Scale the reflection ranging from -2.0f to 2.0f
    return 2.0f * (hitx / (paddle->width / 2.0f));
}


void Game::isPaddleCollides()
{

    for (int i = 0; i<Balls.size(); i++)
    {
        // Get the center x-coordinate of the ball
        float ballcenterx = Balls[i]->x + Balls[i]->width / 2.0f;

        // Check paddle collision
        if (Balls[i]->IsCollides(paddle))
        {
            hit_times = 0;
            Balls[i]->y = paddle->y - Balls[i]->height;
            Balls[i]->SetDirection(GetReflection(ballcenterx - paddle->x), -1);
            if ( Mix_PlayChannel(-1,hit_sound,0) == -1)
                std:: cout << "Error playing sound: " << Mix_GetError << std::endl;
            //ball->SetDirection(0, -1);
        }
    }

}

void Game::isBrickCollides()
{
    for (int k = 0; k<Balls.size(); k++)
    {
        for (int i=0; i<BRICK_PER_ROW; i++)
        {
            for (int j=0; j<BRICK_PER_COL; j++)
            {
                Brick brick = board->bricks[i][j];

                // Check brick's state
                if (brick.state)
                {
                    // Brick x and y coordinates
                    float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                    float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                    // Center of the ball x and y coordinates
                    float ballcenterx = Balls[k]->x + 0.5f*Balls[k]->width;
                    float ballcentery = Balls[k]->y + 0.5f*Balls[k]->height;

                    // Center of the brick x and y coordinates
                    float brickcenterx = brickx + 0.5f*BOARD_BRWIDTH;
                    float brickcentery = bricky + 0.5f*BOARD_BRHEIGHT;

                    if (Balls[k]->x <= brickx + BOARD_BRWIDTH && Balls[k]->x+Balls[k]->width >= brickx
                            && Balls[k]->y <= bricky + BOARD_BRHEIGHT && Balls[k]->y + Balls[k]->height >= bricky)
                    {
                        // // Collision confirmed, delete bricks

                        if (Balls[k]->isBigBall())
                        {
                            if ( Mix_PlayChannel(-1,bighit,0) == -1)
                                std:: cout << "Error playing brick sound: " << Mix_GetError << std::endl;
                            board -> bricks[i][j].status-= 2;
                            hit_times++;
                        }
                        else
                        {
                            if ( Mix_PlayChannel(-1,brickhit,0) == -1)
                                std:: cout << "Error playing brick sound: " << Mix_GetError << std::endl;
                            board -> bricks[i][j].status--;
                            hit_times++;
                        }

                        if ( board -> bricks[i][j].status <= 0 )
                        {
                            board->bricks[i][j].state = false;
                            GameScore += hit_times * SCORE_PER_BRICK;

                        }


                        // Asume the ball goes slow enough to not skip through the bricks

                        // Calculate ysize
                        float ymin = 0;
                        if (bricky > Balls[k]->y)
                        {
                            ymin = bricky;
                        }
                        else
                        {
                            ymin = Balls[k]->y;
                        }

                        float ymax = 0;
                        if (bricky+BOARD_BRHEIGHT < Balls[k]->y+Balls[k]->height)
                        {
                            ymax = bricky+BOARD_BRHEIGHT;
                        }
                        else
                        {
                            ymax = Balls[k]->y+Balls[k]->height;
                        }

                        float ysize = ymax - ymin;

                        // Calculate xsize
                        float xmin = 0;
                        if (brickx > Balls[k]->x)
                        {
                            xmin = brickx;
                        }
                        else
                        {
                            xmin = Balls[k]->x;
                        }

                        float xmax = 0;
                        if (brickx+BOARD_BRWIDTH < Balls[k]->x+Balls[k]->width)
                        {
                            xmax = brickx+BOARD_BRWIDTH;
                        }
                        else
                        {
                            xmax = Balls[k]->x+Balls[k]->width;
                        }

                        float xsize = xmax - xmin;

                        // The origin is at the top-left corner of the screen
                        // Set collision response
                        if (xsize > ysize)
                        {
                            if (ballcentery > brickcentery)
                            {
                                // Bottom
                                Balls[k]->y += ysize + 0.01f; // Move out of collision
                                BallBrickResponse(3);
                            }
                            else
                            {
                                // Top
                                Balls[k]->y -= ysize + 0.01f; // Move out of collision
                                BallBrickResponse(1);
                            }
                        }
                        else
                        {
                            if (ballcenterx < brickcenterx)
                            {
                                // Left
                                Balls[k]->x -= xsize + 0.01f; // Move out of collision
                                BallBrickResponse(0);
                            }
                            else
                            {
                                // Right
                                Balls[k]->x += xsize + 0.01f; // Move out of collision
                                BallBrickResponse(2);
                            }
                        }

                        return;
                    }
                }
            }
        }
    }
}


void Game::BallBrickResponse(int dirindex)
{
    for (int i = 0; i<Balls.size(); i++)
    {
        // Direction factors
        int mulx = 1;
        int muly = 1;

        if (Balls[i]->dirx > 0)
        {
            // Ball dang di chuyen theo chieu duong x
            if (Balls[i]->diry > 0)
            {
                // Ball dang di chuyen theo chieu duong y
                // +1 +1
                if (dirindex == 0)
                {
                    mulx = -1;
                }
                else if (dirindex == 1)
                {
                    muly = -1;
                }
            }
            else if (Balls[i]->diry < 0)
            {
                // Ball dang di chuyen theo chieu am y
                // +1 -1
                if (dirindex == 0)
                {
                    mulx = -1;
                }
                else if (dirindex == 3)
                {
                    muly = -1;
                }
            }
        }
        else if (Balls[i]->dirx < 0)
        {
            // Ball dang di chuyen theo chieu am x
            if (Balls[i]->diry > 0)
            {
                // Ball dang di chuyen theo chieu duong y
                // -1 +1
                if (dirindex == 2)
                {
                    mulx = -1;
                }
                else if (dirindex == 1)
                {
                    muly = -1;
                }
            }
            else if (Balls[i]->diry < 0)
            {
                // Ball dang di chuyen theo chieu am y
                // -1 -1
                if (dirindex == 2)
                {
                    mulx = -1;
                }
                else if (dirindex == 3)
                {
                    muly = -1;
                }
            }
        }

        // Dinh huong lai direction cho ball
        Balls[i]->SetDirection(mulx*Balls[i]->dirx, muly*Balls[i]->diry);
    }
}


void Game::Render(float delta)
{
    SDL_RenderClear(renderer);

    background -> Render(delta);
    board->Render(delta);
    paddle->Render(delta);
    for (int i = 0; i<Balls.size(); i++)
    {
        Balls[i]->Render(delta);
    }

    for (int i = 0; i<Items.size(); i++)
    {
        Items[i]->Render(delta);
    }

    ScoreUpdate(GameScore, HighScore, hit_times);

    SDL_RenderPresent(renderer);
}


void Game:: ScoreUpdate(int score, int highscore, int hit)
{
    SDL_Texture* Score = NULL;
    SDL_Texture* HighScore = NULL;


    int length;
    if (score < 10)
    {
        length = 1*SCORE_WIDTH;
    }
    else if (score >=10 && score < 100)
    {
        length = 2*SCORE_WIDTH;
    }
    else if (score >=100 && score < 1000)
    {
        length = 3*SCORE_WIDTH;
    }

    else if ( score >=1000 && score < 10000)
    {
        length = 4*SCORE_WIDTH;
    }
    else
    {
        length = 5*SCORE_WIDTH;
    }

    //Levelup sound
    if ( score >= 100 && score< 500)
        level = 1;
    else if ( score >= 500 && score < 1000)
        level = 2;
    else if ( score >= 1000 && score < 10000)
        level = 3;

    if (level == 1 && onetime_1 == true)
    {
        if ( Mix_PlayChannel(-1,levelup,1) == -1)
            std:: cout << "Error playing sound: " << Mix_GetError << std::endl;
        onetime_1 = false;
    }
    else if (level == 2 && onetime_2 == true)
    {
        if ( Mix_PlayChannel(-1,levelup,1) == -1)
            std:: cout << "Error playing sound: " << Mix_GetError << std::endl;
        onetime_2 = false;
    }
    else if (level == 3 && onetime_3 == true)
    {
        if ( Mix_PlayChannel(-1,levelup,1) == -1)
            std:: cout << "Error playing sound: " << Mix_GetError << std::endl;
        onetime_3 = false;
    }

    TTF_Font* score_font = TTF_OpenFont("font//arcade.ttf", 60);
    if ( score_font == NULL)
    {
        std::cout << "Error load font: " << SDL_GetError() << std::endl;
    }

    SDL_Color YELLOW = {255,255,0,255};

    std::string score_text = std :: to_string (score);
    SDL_Surface* surface = TTF_RenderText_Solid(score_font, score_text.c_str(), YELLOW);
    Score = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);


    SDL_Rect score_rect;
    score_rect.x = WINDOWGAME_WIDTH + 270/2 - length/2;
    score_rect.y = 430;
    score_rect.w = length;
    score_rect.h = SCORE_HEIGHT;
    SDL_RenderCopy(renderer, Score, 0, &score_rect);

    std::string highscore_text = std :: to_string (highscore);
    surface = TTF_RenderText_Solid(score_font, highscore_text.c_str(), YELLOW);
    HighScore = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    int hlength;
    if (highscore < 10)
    {
        hlength = 1*SCORE_WIDTH;
    }
    else if (highscore >=10 && highscore < 100)
    {
        hlength = 2*SCORE_WIDTH;
    }
    else if (highscore >=100 && highscore < 1000)
    {
        hlength = 3*SCORE_WIDTH;
    }

    else if ( highscore >=1000 && highscore < 10000)
    {
        hlength = 4*SCORE_WIDTH;
    }
    else
    {
        hlength = 5*SCORE_WIDTH;
    }

    SDL_Rect highscore_rect;
    highscore_rect.x = WINDOWGAME_WIDTH + 270/2 - hlength/2;
    highscore_rect.y = 569;
    highscore_rect.w = hlength;
    highscore_rect.h = SCORE_HEIGHT;
    SDL_RenderCopy(renderer, HighScore, 0, &highscore_rect);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(Score);
    SDL_DestroyTexture(HighScore);
    TTF_CloseFont(score_font);
}

void Game::raiseItem()
{
    if (delay > 0)
        delay--;
    else
    {
        delay = ITEM_FREQ;
        Item* newItem = new Item(renderer);
        Items.push_back(newItem);
    }
}

bool Game::isPowerUp(Item* item)
{
    return paddle->IsCollides(item);
}

void Game::powerUpChange(Item* item)
{
    if (isPowerUp(item))
    {
        lastTime = SDL_GetTicks();
        item->y = 800;
        switch(item->itemChosen)
        {
        case 0:
        {
            for (int i = 0; i<Balls.size(); i++)
            {
                Balls[i]->bigBall();
            }
            break;
        }

        case 1:
            paddle->bigPaddle();
            break;
        case 2:
            Balls.push_back(new Ball(renderer));
            Balls.push_back(new Ball(renderer));
            ResetBall(Balls[Balls.size()-1]);
            Balls[Balls.size()-1]->SetDirection(-1,-1);
            ResetBall(Balls[Balls.size()-2]);
            Balls[Balls.size()-1]->SetDirection(1,-1);
            break;
        default:
            break;
        }
    }
    else
    {
        switch(item->itemChosen)
        {
        case 0:
            std::cout << SDL_GetTicks() - lastTime << std::endl;
            if (SDL_GetTicks() > 3000 + lastTime)
            {
                for (int i = 0; i<Balls.size(); i++)
                {
                    Balls[i]->normalBall();
                }
            }
            break;
        case 1:
            std::cout << SDL_GetTicks() - lastTime << std::endl;
            if (SDL_GetTicks() > 3000 + lastTime)
            {
                paddle->normalPaddle();
            }
            break;
        default:
            break;
        }
    }
}

