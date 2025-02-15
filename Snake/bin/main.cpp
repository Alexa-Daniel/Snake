#include <iostream>
#include "C:\raylib\raylib\src\raylib.h"

using namespace std;

int windowWidth = 1080;
int windowHeight = 720;
int cellSize = 25;
int cellNumberX = 32;
int cellNumberY = 34;
int offset = 60;
int posx = 0;
int posy = 0;
int length = 3;
int length2 = 0;
int snakedir= -1;
int prevposx;
int prevposy;
int prevposx2;
int prevposy2;
int dir[4] = {-1, 1, 1, -1};
int frameCounter;
int prevdir;
int HighScore;

Color pastelGreen = {154, 204, 153, 255};
Color darkGreen = {144, 193, 144, 255};
Color pastelBlue = {154, 204, 204, 255};
Color darkBlue = {144, 193, 195, 255};
Color pastelPurple = {163, 153, 204, 255};
Color darkPurple = {153, 142, 195, 255};

bool toAdd = false;
bool canPress = true;

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class SNAKE
{
    public:
    //deque<Vector2> body = {Vector2{18, 12}};

    Texture2D rock;
    Texture2D chest;

    bool toAdd = false;
    double speed = 0.1;
    int speedCounter = 1;
    bool addedSpeed = false;

    struct snk
    {
        int x, y;
    }body[1000];

    SNAKE()
    {
        Image image2 = LoadImage("Graphics/therock.png");
        Image image3 = LoadImage("Graphics/chest.png");
        rock = LoadTextureFromImage(image2);
        chest = LoadTextureFromImage(image3);
        UnloadImage(image2);
        UnloadImage(image3);
    }

    ~SNAKE()
    {
        UnloadTexture(rock);
        UnloadTexture(chest);
    }

    void Draw()
    {
        for(int i=1; i<=length; i++)
        {
            float x = body[i].x;
            float y = body[i].y;

            /*if(i==1)
            {
                DrawTexture(rock, offset + body[1].x * cellSize - 3, offset + body[1].y * cellSize - 7, WHITE);
            }
            else
            {
                DrawTexture(chest, offset + body[i].x * cellSize, offset + body[i].y * cellSize, WHITE);
                //Rectangle cell = {offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};

                //DrawRectangleRounded(cell, 0.5, 6, DARKBROWN);
            }*/

            Rectangle cell = {offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(cell, 0.5, 6, BLACK);

            //DrawCircle(offset + (x+0.5) * cellSize, offset + (y+0.5) * cellSize, cellSize/2, BLACK);
            //DrawCircleLines(offset + (x+0.5) * cellSize, offset + (y+0.5) * cellSize, cellSize/2, BLACK);

            //DrawRectangle(offset + x * cellSize, offset + y * cellSize, cellSize, cellSize, GREEN);
            //DrawRectangleLines(offset + x * cellSize, offset + y * cellSize, cellSize, cellSize, BLACK);
        }
    }

    void Restart()
    {
        length=3;
        length2=0;
        body[1].x=16; body[1].y=16;
        body[2].x=15; body[2].y=16;
        body[3].x=14; body[3].y=16;
        snakedir = -1;
        speed = 0.1;
        speedCounter = 1;
    }

    void Update(int k)
    {
        int i;
        canPress=false;
        if((length-3) > HighScore)
        {
            HighScore = length-3;
        }
        if((length-3) % 10 == 1)
        {
            addedSpeed = false;
        }
        if(toAdd)
        {
            length++;
            length2++;
            toAdd = false;
        }
        if(k==1 || k==3)
        {
            prevposx=body[1].x;
            prevposy=body[1].y;
            body[1].x = body[1].x + dir[k];
            for(i=2; i<=length; i++)
            {
                if(i%2==1)
                {
                    prevposx = body[i].x;
                    prevposy = body[i].y;
                    body[i].x = prevposx2;
                    body[i].y = prevposy2;
                }
                if(i%2==0)
                {
                    prevposx2 = body[i].x;
                    prevposy2 = body[i].y;
                    body[i].x = prevposx;
                    body[i].y = prevposy;
                }
            }
        }
        if(k==0 || k==2)
        {
            prevposx=body[1].x;
            prevposy=body[1].y;
            body[1].y = body[1].y + dir[k];
            for(i=2; i<=length; i++)
            {
                if(i%2==1)
                {
                    prevposx = body[i].x;
                    prevposy = body[i].y;
                    body[i].x = prevposx2;
                    body[i].y = prevposy2;
                }
                if(i%2==0)
                {
                    prevposx2 = body[i].x;
                    prevposy2 = body[i].y;
                    body[i].x = prevposx;
                    body[i].y = prevposy;
                }
            }
        }
    }

    void addSpeed()
    {
        addedSpeed=true;
        speedCounter++;
        speed-=0.01;
    }
};

class FOOD
{
    public:
    Vector2 pos;
    Texture2D food;

    FOOD()
    {
        Image image = LoadImage("Graphics/food.png");
        food = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    ~FOOD()
    {
        UnloadTexture(food);
    }

    void Draw()
    {
        DrawTexture(food, offset + pos.x * cellSize, offset + pos.y * cellSize, BLACK);
        //DrawRectangleLines(offset + pos.x * cellSize, offset + pos.y * cellSize, cellSize, cellSize, BLACK);
    }

    Vector2 RandomPos()
    {
        float x = GetRandomValue(0, cellNumberX-1);
        float y = GetRandomValue(0, cellNumberY-1);
        return Vector2{x, y};
    }

};

class SOUND
{
    public:
    Sound eat;
    Sound hit;
    Sound nextlvl;
    SOUND()
    {
        InitAudioDevice();
        eat = LoadSound("Sounds/eat.mp3");
        hit = LoadSound("Sounds/thud.mp3");
        nextlvl = LoadSound("Sounds/next.mp3");
    }

    ~SOUND()
    {
        UnloadSound(eat);
        UnloadSound(hit);
        UnloadSound(nextlvl);
        CloseAudioDevice();
    }
};

int main()
{
    InitWindow(2 * offset + cellNumberX * cellSize, 2 * offset + cellNumberY * cellSize, "Snake");
    SetTargetFPS(144);

    ClearBackground(pastelGreen);

    Rectangle rec = {(float)offset-5, (float)offset-5, (float)cellSize*cellNumberX+10, (float)cellSize*cellNumberY+10};

    SNAKE snake = SNAKE();
    snake.body[1].x=16; snake.body[1].y=16;
    snake.body[2].x=15; snake.body[2].y=16;
    snake.body[3].x=14; snake.body[3].y=16;

    FOOD food = FOOD();
    food.pos = food.RandomPos();

    SOUND sound = SOUND();

    while(!WindowShouldClose())
    {
        BeginDrawing();

        if((length-3) % 10 == 0 && (length-3)!=0 && !snake.addedSpeed)
        {
            PlaySound(sound.nextlvl);
            snake.addSpeed();
        }

        /*for(int i=0; i<cellNumberX; i++)
        {
            for(int j=0; j<cellNumberY; j++)
            {
                DrawRectangle(offset + i * cellSize, offset + j * cellSize, cellSize, cellSize, GRAY);
                DrawRectangleLines(offset + i * cellSize, offset + j * cellSize, cellSize, cellSize, BLACK);
            }
        }*/

        if(IsKeyPressed(KEY_UP) && snakedir != 2 && canPress)
        {
            snakedir = 0; canPress = false;
        }
        if(IsKeyPressed(KEY_DOWN) && snakedir != 0 && canPress)
        {
            snakedir = 2; canPress = false;
        }
        if(IsKeyPressed(KEY_RIGHT) && snakedir != 3 && canPress)
        {
            snakedir = 1; canPress = false;
        }
        if(IsKeyPressed(KEY_LEFT) && snakedir != 1 && snakedir != -1 && canPress)
        {
            snakedir = 3; canPress = false;
        }

        if(EventTriggered(snake.speed))
        {
            if(snakedir == 0) snake.Update(0);
            if(snakedir == 1) snake.Update(1);
            if(snakedir == 2) snake.Update(2);
            if(snakedir == 3) snake.Update(3);
            canPress=true;
        }

        if(snake.body[1].x == food.pos.x && snake.body[1].y == food.pos.y)
        {
            PlaySound(sound.eat);
            food.pos = food.RandomPos();
            for(int i=1; i<=length; i++)
            {
                if(food.pos.x == snake.body[i].x && food.pos.y == snake.body[i].y)
                {
                    food.pos = food.RandomPos();
                }
            }
            snake.toAdd = true;
        }

        if(snake.body[1].x == cellNumberX || snake.body[1].x == -1)
        {
            PlaySound(sound.hit);
            snake.Restart();
            food.pos = food.RandomPos();
        }
        if(snake.body[1].y == cellNumberY || snake.body[1].y == -1)
        {
            PlaySound(sound.hit);
            snake.Restart();
            food.pos = food.RandomPos();
        }
        for(int i=2; i<=length; i++)
        {
            if(snake.body[1].x == snake.body[i].x && snake.body[1].y == snake.body[i].y)
            {
                PlaySound(sound.hit);
                snake.Restart();
                food.pos = food.RandomPos();
            }
        }
        if(length2 == 30)
        {
            length2 = 0;
        }
        if(length2 < 10)
        {
            ClearBackground(pastelGreen);
            DrawRectangleRec(rec, darkGreen);
            DrawRectangleLinesEx(rec, 5, BLACK);
        }
        if(length2 >= 10 && length2 < 20 ) 
        {
            ClearBackground(pastelBlue);
            DrawRectangleRec(rec, darkBlue);
            DrawRectangleLinesEx(rec, 5, BLACK);
        }
        if(length2 >= 20 && length2 < 30)
        {
            ClearBackground(pastelPurple);
            DrawRectangleRec(rec, darkPurple);
            DrawRectangleLinesEx(rec, 5, BLACK);
        }

        snake.Draw();
        food.Draw();

        DrawText("SNAKE", 400, 10, 30, BLACK);
        DrawText("SPEED: ", 415, 920, 20, BLACK);
        DrawText(TextFormat("%i", snake.speedCounter), 2 * offset + 375, 920, 20, BLACK);
        DrawText("SCORE: ", offset - 5, 920, 20, BLACK);
        DrawText(TextFormat("%i", length-3), 2 * offset + 15, 920, 20, BLACK);
        DrawText("HIGHSCORE: ", offset + 665, 920, 20, BLACK);
        DrawText(TextFormat("%i", HighScore), 2 * offset + 735, 920, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
}

/*int windowHeight = 720;
int windowWidth = 1080;

//int cellNumber = 36;

int tailx[1000];
int taily[1000];
int length = 0;
int frameCounter;
int snake_dir = 0;
int snake_speed = 1;
float snakepos_x = 540;
float snakepos_y = 360;
float snakewidth = 20;
float snakeheight = 20;

int main () 
{
    InitWindow(windowWidth, windowHeight, "Just a window");
    SetTargetFPS(144);

    float foodpos_x = GetRandomValue(1, 1079);
    float foodpos_y = GetRandomValue(1, 719);
    float foodwidth = 5;
    float foodheight = 5;

    Rectangle snake = {snakepos_x, snakepos_y, snakewidth, snakeheight};
    Rectangle food = {foodpos_x, foodpos_y, foodwidth, foodheight};

    while(WindowShouldClose()==false)
    {
        BeginDrawing();

        tailx[1]=snake.x;
        taily[1]=snake.y;
        int prevx=tailx[1];
        int prevy=tailx[1];
        for(int i=2; i<=length; i++)
        {
            int prev2x=tailx[i];
            int prev2y=taily[i];
            tailx[i]=prevx;
            taily[i]=prevy;
            prevx=prev2x;
            prevy=prev2y;
        }

        ///Collision check
        if(CheckCollisionRecs(snake, food))
        {
            food.x = GetRandomValue(1, 1079);
            food.y = GetRandomValue(1, 719);
            cout<<food.x<<' '<<food.y<<endl;
            length++;
        }

        ///Border collision
        if(snake.x+20 >= windowWidth)
        {
            snake_dir = 0;
            CloseWindow();
        }
        if(snake.x <= 0)
        {
            snake_dir = 0;
            CloseWindow();
        }
        if(snake.y+20 >= windowHeight)
        {
            snake_dir = 0;
            CloseWindow();
        }
        if(snake.y <= 0)
        {
            snake_dir = 0;
            CloseWindow();
        }

        ///Assign direction
        if(IsKeyDown(KEY_UP)) snake_dir = 1;
        if(IsKeyPressed(KEY_DOWN)) snake_dir = 2;
        if(IsKeyPressed(KEY_RIGHT)) snake_dir = 3;
        if(IsKeyPressed(KEY_LEFT)) snake_dir = 4;

        ///Update direction
        if(snake_dir == 1) snake.y -= snake_speed; 
        if(snake_dir == 2) snake.y += snake_speed; 
        if(snake_dir == 3) snake.x += snake_speed; 
        if(snake_dir == 4) snake.x -= snake_speed; 

        ClearBackground(BLACK);
        DrawRectangle(snake.x, snake.y, snake.width, snake.height, GREEN);
        for(int i=1; i<=length; i++)
        {
            DrawRectangle(tailx[i], taily[i], snake.width, snake.height, GREEN);
        }
        DrawRectangle(food.x, food.y, food.width, food.height, RED);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

*/