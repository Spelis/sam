#include "raylib.h"
#include "vector"
#include "string"
#include <iostream>
#include <algorithm>
#include <any>
#include <cmath>
#include <fstream>

using namespace std;

struct Block
{
    bool isBack;
    int ColorIndex;
    int isPlat;
};

int main()
{
    InitWindow(800, 600, "SAM Level Editor");
    SetTraceLogLevel(LOG_ERROR);
    Vector2 cur = {250, 250};
    Vector2 player = {5000, 5000};
    Vector2 pSize = {20, 40};
    Vector2 off = {5000,5000};
    int color = 0;
    vector<Color> colors = {GRAY, BROWN, GREEN, BLUE, DARKPURPLE};
    vector<Color> darkcolors = {DARKGRAY, DARKBROWN, DARKGREEN, DARKBLUE, DARKPURPLE};
    vector<int> colorint = {0, 1, 2, 3, 4};
    vector<vector<Block>> level = {};
    bool back = false;
    int plat = 0;
    cout << "Initializing Level..\n";
    int i;
    for (i = 0; i < 500; i++)
    {
        level.push_back({});
        int j;
        for (j = 0; j < 500; j++)
        {
            level.at(i).push_back(Block{false, -1, 0});
        }
    }
    SetTargetFPS(20);

    BeginDrawing();

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_ENTER))
        {
            level.at(cur.y).at(cur.x) = {back, color, plat};
        }
        if (IsKeyPressedRepeat(KEY_W) || IsKeyPressed(KEY_W))
        {
            cur.y--;
        }
        if (IsKeyPressedRepeat(KEY_S) || IsKeyPressed(KEY_S))
        {
            cur.y++;
        }
        if (IsKeyPressedRepeat(KEY_A) || IsKeyPressed(KEY_A))
        {
            cur.x--;
        }
        if (IsKeyPressedRepeat(KEY_D) || IsKeyPressed(KEY_D))
        {
            cur.x++;
        }
        if (IsKeyDown(KEY_UP))
        {
            off.y -= 6;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            off.y += 6;
        }
        if (IsKeyDown(KEY_LEFT))
        {
            off.x -= 6;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            off.x += 6;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            color = (color + 1) % colors.size();
            DrawRectangle(10, 10, 20, 20, colors.at(color));
        }
        //char* charp;
        //sprintf(charp,"%.1f",std::round(cur.x));
        //DrawText(charp,10,40,20,WHITE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            back = !back;
            char* bstr;
            if (back)
            {
                bstr = "Toggled BG: True";
            }
            else
            {
                bstr = "Toggled BG: False";
            }
            DrawText(bstr, 10, 10, 20, WHITE);
        }
        if (IsKeyPressed(KEY_P))
        {
            char* bstr;
            if (plat == 0)
            {
                bstr = "Toggled Plat: True";
                plat = 1;
            }
            else
            {
                bstr = "Toggled Plat: False";
                plat = 0;
            }
            DrawText(bstr, 10, 10, 20, WHITE);
        }
        if (IsKeyDown(KEY_BACKSPACE))
        {
            level.at(cur.y).at(cur.x) = {false, -1};
        }
        if (IsKeyPressed(KEY_F))
        {
            string lvl;
            int i;
            for (i = 0; i < 500; i++)
            {
                int j;
                for (j = 0; j < 500; j++)
                {
                    if (level.at(i).at(j).ColorIndex != -1)
                    {
                        // j, i, level.at(i).at(j)
                        string id = to_string(colorint.at(level.at(i).at(j).ColorIndex));
                        string pl = to_string(level.at(i).at(j).isPlat);
                        cout << level.at(i).at(j).isPlat << endl;
                        string bg = level.at(i).at(j).isBack ? "1" : "0";
                        string x = to_string(j - 250);
                        string y = to_string(i - 250);
                        lvl = lvl + id + ";" + x + ";" + y + ";" + bg + ";" + pl + "\"";
                    }
                }
            }
            lvl = lvl.substr(0,lvl.length()-1);
            ofstream outputFile("level");
            outputFile << lvl;
            outputFile.close();
            break;
        }

        DrawRectangle(0,
                      0,
                      800,
                      600,
                      (Color){0, 0, 0, 32});

        for (i = 0; i < 500; i++)
        {
            int j;
            for (j = 0; j < 500; j++)
            {
                if (level.at(i).at(j).ColorIndex != -1)
                {
                    vector<Color> c;
                    if (level.at(i).at(j).isBack)
                    {
                        c = darkcolors;
                    }
                    else
                    {
                        c = colors;
                    }
                    DrawRectangle(((j) * 20) - off.x, ((i) * 20) - off.y, 20, 20, c.at(level.at(i).at(j).ColorIndex));
                }
            }
        }
        DrawRectangleLines((cur.x * 20) - off.x, (cur.y * 20) - off.y, 20, 20, WHITE);

        DrawRectangleV((Vector2){player.x - off.x,player.y - off.y}, pSize, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}