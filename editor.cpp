#include "raylib.h"
#include "vector"
#include "string"
#include <iostream>
#include <algorithm>
#include <any>

using namespace std;

struct Block
{
    bool isBack;
    int ColorIndex;
};

Vector2 cur = {0, 0};
Vector2 player = {390, 290};
Vector2 pSize = {20, 40};
int color = 0;
vector<Color> colors = {GRAY, BROWN, GREEN, BLUE, DARKPURPLE};
vector<Color> darkcolors = {DARKGRAY, DARKBROWN, DARKGREEN, DARKBLUE, DARKPURPLE};
vector<int> colorint = {0, 1, 2, 3, 4};
vector<vector<Block>> level = {};
bool back = false;

int main()
{
    int i;
    for (i = 0; i < 30; i++)
    {
        level.push_back({});
        int j;
        for (j = 0; j < 40; j++)
        {
            level.at(i).push_back(Block{false, -1});
        }
    }
    InitWindow(800, 600, "SAM Level Editor");
    SetTargetFPS(20);

    BeginDrawing();

    while (!WindowShouldClose())
    {
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
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            color = (color + 1) % colors.size();
            DrawRectangle(10, 10, 20, 20, colors.at(color));
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            back = !back;
            char *bstr;
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
        if (IsKeyPressed(KEY_ENTER))
        {
            level.at(cur.y).at(cur.x) = {back, color};
        }
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            level.at(cur.y).at(cur.x) = {false, -1};
        }
        if (IsKeyPressed(KEY_F))
        {
            cout << "Saving...\n";
            string lvl;
            lvl = "{";
            int i;
            for (i = 0; i < 30; i++)
            {
                int j;
                for (j = 0; j < 40; j++)
                {
                    if (level.at(i).at(j).ColorIndex != -1)
                    {
                        // j, i, level.at(i).at(j)
                        string id = to_string(colorint.at(level.at(i).at(j).ColorIndex));
                        string bg = level.at(i).at(j).isBack ? "1" : "0";
                        string x = to_string(j);
                        string y = to_string(i);
                        lvl = lvl + "\"" + id + ";" + x + ";" + y + ";" + bg + "\", ";
                    }
                }
            }
            lvl = lvl + "}";
            cout << "Saved!\n";
            cout << lvl << "\n";
            break;
        }

        DrawRectangle(0,
                      0,
                      800,
                      600,
                      (Color){0, 0, 0, 32});

        for (i = 0; i < 30; i++)
        {
            int j;
            for (j = 0; j < 40; j++)
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
                    DrawRectangle(j * 20, i * 20, 20, 20, c.at(level.at(i).at(j).ColorIndex));
                }
            }
        }
        DrawRectangleLines(cur.x * 20, cur.y * 20, 20, 20, WHITE);

        DrawRectangleV(player, pSize, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}