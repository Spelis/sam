#include "raylib.h"
#include "vector"
#include "string"
#include <iostream>
#include <sstream>
#include "stdlib.h"
#include "time.h"
#include <thread>

using namespace std;

vector<string> randLevel(int range)
{
    vector<string> level = {};
    int i;
    for (i = 0; i < range; i++)
    {
        // 40, 30
        string block = to_string(rand() % 5);
        string x = to_string(rand() % 40);
        string y = to_string(rand() % 30);
        string b = block + ";" + x + ";" + y;
        level.push_back(b);
    }
    return level;
}

Vector2 player = {390, 290};
Vector2 pSize = {20, 40};
Vector2 veloc = {0, 0};
Vector2 camPos = player;

int jump = 0;

vector<string> splitString(const string &input, char delimiter)
{
    vector<string> tokens;
    stringstream ss(input);
    string token;
    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}
        float lerp(float start, float end, float t) {
            return start + (end - start) * t;
        }

int main()
{
    InitWindow(800, 600, "SAM");
    srand(time(NULL));

    Texture2D stone = LoadTexture("assets/stone.png");
    Texture2D dirt = LoadTexture("assets/dirt.png");
    Texture2D grass = LoadTexture("assets/grass.png");
    Texture2D water = LoadTexture("assets/water.png");
    Texture2D obsidian = LoadTexture("assets/obsidian.png");

    vector<Texture2D> textures = {stone, dirt, grass, water, obsidian};

    vector<string> level = {"1;17;12;0", "1;18;12;0", "1;19;12;0", "1;20;12;0", "1;21;12;0", "1;22;12;0", "1;17;13;1", "1;22;13;1", "1;17;14;1", "1;22;14;1", "1;17;15;1", "1;22;15;1", "1;17;16;1", "1;22;16;1", "0;11;17;0", "3;12;17;0", "3;13;17;0", "3;14;17;0", "3;15;17;0", "3;16;17;0", "1;17;17;0", "1;18;17;0", "1;19;17;0", "1;20;17;0", "1;21;17;0", "1;22;17;0", "3;23;17;0", "3;24;17;0", "3;25;17;0", "3;26;17;0", "3;27;17;0", "0;28;17;0", "0;11;18;0", "3;12;18;0", "3;13;18;0", "3;14;18;0", "3;15;18;0", "3;16;18;0", "0;17;18;0", "0;18;18;1", "0;19;18;1", "0;20;18;1", "0;21;18;1", "0;22;18;0", "3;23;18;0", "3;24;18;0", "3;25;18;0", "3;26;18;0", "3;27;18;0", "0;28;18;0", "0;11;19;0", "0;12;19;0", "0;13;19;0", "0;14;19;0", "0;15;19;0", "0;16;19;0", "0;17;19;0", "0;18;19;1", "0;19;19;1", "0;20;19;1", "0;21;19;1", "0;22;19;0", "0;23;19;0", "0;24;19;0", "0;25;19;0", "0;26;19;0", "0;27;19;0", "0;28;19;0"};
    float delta;
    SetTargetFPS(600);

    while (!WindowShouldClose())
    {
        delta = GetFrameTime() * 600;
        bool atk = false;
        // Input

        if (IsKeyDown(KEY_A))
        {
            veloc.x -= 0.02;
        }
        if (IsKeyDown(KEY_D))
        {
            veloc.x += 0.02;
        }
        if (IsKeyPressed(KEY_SPACE) && jump > 1)
        {
            veloc.y = -1;
            jump--;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            atk = true;
        }

        camPos.x = lerp(camPos.x,player.x,0.02f);
        camPos.y = lerp(camPos.y,player.y,0.02f);


        // STOP Input

        veloc.y += 0.005;
        veloc.x *= 0.97;

        ClearBackground(SKYBLUE);
        BeginDrawing();

        Rectangle pl = {player.x, player.y, pSize.x, pSize.y};
        bool collidingXM = false;
        bool collidingXP = false;
        bool collidingYM = false;
        bool collidingYP = false;
        bool jumpable = false;
        bool watery = false;
        Rectangle XM = {player.x, player.y + 16, 1, pSize.y - 32};
        Rectangle XP = {player.x + pSize.x, player.y + 16, 1, pSize.y - 32};
        Rectangle YM = {player.x + 8, player.y, pSize.x - 16, 1};
        Rectangle YP = {player.x + 8, player.y + pSize.y - 1, pSize.x - 16, 1};
        Rectangle jColl = {player.x + 8, player.y + pSize.y + 2, pSize.x - 16, 1};
        for (int i = 0; i < level.size(); i++)
        {
            string block = level.at(i);
            vector<string> blockinfo = splitString(block, ';');
            Texture2D texture = textures.at(stoi(blockinfo.at(0)));
            Rectangle blockrec = {stoi(blockinfo.at(1)) * 20.0f, stoi(blockinfo.at(2)) * 20.0f, 20, 20};
            int a;
            int c;
            if (blockinfo.at(0) != "3")
            {
                a = 255;
            }
            else
            {
                a = 128;
            }
            if (blockinfo.at(3) == "1")
            {
                c = 128;
            }
            else
            {
                c = 255;
            }
            DrawTexture(texture, blockrec.x-camPos.x+390, blockrec.y-camPos.y+290, (Color){c, c, c, a});
        }
        for (int i = 0; i < level.size(); i++)
        {
            string block = level.at(i);
            vector<string> blockinfo = splitString(block, ';');
            Rectangle blockrec = {stoi(blockinfo.at(1)) * 20.0f, stoi(blockinfo.at(2)) * 20.0f, 20, 20};
            int blocktype = stoi(blockinfo.at(0));
            int isbg = stoi(blockinfo.at(3));

            // XM clipping
            if (isbg != 1)
            {
                if (blocktype != 3)
                {
                    int distXM = blockrec.x - player.x;
                    int distXP = player.x + pSize.x - blockrec.x;
                    int distYM = blockrec.y - player.y;
                    int distYP = player.y + pSize.y - blockrec.y;
                    if (distXM < veloc.x)
                    {
                        if (distYP < 20 && distYM < 20)
                        {
                            veloc.x = distXM;
                        }
                    }
                    if (distXP > veloc.x)
                    {
                        if (distYP < 20 && distYM < 20)
                        {
                            veloc.x = distXM;
                        }
                    }
                    if (distYM < veloc.y)
                    {
                        if (distYP < 20 && distYM < 20)
                        {
                            veloc.y = distYM;
                        }
                    }
                    if (distYP < veloc.y)
                    {
                        if (distYP < 40 && distYM < 20)
                        {
                            veloc.y = distYM;
                        }
                    }

                    // Check for collision
                    if (CheckCollisionRecs(XM, blockrec))
                    {
                        collidingXM = true;
                    }
                    if (CheckCollisionRecs(XP, blockrec))
                    {
                        collidingXP = true;
                    }
                    if (CheckCollisionRecs(YM, blockrec))
                    {
                        collidingYM = true;
                    }
                    if (CheckCollisionRecs(YP, blockrec))
                    {
                        collidingYP = true;
                    }
                }
                else if (CheckCollisionRecs(pl, blockrec))
                {
                    watery = true;
                }
                if (CheckCollisionRecs(jColl, blockrec) || watery)
                {
                    jumpable = true;
                }
            }
        }
        DrawRectangleV((Vector2){player.x - camPos.x + 390,player.y - camPos.y + 290}, pSize, LIGHTGRAY);

        player.x += veloc.x * delta;
        if (!watery)
        {
            player.y += veloc.y * delta;
        }
        else
        {
            player.y += veloc.y * 0.5 * delta;
        }

        // Resolve collisions after applying velocity
        if (collidingXM)
        {
            veloc.x = 0;
            player.x += 1 * delta;
        }
        if (collidingXP)
        {
            veloc.x = 0;
            player.x -= 1  * delta;
        }
        if (collidingYM)
        {
            veloc.y = 0;
            player.y += 1 * delta;
        }
        if (collidingYP)
        {
            if (veloc.y > 0)
            {
                veloc.y = 0;
            }
            player.y -= 1 * delta;
        }
        if (jumpable)
        {
            jump = 2;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
