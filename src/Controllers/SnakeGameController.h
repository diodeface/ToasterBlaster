#pragma once

#include "Utilities/Util.h"
#include "Utilities/Globals.h"
#include "Controller.h"
#include "Drawing/DisplayManager.h"
#include "Drawing/Effects/SetPixel.h"
#include "Drawing/Effects/DrawDigits.h"
#include <vector>
#include <algorithm>

enum SnakeGameState {
    SNAKE_STOP,
    SNAKE_WAITING,
    SNAKE_PLAY
};

class SnakeGameController : public Controller {
  private:
    DisplayManager* displayManager;
    u16 score, maxSnakeSize;
    u8 tailRemaining;
    SnakeGameState state;
    Direction direction, lastDirection;

    Timestamp blinkTime, blinkPeriod;
    Timestamp time, period;

    Point fruit;
    bool fruitColor;
    u8 screenWidth, screenHeight;

    std::vector<Point> snakeParts;
    std::vector<Display*> gameDisplays;
    std::vector<Display*> scoreDisplays;

    void spawnFruit(u8 points = 1) {
        // check snake size to prevent infinite loop
        if (snakeParts.size() >= maxSnakeSize) {
            gameOver();
            return;
        }

        // move fruit to any position that's not a snake part
        u32 tries = 0;
        do {
            fruit.x = random(screenWidth) + 1;
            fruit.y = random(screenHeight) + 1;
            tries++;
            if(tries >= 500000) {
                gameOver();
                return;
            } 
        } while (collisionCheck(fruit));

        score += points;
        tailRemaining = 2;
        drawScore(score);
        TOASTER_LOG("Snake: New fruit: %d, %d, score: %d\n", fruit.x, fruit.y, score);
    }

    void drawPixel (u8 x, u8 y, DrawMode color = DRAW_MODE_1) {
        for(Display* d : gameDisplays) {
            SetPixel::setPixel(d->bitmapBuffer, d->getSize(), x, y, color);
        }
    }

    void drawScore(u32 value) {
        for(Display* d : scoreDisplays) {
            DrawDigits::drawDigits(d->bitmapBuffer, d->getSize(), value);
        }
    }

    void gameOver() {
        TOASTER_LOG("Snake: Game over!\n");
        Point part = snakeParts[1];
        for(Display* d : gameDisplays) {
            //Invert::invert(d->bitmapBuffer, d->getSize());
            d->particleSystem->emit(32, d->getSize(), part.x, part.y, -0.5f, -0.5f, 0.5f, 0.5f, DRAW_MODE_XOR);
        }
        state = SNAKE_STOP;
        direction = NONE;
        lastDirection = NONE;
    }

    void resetGame() {
        score = 0;
        state = SNAKE_WAITING;
        direction = NONE;
        lastDirection = NONE;
        snakeParts.clear();

        for(Display* d : gameDisplays) {
            std::fill(d->bitmapBuffer, d->bitmapBuffer + d->getSize(), 0);
            d->particleSystem->clear();
        }

        for(Display* d : scoreDisplays) {
            std::fill(d->bitmapBuffer, d->bitmapBuffer + d->getSize(), 0);
        }

        for (u8 i = 1; i < screenWidth + 2; i++) {
            drawPixel(i, 0);
            drawPixel(i, screenHeight + 2);
        }

        for (u8 i = 1; i < screenHeight + 2; i++) {
            drawPixel(0, i);
            drawPixel(screenWidth + 2, i);
        }
        
        u8 x = 1 + screenWidth / 2;
        u8 y = 1 + screenHeight / 2;
        snakeParts.push_back({x, y});
        drawPixel(x, y);

        spawnFruit(0);
        tailRemaining = 5;
    }

    bool collisionCheck (Point point, size_t offset = 0) {
        // check collisions
        for(size_t i = offset; i < snakeParts.size(); i++) {
            Point part = snakeParts[i];
            if (part.x == point.x && part.y == point.y) return true;
        }
        return false;
    }

    void tick() {
        switch(state) {
            case SNAKE_STOP:
                if (direction != NONE) resetGame();
                break;
            case SNAKE_WAITING:
                if (direction != NONE) state = SNAKE_PLAY;
                break;
            case SNAKE_PLAY:          
                // create new part and draw pixel
                Point part = snakeParts[0];
                switch(direction) {
                    case UP: 
                        --part.y;
                        break;
                    case DOWN: 
                        ++part.y;
                        break;
                    case LEFT: 
                        --part.x;
                        break;
                    case RIGHT: 
                        ++part.x;
                        break;
                }
                snakeParts.insert(snakeParts.begin(), part);
                drawPixel(part.x, part.y);

                // self collision check
                if(collisionCheck(part, 1)) gameOver();

                // bounds collision check
                if(part.x <= 0 || part.x > screenWidth+1 || part.y <= 0 || part.y > screenHeight+1) gameOver();

                // check fruit collisions
                if(part.x == fruit.x && part.y == fruit.y) {
                    spawnFruit();
                }

                // grow tail
                if (tailRemaining > 0) {
                    --tailRemaining;
                } else {
                    // remove last part and draw blank pixel
                    part = snakeParts[snakeParts.size()-1];
                    drawPixel(part.x, part.y, DRAW_MODE_0);
                    snakeParts.pop_back();
                }
                    
                displayManager->requestRedraw();

                lastDirection = direction;
                break;
        }
    }

  public:
    SnakeGameController(
        DisplayManager* displayManager,
        DisplayBitmask gameDisplayBitmask = MOUTH,
        DisplayBitmask scoreDisplayBitmask = EYES,
        Timestamp gameSpeed = 200 MILLIS
    ) 
        : Controller(CONTROLLER_DEFAULT, SP_STOP)
        , displayManager(displayManager) 
        , period(gameSpeed)
        , blinkPeriod(100 MILLIS)
        , fruitColor(DRAW_MODE_0)
    {
        for(Display* d : *displayManager->getDisplays(gameDisplayBitmask)) {
            gameDisplays.push_back(d);
        }
        for(Display* d : *displayManager->getDisplays(scoreDisplayBitmask)) {
            scoreDisplays.push_back(d);
        }
        displayManager->setKeyframe(Keyframes::clearAll);
        displayManager->update();
        displayManager->hud->clearBottom();
        displayManager->hud->drawStatusText("Snake!");
        displayManager->hud->drawStrX2(0, 48, "Snake!");
        screenWidth = gameDisplays[0]->getSize() - 3;   
        screenHeight = 5; // assuming display that's 8 pixels tall
        maxSnakeSize = (screenWidth+1) * (screenHeight+1);
        resetGame();
    }

    void handleInput(u8 x, u8 y) override {
        if(y == 0x00 && lastDirection != DOWN) direction = UP;
        else if(x == 0xFF && lastDirection != RIGHT) direction = LEFT;
        else if(y == 0xFF && lastDirection != UP) direction = DOWN;
        else if(x == 0x00 && lastDirection != LEFT) direction = RIGHT;
    }

    void update() override {
        // 250ms period: fruit blink
        if((blinkTime += deltaTime) > blinkPeriod) {
            blinkTime = 0;
            fruitColor = !fruitColor;
            drawPixel(fruit.x, fruit.y, (DrawMode)fruitColor);
            displayManager->requestRedraw();
        }

        // 500ms period: game tick
        if((time += deltaTime) > period) {
            time = 0;
            tick();
        }
    }
};