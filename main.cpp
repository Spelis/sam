#include "raylib.h"
#include "vector"
#include "string"
#include <iostream>
#include <sstream>
#include "stdlib.h"
#include "time.h"
#include <thread>
#include <fstream>
#include <cctype>
#include <any>
#include <cmath>

using namespace std;

Vector2 camPos = {0, 0}; // camera position
class Creature
{
public:
    int health;
    int canDJ;
    int jump;
    int atk;
    int dashCD;
    Vector2 player;
    Vector2 pSize;
    Vector2 veloc;

    void Draw(Color c)
    {
        DrawRectangleV((Vector2){player.x - camPos.x + 390, player.y - camPos.y + 290}, pSize, c);
    }
};

vector<Creature> enemies = {};
void newZombie(Vector2 sPos, int sH, int DJ)
{
    if (enemies.size() < 10) {
    Creature z;
    z.player = sPos;    // player position
    z.pSize = {20, 40}; // player size
    z.veloc = {0, 0};   // player velocity
    z.health = sH;      // player health
    z.canDJ = DJ;       // double jumping
    z.jump = 0;         // double jump check
    z.atk = 0;
    z.dashCD = 0;
    enemies.push_back(z);
    }
}

Creature player;

const int atkRange = 50;
bool plDown = false;

