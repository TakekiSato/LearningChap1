//
//  Game.h
//  chap1
//
//  Created by TakekiSato on 2019/05/28.
//  Copyright © 2019 TakekiSato. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <SDL2/SDL.h>
#include <vector>

struct Vector2
{
    float x;
    float y;
};

struct Ball
{
    Vector2 pos;
    Vector2 vel;
};

class Game
{
public:
    Game();
    // ゲームの初期化
    bool Initialize();
    // ゲームオーバーまでゲームループを実行
    void RunLoop();
    // ゲームをシャットダウンする
    void Shutdown();
    
private:
    // ゲームループのためのヘルパー関数
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    
    // SDLが作るウィンドウ
    SDL_Window* mWindow;
    SDL_Renderer* mRederer;
    
    // ゲーム開始以降のティック数
    Uint32 mTicksCount;
    // ゲームが続くかどうか
    bool mIsRunning;
    
    // パドルの動く方向
    int mPaddleDir;
    // パドルの位置
    Vector2 mPaddlePos;
    
    // パドル２本目の動く方向
    int mPaddleDir_1;
    // パドル２本目の位置
    Vector2 mPaddlePos_1;
    
    // ボールの位置
    Vector2 mBallPos;
    // ボールの速度
    Vector2 mBallVel;
    
    // Ballの座標と速度の構造体
    std::vector<Ball> Balls;
};


#endif /* Game_h */
