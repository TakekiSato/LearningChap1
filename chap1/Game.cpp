//
//  Game.cpp
//  chap1
//
//  Created by TakekiSato on 2019/05/29.
//  Copyright © 2019 TakekiSato. All rights reserved.
//

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddleDir(0)
,mPaddleDir_1(0)
{
    
}

bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    
    if(sdlResult != 0)
    {
        SDL_Log("SDLを初期化できません：%s", SDL_GetError());
        return false;
    }
    
    mWindow = SDL_CreateWindow("Chapter1 Practice", // ウィンドウのタイトル
                               100,
                               100,
                               1024,
                               768,
                               0);
    if(!mWindow)
    {
        SDL_Log("ウィンドウの作成に失敗しました：%s", SDL_GetError());
        return false;
    }
    
    mRederer = SDL_CreateRenderer(mWindow,  // 作成するレンダラーの描画対象になるウィンドウ
                                  -1,  // 通常は-1
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!mRederer)
    {
        SDL_Log("レンダラーの作成に失敗しました:%s", SDL_GetError());
        return false;
    }
    // パドルの位置初期化
    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f/2.0f;
    // パドル２本目の位置初期化
    mPaddlePos_1.x = 1024.0f - thickness - 10.0f;
    mPaddlePos_1.y = 768.0f/2.0f;
    // ボールの位置初期化
    mBallPos.x = 1024.0f/2.0f;
    mBallPos.y = 768.0f/2.0f;
    // ボールの速度初期化
    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;
    
    
    Ball ball_0;
    ball_0.pos.x = 1024.0f/3.0f;
    ball_0.pos.y = 768.0f/2.0f;
    ball_0.vel.x = -200.0f;
    ball_0.vel.y = 235.0f;
    Balls.push_back(ball_0);
    
    Ball ball_1;
    ball_1.pos.x = (1024.0f/3.0f) * 2;
    ball_1.pos.y = 768.0f/2.0f;
    ball_1.vel.x = 200.0f;
    ball_1.vel.y = 235.0f;
    Balls.push_back(ball_1);
    
    
    return true;
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRederer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop()
{
    while(mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    // キューにイベントがあれば繰り返す
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            // ここで各種のイベントを処理する
            // SDL_QUITイベントならば、ループを終える
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }
    // キーボードの状態を取得する
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // [ESC]キーが押されていても、ループを終える
    if(state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
    
    // w/sキーを押すことによってパドルの方向を更新する
    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddleDir += 1;
    }
    
    // I/Kキーを押すことによってパドル２本目の方向を更新する
    mPaddleDir_1 = 0;
    if (state[SDL_SCANCODE_I])
    {
        mPaddleDir_1 -= 1;
    }
    if (state[SDL_SCANCODE_K])
    {
        mPaddleDir_1 += 1;
    }
}

