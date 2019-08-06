#include "Board.h"

Board::Board(SDL_Renderer* renderer): Object(renderer) {

    SDL_Surface* surface = IMG_Load("PNG//bricks.png");
    bricktexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//brokenbricks.png");
    brokenbricktexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//side.png");
    sidetexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//logo.png");
    logotexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//heart.png");
    hearttexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//explosion.png");
    explosiontexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    TTF_Font* score_font = TTF_OpenFont("font//arcade.ttf", 60);
    if ( score_font == NULL) {
        std::cout << "Error load font: " << SDL_GetError() << std::endl;
    }
    SDL_Color WHITE = {255,255,255,255};

    std::string score_word = "SCORE";
    surface = TTF_RenderText_Solid(score_font, score_word.c_str(), WHITE);
    scoreWordTexture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    std::string highscore_word = "HIGHSCORE";
    surface = TTF_RenderText_Solid(score_font, highscore_word.c_str(), WHITE);
    highscoreWordTexture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    srand(time(0));

    x = 16;
    y = 0;
    width = 960;
    height = 800;

    brickoffsetx = 0;
    brickoffsety = 0;
}

Board::~Board() {
    // Clean resources
    SDL_DestroyTexture(bricktexture);
    SDL_DestroyTexture(sidetexture);
    SDL_DestroyTexture(brokenbricktexture);

}

void Board::Update(float delta) {

}

void Board::Render(float delta) {
    // Render bricks
    for (int i=0; i<BRICK_PER_ROW; i++) {
        for (int j=0; j<BRICK_PER_COL; j++) {
            Brick brick = bricks[i][j];

            // Check if the brick exists
            if (!brick.state)
                continue;

            SDL_Rect srcrect;

            srcrect.x = (brick.type % 2) * BOARD_BRWIDTH;
            srcrect.y = (brick.type / 2) * BOARD_BRHEIGHT;
            srcrect.w = BOARD_BRWIDTH;
            srcrect.h = BOARD_BRHEIGHT;

            SDL_Rect dstrect;
            dstrect.x = brickoffsetx + x + i * BOARD_BRWIDTH;
            dstrect.y = brickoffsety + y + j * BOARD_BRHEIGHT;
            dstrect.w = BOARD_BRWIDTH;
            dstrect.h = BOARD_BRHEIGHT;


            if (brick.status == 1 )
                SDL_RenderCopy(renderer, brokenbricktexture, &srcrect, &dstrect);
            else
                SDL_RenderCopy(renderer, bricktexture, &srcrect, &dstrect);
        }
    }

    // Render sides
    SDL_Rect dstrect;
    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 650;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);

    dstrect.x = 992 - 16;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 650;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);

    // Render logo
    SDL_Rect drect;
    drect.x = 998;
    drect.y = 30;
    drect.w = 250;
    drect.h = 250;
    SDL_RenderCopy(renderer, logotexture, 0 , &drect);

    /*SDL_Rect explosionRect;
    explosionRect.x = 200;
    explosionRect.y = 200;
    explosionRect.w = 2*BOARD_BRWIDTH;
    explosionRect.h = 2*BOARD_BRHEIGHT;
    SDL_RenderCopy(renderer, explosiontexture, 0 , &explosionRect);
    */

    // Render heart
    for ( int i = 0 ; i < HEARTCOUNT; i++ ) {
        Heart heart = hearts [i];

        if ( !heart.state)
            continue;

        drect.x = 1020 + i* (HEART_WIDTH + 30) ;
        drect.y = 300;
        drect.w = HEART_WIDTH;
        drect.h = HEART_HEIGHT;
        SDL_RenderCopy(renderer, hearttexture, 0 , &drect);
    }

    //Render score word
    SDL_Rect scoreWordRect;
    scoreWordRect.x = WINDOWGAME_WIDTH + 270/2 - 87;
    scoreWordRect.y = 360;
    scoreWordRect.w = 170;
    scoreWordRect.h = 100;
    SDL_RenderCopy(renderer, scoreWordTexture, 0 , &scoreWordRect);

    SDL_Rect highscoreWordRect;
    highscoreWordRect.x = WINDOWGAME_WIDTH + 270/2 - 125;
    highscoreWordRect.y = 500;
    highscoreWordRect.w = 250;
    highscoreWordRect.h = 100;
    SDL_RenderCopy(renderer, highscoreWordTexture, 0 , &highscoreWordRect);
}

void Board::CreateLevel() {
    //Create bricks
    for (int i=0; i<BRICK_PER_ROW; i++) {
        for (int j=0; j<BRICK_PER_COL; j++) {
            Brick brick;

            brick.state = true;
            if ( j == 7 || j == 8)
                brick.state = false;
            if ( j >= 9 && j <= 15) {
                if ( i % 2 == 0 && j % 2 ==0)
                    brick.state = false;
            }
            if ( j == 9 ) {
                brick.type = 5;
                brick.status = 4;
            }

            else {
                brick.type = rand() % (4 - 0 + 1) + 0;
                brick.status = 2;
            }
            bricks[i][j] = brick;

        }
    }

    //Create Heart
    for ( int i=0; i<HEARTCOUNT; i++) {
        Heart heart;
        heart.state = true;
        hearts[i] = heart;
    }

}
