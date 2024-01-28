#include <iostream>
#include <cmath>
#include <Windows.h>

using namespace std;

struct Vec2 {
    float x, y;
    Vec2(float _x = 0, float _y = 0) : x(_x), y(_y) {}

    Vec2 operator+(const Vec2& other) const {
        return { x + other.x, y + other.y };
    }

    Vec2 operator-(const Vec2& other) const {
        return { x - other.x, y - other.y };
    }

    Vec2 operator*(float scalar) const {
        return { x * scalar, y * scalar };
    }
};

struct Game {
    int nScreenWidth = 120;
    int nScreenHeight = 40;
    int nMapWidth = 16;
    int nMapHeight = 16;

    Vec2 playerPos = { 14.7f, 5.09f };
    float fPlayerA = 0.0f;
    float fFOV = 3.14159f / 4.0f;
    float fDepth = 16.0f;
    float fSpeed = 5.0f;

    wchar_t map[17][17] = {
        L"#########.......",
        L"#...............",
        L"#.......########",
        L"#..............#",
        L"#......#.......#",
        L"#.....###......#",
        L"#....#####.....#",
        L"###.#######....#",
        L"##.............#",
        L"#......####..###",
        L"#......#.......#",
        L"################",
        L"#..............#",
        L"#......#########",
        L"#..............#",
        L"################"
    };

    Game() {}
    void HandleInput(float fElapsedTime) {
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
            fPlayerA -= (fSpeed * 0.75f) * fElapsedTime;

        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
            fPlayerA += (fSpeed * 0.75f) * fElapsedTime;

        Vec2 moveDir = { sinf(fPlayerA), cosf(fPlayerA) };
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
            Vec2 newPlayerPos = playerPos + moveDir * fSpeed * fElapsedTime;
            int nNewPlayerPosX = static_cast<int>(newPlayerPos.x);
            int nNewPlayerPosY = static_cast<int>(newPlayerPos.y);
            if (map[nNewPlayerPosY][nNewPlayerPosX] != '#')
                playerPos = newPlayerPos;
        }

        if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
            Vec2 newPlayerPos = playerPos - moveDir * fSpeed * fElapsedTime;
            int nNewPlayerPosX = static_cast<int>(newPlayerPos.x);
            int nNewPlayerPosY = static_cast<int>(newPlayerPos.y);
            if (map[nNewPlayerPosY][nNewPlayerPosX] != '#')
                playerPos = newPlayerPos;
        }
    }

    void Render3D() {
        const float fWallScalingFactor = 1.5f;

        for (int x = 0; x < nScreenWidth; x++) {
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + (static_cast<float>(x) / nScreenWidth) * fFOV;
            Vec2 rayDir = { cosf(fRayAngle), sinf(fRayAngle) };
            Vec2 rayPos = playerPos;
            float fDistanceToWall = 0.0f;
            while (fDistanceToWall < fDepth) {
                rayPos = rayPos + rayDir;
                int nTestX = static_cast<int>(rayPos.x);
                int nTestY = static_cast<int>(rayPos.y);
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
                    fDistanceToWall = fDepth;
                    break;
                }

                if (map[nTestY][nTestX] == '#') {
                    Vec2 wallPos = rayPos - playerPos;
                    fDistanceToWall = wallPos.x / cosf(fRayAngle);
                    break;
                }
            }
            int nCeiling = static_cast<int>(nScreenHeight / 2.0 - nScreenHeight / (fDistanceToWall * fWallScalingFactor));
            int nFloor = nScreenHeight - nCeiling;
            for (int y = 0; y < nScreenHeight; y++) {
                wchar_t nShade;
                if (y < nCeiling)
                    nShade = L' '; 
                else if (y >= nCeiling && y <= nFloor)
                    nShade = L'#'; 
                else
                    nShade = L' '; 
                wcout << nShade;
            }
            wcout << endl;
        }
    }

};

int main()
{
    Game game;

    while (1) {
        float fElapsedTime = 0.1f;
        game.HandleInput(fElapsedTime);
        game.Render3D();
    }

    return 0;
}