void Game::UpdateGame()
{
    // 前のフレームから16msが経過するまで待つ
    // フレームレートが異なるモニターでもゲームの速度が変わらないようにするため？
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;
    // deltaTimeは前のフレームとの時刻の差を秒に変換した値
    // 1ミリ秒→0.001秒なのでミリ秒を1000で割ると秒に変換できる
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    
    // デルタタイムを最大値で制限する
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    // 時刻を更新（次のフレームのために）
    mTicksCount = SDL_GetTicks();
    
    // TODO: ゲームワールドのオブジェクトを
    //       デルタタイムの関数として更新する
    if (mPaddleDir != 0)
    {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        // パドルが画面から出ないようにする
        if (mPaddlePos.y < (paddleH/2.0f + thickness))
        {
            mPaddlePos.y = paddleH/2.0f + thickness;
        }
        else if(mPaddlePos.y > 768.0f - paddleH/2.0f - thickness)
        {
            mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
        }
    }
    
    if (mPaddleDir_1 != 0)
    {
        mPaddlePos_1.y += mPaddleDir_1 * 300.0f * deltaTime;
        // パドルが画面から出ないようにする
        if (mPaddlePos_1.y < (paddleH/2.0f + thickness))
        {
            mPaddlePos_1.y = paddleH/2.0f + thickness;
        }
        else if(mPaddlePos_1.y > 768.0f - paddleH/2.0f - thickness)
        {
            mPaddlePos_1.y = 768.0f - paddleH/2.0f - thickness;
        }
    }
    
    // ボールの位置更新
    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;
    
    // ボールの位置更新：複数版
//    for(int i = 0; i < Balls.size() - 1; i++)
//    {
//        Balls[i].pos.x += Balls[i].vel.x * deltaTime;
//        Balls[i].pos.y += Balls[i].vel.y * deltaTime;
//    }
    
//    // パドル1とボールの位置の差
//    float diff = mPaddlePos.y - mBallPos.y;
//    // 位置の差を絶対値に変換する
//    diff = (diff > 0.0f) ? diff : -diff;
//    if (
//        // もしyの差が充分に小さく
//        diff <= paddleH / 2.0f &&
//        // ボールが正しい位置にあり
//        mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
//        // ボールが左向きに動いていれば
//        mBallVel.x < 0.0f
//        )
//    {
//        mBallVel.x *= -1.0f;
//    }
//    else if (mBallPos.x <= 0.0f)
//    {
//        // mIsRunning = false;  // テスト中なので
//        mBallVel.x *= -1.0f;
//    }
//    else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
//    {
//        mBallVel.x *= -1;
//    }
//    // パドル２本目との衝突判定も追加の必要あり
//    float diff_1 = mPaddlePos_1.y - mBallPos.y;
//    diff_1 = (diff_1 > 0.0f) ? diff_1 : -diff_1;
//    if (
//        // パドルにボールが当たっていて
//        diff_1 <= paddleH / 2.0f &&
//        // ボールの位置が正しい位置にあれば
//        mBallPos.x <= mPaddlePos_1.x && mBallPos.x >= mPaddlePos_1.x - thickness / 2.0f &&
//        // ボールが右向きに動いていれば
//        mBallVel.x > 0.0f
//        )
//    {
//        mBallVel.x *= -1.0f;
//    }
//
//    // 上の壁との衝突
//    if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
//    {
//        mBallVel.y *= -1;
//    }
//    // 下の壁との衝突
//    else if (mBallPos.y >= 768.0f - thickness && mBallVel.y > 0.0f)
//    {
//        mBallVel.y *= -1;
//    }
    
    // 範囲for文版(正しいかはわからん)
    for(auto &ball:Balls)
    {
        ball.pos.x += ball.vel.x * deltaTime;
        ball.pos.y += ball.vel.y * deltaTime;
    }
    
    // パドルと壁との衝突検知：複数ボール版
    for(auto &ball:Balls)
    {
        // パドル１との衝突判定
        float diff_0 = mPaddlePos.y - ball.pos.y;
        diff_0 = (diff_0 > 0) ? diff_0 : -diff_0;
        if (
            // もしyの差が充分に小さく
            diff_0 <= paddleH / 2.0f &&
            // ボールが正しい位置にあり
            ball.pos.x <= 25.0f && ball.pos.x >= 20.0f &&
            // ボールが左向きに動いていれば
            ball.vel.x < 0.0f
            )
        {
            ball.vel.x *= -1.0f;
        }
        // パドル２との衝突判定
        float diff_1 = mPaddlePos_1.y - ball.pos.y;
        diff_1 = (diff_1 > 0) ? diff_1 : -diff_1;
        if (
            // パドルにボールが当たっていて
            diff_1 <= paddleH / 2.0f &&
            // ボールの位置が正しい位置にあれば
            ball.pos.x <= mPaddlePos_1.x && ball.pos.x >= mPaddlePos_1.x - thickness / 2.0f &&
            // ボールが右向きに動いていれば
            ball.vel.x > 0.0f
            )
        {
            ball.vel.x *= -1.0f;
        }
        // 左の壁との衝突判定
        if (ball.pos.x <= 0.0f)
        {
            // mIsRunning = false;  // テスト中なので
            ball.vel.x *= -1.0f;
        } // 右壁との衝突判定
        else if (ball.pos.x >= (1024.0f - thickness) && ball.vel.x > 0.0f)
        {
            ball.vel.x *= -1;
        }
        
        // 上の壁との衝突
        if (ball.pos.y <= thickness && ball.vel.y < 0.0f)
        {
            ball.vel.y *= -1;
        }
        // 下の壁との衝突
        else if (ball.pos.y >= 768.0f - thickness && ball.vel.y > 0.0f)
        {
            ball.vel.y *= -1;
        }
    }
}

void Game::GenerateOutput()
{
    // 描画色の指定
    SDL_SetRenderDrawColor(mRederer,
                           0,    // R
                           0,    // G
                           255,  // B
                           255   // A
                           );
    // バックバッファローを現在の描画色でクリアする
    SDL_RenderClear(mRederer);
    // フロントバッファとバックバッファの交換の前にオブジェクトの描画を行う
    // 長方形とボールは白色で描画する
    SDL_SetRenderDrawColor(mRederer, 255, 255, 255, 255);
    // 長方形の描画にSDL_Rect構造体を使う
    // 上部長方形
    SDL_Rect wall{
        0,         // 左上隅の x
        0,         // 左上隅の y
        1024,      // 幅
        thickness  // 高さ
    };
    // 長方形を描画
    SDL_RenderFillRect(mRederer, &wall);
    // 下部長方形
    wall.y = 768 - thickness;
    SDL_RenderFillRect(mRederer, &wall);
    // 右長方形
//    wall.x = 1024 - thickness;
//    wall.y = 0;
//    wall.w = thickness;
//    wall.h = 1024;
//    SDL_RenderFillRect(mRederer, &wall);
    
    // パドルの描画
    SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - paddleH/2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRederer, &paddle);
    
    // パドル2本目の描画
    SDL_Rect paddle_1{
        static_cast<int>(mPaddlePos_1.x),
        static_cast<int>(mPaddlePos_1.y - paddleH/2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRederer, &paddle_1);
    
//    // ボールの描画
//    SDL_Rect ball {
//        static_cast<int>(mBallPos.x - thickness/2),
//        static_cast<int>(mBallPos.y - thickness/2),
//        thickness,
//        thickness
//    };
//    SDL_RenderFillRect(mRederer, &ball);
    
    // ボールの描画：複数版
    for(auto &ball:Balls)
    {
        SDL_Rect ball_rect {
            static_cast<int>(ball.pos.x - thickness/2),
            static_cast<int>(ball.pos.y - thickness/2),
            thickness,
            thickness
        };
        SDL_RenderFillRect(mRederer, &ball_rect);
    }
    // フロントバッファとバックバッファを交換する
    SDL_RenderPresent(mRederer);
}
