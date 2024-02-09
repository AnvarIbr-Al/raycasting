#include <iostream>
#include <cmath>
#include <Windows.h>

using namespace std;

struct Vec2 {
    float x, y;
    Vec2(float _x = 0, float _y = 0) : x(_x), y(_y) {}
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

    wchar_t map[16][16] = {
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

    Vec2 operator+(const Vec2& other) const {
        return { x + other.x, y + other.y };
    }

    Vec2 operator*(float scalar) const {
        return { x * scalar, y * scalar };
    }

    Vec2 Rotate(float angle) const {
        return {
            x * cos(angle) - y * sin(angle),
            x * sin(angle) + y * cos(angle)
        };
    }

    void Render3D() {
        for (int x = 0; x < nScreenWidth; x++) {
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + (static_cast<float>(x) / nScreenWidth) * fFOV;
            Vec2 rayDir = { cosf(fRayAngle), sinf(fRayAngle) };

            for (float fDistanceToWall = 0.0f; fDistanceToWall < fDepth; fDistanceToWall += 0.1f) {
                Vec2 testPos = playerPos + rayDir * fDistanceToWall;
                int nTestX = static_cast<int>(testPos.x);
                int nTestY = static_cast<int>(testPos.y);

                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                    break;
                else if (map[nTestY][nTestX] == '#') {
                    int nCeiling = static_cast<int>(nScreenHeight / 2.0 - nScreenHeight / fDistanceToWall);
                    int nFloor = nScreenHeight - nCeiling;

                    wchar_t nShade = (fDistanceToWall <= fDepth / 4.0f) ? 0x2588 :
                        (fDistanceToWall < fDepth / 3.0f) ? 0x2593 :
                        (fDistanceToWall < fDepth / 2.0f) ? 0x2592 :
                        (fDistanceToWall < fDepth) ? 0x2591 : ' ';

                    for (int y = 0; y < nScreenHeight; y++) {
                        if (y <= nCeiling)
                            wcout << L' ';
                        else if (y > nCeiling && y <= nFloor)
                            wcout << nShade;
                        else {
                            float b = 1.0f - ((static_cast<float>(y) - nScreenHeight / 2.0f) / (nScreenHeight / 2.0f));
                            nShade = (b < 0.25) ? '#' : (b < 0.5) ? 'x' : (b < 0.75) ? '.' : (b < 0.9) ? '-' : ' ';
                            wcout << nShade;
                        }
                    }
                    break;
                }
            }
        }
    }

    void HandleInput(float fElapsedTime) {
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
            fPlayerA -= (fSpeed * 0.75f) * fElapsedTime;

        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
            fPlayerA += (fSpeed * 0.75f) * fElapsedTime;

        Vec2 moveDir = { sinf(fPlayerA), cosf(fPlayerA) };
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
            playerPos = playerPos + moveDir * fSpeed * fElapsedTime;
            if (map[static_cast<int>(playerPos.y)][static_cast<int>(playerPos.x)] == '#')
                playerPos = playerPos - moveDir * fSpeed * fElapsedTime;
        }

        if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
            playerPos = playerPos - moveDir * fSpeed * fElapsedTime;
            if (map[static_cast<int>(playerPos.y)][static_cast<int>(playerPos.x)] == '#')
                playerPos = playerPos + moveDir * fSpeed * fElapsedTime;
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
