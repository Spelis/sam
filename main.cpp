#include "raylib.h"
#include "vector"
#include "string"
#include <iostream>
#include <sstream>

using namespace std;

Vector2 player = {0, 0};
Vector2 pSize = {20,40};
Vector2 veloc = {0,0};
vector<string> level = {"0;1;1","1;2;2","2;6;6"};
vector<Color> colors = {GRAY,BROWN,GREEN,BLUE,BLACK};

vector<string> splitString(const string& input, char delimiter) {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


int main() {
    float delta = GetFrameTime();
    InitWindow(800,600,"SAM");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Input

        if (IsKeyDown(KEY_A)) {
            veloc.x -= 0.1;
        }
        if (IsKeyDown(KEY_D)) {
            veloc.x += 0.1;
        }
        if (IsKeyPressed(KEY_SPACE)) {
            veloc.y = -5;
        }


        // STOP Input

        ClearBackground(BLACK);
        BeginDrawing();

        DrawRectangleV(player,pSize,LIGHTGRAY);
        bool collidingXM = false;
        bool collidingXP = false;
        bool collidingYM = false;
        bool collidingYP = false;
        Rectangle XM = {player.x,player.y+8,1,pSize.y-16};
        Rectangle XP = {player.x+pSize.x,player.y+8,1,pSize.y-16};
        Rectangle YM = {player.x+8,player.y,pSize.x-16,1};
        Rectangle YP = {player.x+8,player.y+pSize.y-1,pSize.x-16,1};
        for (int i=0; i<level.size(); i++) {
            string block = level.at(i);
            vector<string> blockinfo = splitString(block,';');
            Color color = colors.at(stoi(blockinfo.at(0)));
            Rectangle blockrec = {stoi(blockinfo.at(1))*20, stoi(blockinfo.at(2))*20,20,20};
            DrawRectangleRec(blockrec,color);
        }
        for (int i = 0; i < level.size(); i++) {
            string block = level.at(i);
            vector<string> blockinfo = splitString(block, ';');
            Rectangle blockrec = {stoi(blockinfo.at(1)) * 20, stoi(blockinfo.at(2)) * 20, 20, 20};


        // Check for collision
            if (CheckCollisionRecs(XM, blockrec)) {
                collidingXM = true;
                break; // Exit the loop once a collision is found
            }
            if (CheckCollisionRecs(XP, blockrec)) {
                collidingXP = true;
                break; // Exit the loop once a collision is found
            }
            if (CheckCollisionRecs(YM, blockrec)) {
                collidingYM = true;
                break; // Exit the loop once a collision is found
            }
            if (CheckCollisionRecs(YP, blockrec)) {
                collidingYP = true;
                break; // Exit the loop once a collision is found
            }
        }
        veloc.y += 0.1;
        veloc.x *= 0.98;
    player.x += veloc.x;
    player.y += veloc.y;

// Resolve collisions after applying velocity
    if (collidingXM) {
        veloc.x = 0;
        player.x++;
    }
    if (collidingXP) {
        veloc.x = 0;
        player.x--;
    }
    if (collidingYM) {
        veloc.y = 0;
        player.y++;
    }
    if (collidingYP) {
        veloc.y = 0;
        player.y--;
    }



        EndDrawing();
    }

    CloseWindow();

    return 0;
}