std::string join(const std::vector<std::string> &v, const std::string &delimiter)
{
    std::string out;
    if (!v.empty())
    {
        out += v[0];
        for (size_t i = 1; i < v.size(); ++i)
        {
            out += delimiter + v[i];
        }
    }
    return out;
}
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
vector<string> loadLevelfromfile(string filename)
{
    ifstream inputFile(filename);
    string content((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    vector<string> result = splitString(content, '"');
    return result;
}
void loadSave(int levelId)
{
    vector<string> vec = splitString(loadLevelfromfile("save").at(levelId - 1), ';');
    player.player = {stof(vec.at(0)), stof(vec.at(1))};
    player.veloc = {stof(vec.at(2)), stof(vec.at(3))};
    player.canDJ = stoi(vec.at(5));
    enemies = {};
}
void saveSave(int levelId)
{
    vector<string> savelevel = loadLevelfromfile("save");
    if (savelevel.size() < levelId)
    {
        savelevel.resize(levelId);
    }
    savelevel.at(levelId - 1).resize(50);
    sprintf(&savelevel.at(levelId - 1)[0], "%.2f;%.2f;%.2f;%.2f;%d;%d;S%d", player.player.x, player.player.y, player.veloc.x, player.veloc.y, player.health, player.canDJ, levelId);

    ofstream outputFile("save");
    outputFile << join(savelevel, "\"");
    outputFile.close();
}
void reset() {
    player.player = {0, 0};  // player position
    player.pSize = {20, 40}; // player size
    player.veloc = {0, 0};   // player velocity
    player.health = 100;     // player health
    player.canDJ = 0;        // double jumping
    player.jump = 0;         // double jump check
    player.atk = 0;
    player.dashCD = 0;
    enemies = {};

}
float lerp(float start, float end, float t)
{
    return start + (end - start) * t;
}

int main()
{
    reset();

    int saveInt;
    if (loadLevelfromfile("save").size() != 0)
    {
        cout << "Save: ";
        cin >> saveInt;
        loadSave(saveInt);
    }

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(800, 600, "SAM");
    srand(time(NULL));

    Texture2D stone = LoadTexture("assets/stone.png");
    Texture2D dirt = LoadTexture("assets/dirt.png");
    Texture2D grass = LoadTexture("assets/grass.png");
    Texture2D water = LoadTexture("assets/water.png");
    Texture2D obsidian = LoadTexture("assets/obsidian.png");

    vector<Texture2D> textures = {stone, dirt, grass, water, obsidian};

    vector<string> level = loadLevelfromfile("level");
    float delta;
    bool skipFrame = false;
    SetTargetFPS(600);
    int frame = 0;

    while (!WindowShouldClose())
    {
        if (player.health <= 0) {
            reset();
        }
        frame++;
        delta = GetFrameTime() * 600;
        DrawText(("FPS: " + to_string(GetFPS())).c_str(), 10, 80, 20, BLACK);
        if (skipFrame)
        {
            skipFrame = false;
            delta = 1;
        }
        if (player.dashCD > 0)
        {
            player.dashCD--;
        }
        // Input

        if (IsKeyDown(KEY_A))
        {
            player.veloc.x -= 0.02;
        }
        if (IsKeyDown(KEY_D))
        {
            player.veloc.x += 0.02;
        }
        if (IsKeyPressed(KEY_SPACE) && player.jump > 0)
        {
            player.player.y -= 2;
            player.veloc.y = -1;
            player.jump--;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            player.atk = 100;
        }
        if (IsKeyPressed(KEY_F))
        { // save
            cout << "Save: ";
            cin >> saveInt;
            saveSave(saveInt);
            skipFrame = true;
        }
        if (IsKeyPressed(KEY_R))
        {
            cout << "Save: ";
            cin >> saveInt;
            loadSave(saveInt);
            skipFrame = true;
        }
        if (IsKeyPressed(KEY_Z))
        {
            newZombie((Vector2){0, 0}, 100, 0);
        }
        if (IsKeyDown(KEY_RIGHT_SHIFT) && player.dashCD == 0)
        {
            if (player.veloc.x > 0)
            {
                player.veloc.x = 2;
            }
            if (player.veloc.x < 0)
            {
                player.veloc.x = -2;
            }
            player.dashCD = 600;
        }
        if (player.dashCD > 0)
        {
            DrawText(to_string((player.dashCD / 6)).c_str(), 10, 40, 20, WHITE);
        }

        camPos.x = lerp(camPos.x, player.player.x, 0.02f);
        camPos.y = lerp(camPos.y, player.player.y, 0.02f);

        // STOP Input
        player.veloc.y += 0.005;

        player.veloc.x *= 0.97;

        BeginDrawing();
        ClearBackground(SKYBLUE);

        DrawText(to_string(player.health).c_str(), 10, 10, 20, RED);

        Rectangle pl = {player.player.x, player.player.y, player.pSize.x, player.pSize.y};
        bool collidingXM = false;
        bool collidingXP = false;
        bool collidingYM = false;
        bool collidingYP = false;
        bool jumpable = false;
        bool watery = false;
        Rectangle XM = {player.player.x, player.player.y + 16, 1, player.pSize.y - 32};
        Rectangle XP = {player.player.x + player.pSize.x, player.player.y + 16, 1, player.pSize.y - 32};
        Rectangle YM = {player.player.x + 8, player.player.y, player.pSize.x - 16, 1};
        Rectangle YP = {player.player.x + 8, player.player.y + player.pSize.y - 1, player.pSize.x - 16, 1};
        Rectangle jColl = {player.player.x + 8, player.player.y + player.pSize.y + 1, player.pSize.x - 16, 1};

        for (int i = 0; i < level.size(); i++)
        {
            string block = level.at(i);
            vector<string> blockinfo = splitString(block, ';');
            Texture2D texture = textures.at(stoi(blockinfo.at(0)));
            Rectangle blockrec = {stoi(blockinfo.at(1)) * 20.0f, stoi(blockinfo.at(2)) * 20.0f, 20, 20};
            int isPlat = stoi(blockinfo.at(4));
            unsigned char a;
            unsigned char c;
            if (blockinfo.at(0) != "3")
            {
                a = 255;
            }
            else
            {
                a = 128;
            }
            if (blockinfo.at(3) == "0")
            {
                continue;
            }
            else
            {
                c = 128;
            }
            DrawTextureRec(texture, (Rectangle){0, 0, 20, 20.0f / (isPlat + 1)}, (Vector2){blockrec.x - camPos.x + 390, blockrec.y - camPos.y + 290}, (Color){c, c, c, a});
        }
        player.Draw(LIGHTGRAY);
        for (int i = 0; i < enemies.size(); i++)
        {
            enemies.at(i).Draw(RED);
        }
        for (int i = 0; i < level.size(); i++)
        {
            string block = level.at(i);
            vector<string> blockinfo = splitString(block, ';');
            Texture2D texture = textures.at(stoi(blockinfo.at(0)));
            Rectangle blockrec = {stoi(blockinfo.at(1)) * 20.0f, stoi(blockinfo.at(2)) * 20.0f, 20, 20};
            int isPlat = stoi(blockinfo.at(4));
            unsigned char a;
            unsigned char c;
            if (blockinfo.at(0) != "3")
            {
                a = 255;
            }
            else
            {
                a = 128;
            }
            if (blockinfo.at(3) == "0")
            {
                c = 255;
            }
            else
            {
                continue;
            }
            // DrawTexturePro(texture, , );
            DrawTextureRec(texture, (Rectangle){0, 0, 20, 20.0f / (isPlat + 1)}, (Vector2){blockrec.x - camPos.x + 390, blockrec.y - camPos.y + 290}, (Color){c, c, c, a});
            if ((isPlat + 1) == 2)
            {
                c = c / 2;
                DrawTextureRec(texture, (Rectangle){0, 10.0f, 20, 10.0f}, (Vector2){blockrec.x - camPos.x + 390, blockrec.y - camPos.y + 300}, (Color){c, c, c, a});
            }
        }
        for (int i = 0; i < level.size(); i++)
        {
            string block = level.at(i);
            vector<string> blockinfo = splitString(block, ';');
            Rectangle blockrec = {stoi(blockinfo.at(1)) * 20.0f, stoi(blockinfo.at(2)) * 20.0f, 20, 20};
            int blocktype = stoi(blockinfo.at(0));
            int isbg = stoi(blockinfo.at(3));
            int isPlat = stoi(blockinfo.at(4));

            // XM clipping
            if (isbg != 1)
            {
                if (blocktype != 3)
                {
                    int distXM = blockrec.x - player.player.x;
                    int distXP = player.player.x + player.pSize.x - blockrec.x;
                    int distYM = blockrec.y - player.player.y;
                    int distYP = player.player.y + player.pSize.y - blockrec.y;
                    if (distXM < player.veloc.x)
                    {
                        if (distYP < 20 && distYM < 20)
                        {
                            player.veloc.x = distXM;
                        }
                    }
                    if (distXP > player.veloc.x)
                    {
                        if (distYP < 20 && distYM < 20)
                        {
                            player.veloc.x = distXM;
                        }
                    }
                    if (distYM < player.veloc.y)
                    {
                        if (distYP < 20 && distYM < 20)
                        {
                            player.veloc.y = distYM;
                        }
                    }
                    if (distYP < player.veloc.y)
                    {
                        if (distYP < 40 && distYM < 20)
                        {
                            player.veloc.y = distYM;
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
                    if (CheckCollisionRecs(YM, blockrec) && isPlat == 0)
                    {
                        collidingYM = true;
                    }
                    if (CheckCollisionRecs(YP, blockrec))
                    {
                        collidingYP = true;
                        if (isPlat == 1 && (IsKeyDown(KEY_S) || plDown))
                        {
                            collidingYP = false;
                            plDown = true;
                        }
                        else
                        {
                            plDown = false;
                        }
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
        if (player.atk > 0)
        {
            Rectangle atkRect;
            if (player.veloc.x > 0)
            {
                atkRect = {player.player.x + 390 - camPos.x + player.pSize.x, player.player.y + 290 - camPos.y + 10, atkRange, 20};
            }
            else if (player.veloc.x < 0)
            {
                atkRect = {player.player.x + 390 - camPos.x - atkRange, player.player.y + 290 - camPos.y + 10, atkRange, 20};
            }
            else
            {
                atkRect = {player.player.x + 390 - camPos.x, player.player.y + 290 - camPos.y + 10, player.pSize.x, player.pSize.y};
            }
            DrawRectangleRec(atkRect, WHITE);
            DrawRectangle(0, 0, 20, 20, WHITE);

            DrawText(to_string(player.atk).c_str(), 10, 40, 20, WHITE);
            player.atk--;
        }

        if (!watery)
        {
            player.player.y += player.veloc.y * delta;
            player.player.x += player.veloc.x * delta;
        }
        else
        {
            player.player.y += player.veloc.y * 0.5 * delta;
            player.player.x += player.veloc.x * 0.5 * delta;
        }

        // Resolve collisions after applying velocity for player
        if (collidingXM)
        {
            player.veloc.x = 0;
            player.player.x += 1 * delta;
        }
        if (collidingXP)
        {
            player.veloc.x = 0;
            player.player.x -= 1 * delta;
        }
        if (collidingYM)
        {
            player.veloc.y = 0;
            player.player.y += 1 * delta;
        }
        if (collidingYP)
        {
            if (player.veloc.y > 0)
            {
                player.veloc.y = 0;
            }
            player.player.y -= 1 * delta;
        }
        if (jumpable && player.canDJ)
        {
            player.jump = 2;
        }
        else if (!jumpable && player.canDJ && player.jump == 2)
        {
            player.jump = 1;
        }
        if (jumpable && !player.canDJ)
        {
            player.jump = 1;
        }
        else if (!jumpable && !player.canDJ)
        {
            player.jump = 0;
        }

        // Resolve collisions and apply gravity for enemies
        for (int i = 0; i < enemies.size(); i++)
        {
            Creature &enemy = enemies[i];
            Rectangle enemyRect = {enemy.player.x, enemy.player.y, enemy.pSize.x, enemy.pSize.y};
            if (frame % 20 == 0) {
            if (CheckCollisionRecs(enemyRect,pl)) {
                player.health--;
            }}
            if (frame % 100 == 0)
            {
                int input = rand() % 3;
                if (input == 0)
                {
                    enemy.veloc.x -= 0.02;
                }
                if (input == 1)
                {
                    enemy.veloc.x += 0.02;
                }
                if (input == 2 && enemy.jump > 0)
                {
                    enemy.player.y -= 2;
                    enemy.veloc.y = -0.7;
                    enemy.jump--;
                }
            }
            bool enemyCollidingXM = false;
            bool enemyCollidingXP = false;
            bool enemyCollidingYM = false;
            bool enemyCollidingYP = false;
            bool enemyJumpable = false;
            bool enemyWatery = false;
            Rectangle enemyXM = {enemy.player.x, enemy.player.y + 16, 1, enemy.pSize.y - 32};
            Rectangle enemyXP = {enemy.player.x + enemy.pSize.x, enemy.player.y + 16, 1, enemy.pSize.y - 32};
            Rectangle enemyYM = {enemy.player.x + 8, enemy.player.y, enemy.pSize.x - 16, 1};
            Rectangle enemyYP = {enemy.player.x + 8, enemy.player.y + enemy.pSize.y - 1, enemy.pSize.x - 16, 1};
            Rectangle enemyJColl = {enemy.player.x + 8, enemy.player.y + enemy.pSize.y + 1, enemy.pSize.x - 16, 1};

            for (int j = 0; j < level.size(); j++)
            {
                string block = level.at(j);
                vector<string> blockinfo = splitString(block, ';');
                Rectangle blockrec = {stoi(blockinfo.at(1)) * 20.0f, stoi(blockinfo.at(2)) * 20.0f, 20, 20};
                int blocktype = stoi(blockinfo.at(0));
                int isbg = stoi(blockinfo.at(3));
                int isPlat = stoi(blockinfo.at(4));

                if (isbg != 1)
                {
                    if (blocktype != 3)
                    {
                        if (CheckCollisionRecs(enemyXM, blockrec))
                        {
                            enemyCollidingXM = true;
                        }
                        if (CheckCollisionRecs(enemyXP, blockrec))
                        {
                            enemyCollidingXP = true;
                        }
                        if (CheckCollisionRecs(enemyYM, blockrec) && isPlat == 0)
                        {
                            enemyCollidingYM = true;
                        }
                        if (CheckCollisionRecs(enemyYP, blockrec))
                        {
                            enemyCollidingYP = true;
                        }
                    }
                    else if (CheckCollisionRecs(enemyRect, blockrec))
                    {
                        enemyWatery = true;
                    }
                    if (CheckCollisionRecs(enemyJColl, blockrec) || enemyWatery)
                    {
                        enemyJumpable = true;
                    }
                }
            }

            if (!enemyWatery)
            {
                enemy.player.y += enemy.veloc.y * delta;
                enemy.player.x += enemy.veloc.x * delta;
            }
            else
            {
                enemy.player.y += enemy.veloc.y * 0.5 * delta;
                enemy.player.x += enemy.veloc.x * 0.5 * delta;
            }

            // Resolve collisions after applying velocity for enemies
            if (enemyCollidingXM)
            {
                enemy.veloc.x = 0;
                enemy.player.x += 1 * delta;
            }
            if (enemyCollidingXP)
            {
                enemy.veloc.x = 0;
                enemy.player.x -= 1 * delta;
            }
            if (enemyCollidingYM)
            {
                enemy.veloc.y = 0;
                enemy.player.y += 1 * delta;
            }
            if (enemyCollidingYP)
            {
                if (enemy.veloc.y > 0)
                {
                    enemy.veloc.y = 0;
                }
                enemy.player.y -= 1 * delta;
            }
            if (enemyJumpable && enemy.canDJ)
            {
                enemy.jump = 2;
            }
            else if (!enemyJumpable && enemy.canDJ && enemy.jump == 2)
            {
                enemy.jump = 1;
            }
            if (enemyJumpable && !enemy.canDJ)
            {
                enemy.jump = 1;
            }
            else if (!enemyJumpable && !enemy.canDJ)
            {
                enemy.jump = 0;
            }
            enemy.veloc.y += 0.005;
            enemy.veloc.x *= 0.97;
            enemy.player.y += enemy.veloc.y * delta;
            enemy.player.x += enemy.veloc.x * delta;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
