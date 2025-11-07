#include <windows.h>
#include <winuser.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mmsystem.h>
#include <windows.h>

#pragma comment(lib, "winmm.lib")
#define _WIN32_WINNT 0x0501
#pragma comment(lib, "User32.lib")
#define PI 3.14159265f

int Layar_w_Actual;
int Layar_H_Actual;

const int Max_musuh = 3;
const int Max_musuh2 = 4;
const int Max_Peluru = 100;
const int Max_Bintang = 100;

const int Player_SC = 3;
const float PLAYER_SPEED = 5.0f;
const int Musuh_SC = 15;
const int Musuh2_SC = 10;

float gameTime = 0.0f;
float lastShotSoundTime = 0.0f;
const float SHOT_SOUND_DELAY = 0.1f;
const float FRAME_RATE_IN_SECONDS = 0.01f;
float lastShotTime_Mode1 = 0.0f;
float lastShotTime_Mode2 = 0.0f;
const float SHOT_COOLDOWN = 0.05f;

bool isBeamFiring = false;
bool isLaserFullSoundPlaying = false;
bool isGameOverSoundPlaying = false;
bool isMenuSoundPlaying = false;

enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER
};

struct Object {
    float x, y;
    float speed;
    float vx, vy;
    int arah;
    int SC;
    int posisiY;
    bool aktif;
    int bulletType;

    int weight, height;
};

GameState currentState;
Object player;
float TA_player = 0.0f;
Object musuh[Max_musuh];
Object musuh2[Max_musuh2];
Object peluru[Max_Peluru];
Object bintang[Max_Bintang];

int score;
int nyawa;
char scoreText[10];
char nyawaText[10];

void PesawatMusuh(int x, int y, int sc, int posisi, int arah, int baseColor, int lineColor)
{
    y -= posisi;

    int bodyPoints[] = {
        (0*sc)*arah + x, (0*sc)+y,
        (-2*sc)*arah + x, (-1*sc)+y,
        (-5*sc)*arah + x, (-1*sc)+y,
        (int)((-6.0*sc)*arah) + x, (int)((-0.5*sc)+y),
        (int)((-7.0*sc)*arah) + x, (int)((-3.0*sc)+y),
        (int)((-7.0*sc)*arah) + x, (int)((1.0*sc)+y),
        (int)((-6.0*sc)*arah) + x, (int)((0.5*sc)+y),
        (-5*sc)*arah + x, (1*sc)+y,
        (-5*sc)*arah + x, (0*sc)+y,
        (-3*sc)*arah + x, (0*sc)+y,
        (-2*sc)*arah + x, (1*sc)+y,
        (0*sc)*arah + x, (0*sc)+y
    };
    int numBodyPoints = sizeof(bodyPoints) / sizeof(bodyPoints[0]) / 2;

    int wingTopPoints[] = {
        (-3*sc)*arah + x, (-1*sc)+y,
        (-5*sc)*arah + x, (-2*sc)+y,
        (-5*sc)*arah + x, (-1*sc)+y,
        (-3*sc)*arah + x, (-1*sc)+y
    };
    int numWingTopPoints = sizeof(wingTopPoints) / sizeof(wingTopPoints[0]) / 2;

    int wingBottomPoints[] = {
        (-3*sc)*arah + x, (0*sc)+y,
        (-5*sc)*arah + x, (3*sc)+y,
        (-5*sc)*arah + x, (0*sc)+y,
        (-3*sc)*arah + x, (0*sc)+y
    };
    int numWingBottomPoints = sizeof(wingBottomPoints) / sizeof(wingBottomPoints[0]) / 2;

    setfillstyle(SOLID_FILL, baseColor);
    fillpoly(numBodyPoints, bodyPoints);
    fillpoly(numWingTopPoints, wingTopPoints);
    fillpoly(numWingBottomPoints, wingBottomPoints);

    setcolor(lineColor);
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);

    drawpoly(numBodyPoints, bodyPoints);
    drawpoly(numWingTopPoints, wingTopPoints);
    drawpoly(numWingBottomPoints, wingBottomPoints);
}

void PesawatMusuh2(int x, int y, int sc, int posisi, int arah, int baseColor, int lineColor)
{
    y -= posisi;

    int leftSidePoints[] = {
        (0*sc)*arah + x, (int)((-4.0*sc)+y),
        (int)((-1.0*sc)*arah) + x, (int)((-4.0*sc)+y),
        (int)((-1.0*sc)*arah) + x, (int)((-6.0*sc)+y),
        (int)((-3.0*sc)*arah) + x, (int)((-6.0*sc)+y),
        (int)((-3.0*sc)*arah) + x, (int)((-3.5*sc)+y),
        (int)((-1.0*sc)*arah) + x, (int)((-4.0*sc)+y),
        (int)((-4.0*sc)*arah) + x, (int)((-3.0*sc)+y),
        (int)((-5.0*sc)*arah) + x, (int)((-5.0*sc)+y),
        (int)((-5.0*sc)*arah) + x, (int)((-3.0*sc)+y),
        (int)((-4.0*sc)*arah) + x, (int)((0.0*sc)+y),
        (int)((-2.0*sc)*arah) + x, (int)((4.0*sc)+y),
        (int)((0.0*sc)*arah) + x, (int)((4.5*sc)+y),
        (int)((0.0*sc)*arah) + x, (int)((-4.0*sc)+y)
    };
    int numLeftSidePoints = sizeof(leftSidePoints) / sizeof(leftSidePoints[0]) / 2;

    int rightSidePoints[] = {
        (0*sc)*arah + x, (int)((-4.0*sc)+y),
        (int)((1.0*sc)*arah) + x, (int)((-4.0*sc)+y),
        (int)((1.0*sc)*arah) + x, (int)((-6.0*sc)+y),
        (int)((3.0*sc)*arah) + x, (int)((-6.0*sc)+y),
        (int)((3.0*sc)*arah) + x, (int)((-3.5*sc)+y),
        (int)((1.0*sc)*arah) + x, (int)((-4.0*sc)+y),
        (int)((4.0*sc)*arah) + x, (int)((-3.0*sc)+y),
        (int)((5.0*sc)*arah) + x, (int)((-5.0*sc)+y),
        (int)((5.0*sc)*arah) + x, (int)((-3.0*sc)+y),
        (int)((4.0*sc)*arah) + x, (int)((0.0*sc)+y),
        (int)((2.0*sc)*arah) + x, (int)((4.0*sc)+y),
        (int)((0.0*sc)*arah) + x, (int)((4.5*sc)+y),
        (int)((0.0*sc)*arah) + x, (int)((-4.0*sc)+y)
    };
    int numRightSidePoints = sizeof(rightSidePoints) / sizeof(rightSidePoints[0]) / 2;

    setfillstyle(SOLID_FILL, baseColor);
    fillpoly(numLeftSidePoints, leftSidePoints);
    fillpoly(numRightSidePoints, rightSidePoints);

    setcolor(lineColor);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

    drawpoly(numLeftSidePoints, leftSidePoints);
    drawpoly(numRightSidePoints, rightSidePoints);

    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
}

void PesawatPlayer(int x, int y, int sc, float angle)
{
    const int JUMLAH_TITIK = 31;
    float basePointsX[JUMLAH_TITIK] = {
        0.0f, 5.0f, 8.0f, 10.0f, 10.0f,  2.0f,  -5.0f, -10.0f, -12.0f, -20.0f, -15.0f, -18.0f, -20.0f, -25.0f, -35.0f, -40.0f,
        -35.0f, -25.0f, -20.0f, -18.0f, -15.0f, -20.0f, -12.0f, -10.0f,  -5.0f,  2.0f, 10.0f, 8.0f, 5.0f,  0.0f
    };

    float basePointsY[JUMLAH_TITIK] = {
        0.0f,  5.0f,  8.0f,  12.0f,  10.0f, 15.0f, 25.0f, 22.0f, 15.0f, 18.0f, 12.0f,  6.0f,  4.0f,  2.0f,  1.0f,  0.0f,
       -1.0f, -2.0f, -4.0f, -6.0f, -12.0f, -18.0f, -15.0f, -22.0f, -25.0f, -15.0f, -10.0f, -8.0f, -5.0f,  0.0f
    };

    int bentukPesawat[JUMLAH_TITIK * 2];
    float c = cos(angle);
    float s = sin(angle);

    for(int i = 0; i < JUMLAH_TITIK; i++)
    {
        float rotX = basePointsX[i] * c - (-basePointsY[i]) * s;
        float rotY = basePointsX[i] * s + (-basePointsY[i]) * c;

        bentukPesawat[i*2]   = x + (int)(rotX * sc);
        bentukPesawat[i*2+1] = y + (int)(rotY * sc);
    }

    setfillstyle(SOLID_FILL, COLOR(173, 216, 230));
    fillpoly(JUMLAH_TITIK, bentukPesawat);

    setcolor(COLOR(0, 105, 170));
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

    drawpoly(JUMLAH_TITIK, bentukPesawat);

    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
}

bool tabrakan(Object& a, Object& b)
{
    float a_left = a.x - a.weight / 2.0;
    float a_right = a.x + a.weight / 2.0;
    float a_top = a.y - a.height / 2.0;
    float a_bottom = a.y + a.height / 2.0;

    float b_left = b.x - b.weight / 2.0;
    float b_right = b.x + b.weight / 2.0;
    float b_top = b.y - b.height / 2.0;
    float b_bottom = b.y + b.height / 2.0;

    if (a_left < b_right && a_right > b_left &&
        a_top < b_bottom && a_bottom > b_top)
    {
        return true;
    }
    return false;
}

void musuhMuncul(int i)
{
    if (rand() % 2 == 0) {
        musuh[i].x = 0 - (Musuh_SC * 7);
        musuh[i].arah = 1;
    }

     else {
        musuh[i].x = Layar_w_Actual + (Musuh_SC * 7);
        musuh[i].arah = -1;
    }

    musuh[i].y = (rand() % (Layar_H_Actual / 2)) + 50;
    musuh[i].speed = (rand() % 4) + 2;
    musuh[i].SC = Musuh_SC;
    musuh[i].posisiY = 0;
    musuh[i].aktif = true;

    musuh[i].weight = 7 * Musuh_SC;
    musuh[i].height = 6 * Musuh_SC;
}

void musuh2Muncul(int i)
{
    musuh2[i].y = -(rand() % 100) - 50;
    musuh2[i].SC = Musuh2_SC;
    musuh2[i].weight = 10 * Musuh2_SC;
    musuh2[i].height = 10.5 * Musuh2_SC;
    musuh2[i].aktif = true;
    float baseSpeed = (rand() % 3) + 2;

    int polaGerak = rand() % 3;

    if (polaGerak == 0)
    {
        musuh2[i].x = rand() % Layar_w_Actual;
        musuh2[i].vx = 0;
        musuh2[i].vy = baseSpeed;
    }
    else if (polaGerak == 1)
    {
        musuh2[i].x = 0 - (musuh2[i].weight / 2);
        musuh2[i].vx = baseSpeed * 0.707f;
        musuh2[i].vy = baseSpeed * 0.707f;
    }
    else
    {
        musuh2[i].x = Layar_w_Actual + (musuh2[i].weight / 2);
        musuh2[i].vx = -baseSpeed * 0.707f;
        musuh2[i].vy = baseSpeed * 0.707f;
    }
}

void Peluru(int type)
{
    float bulletSpeed = 15.0f;
    float c = cos(TA_player);
    float s = sin(TA_player);

    if (type == 1) // Tembak 3 Lingkaran
    {
        int count = 0;
        for (int i = 0; i < Max_Peluru; i++)
        {
            if (peluru[i].aktif == false)
            {
                peluru[i].aktif = true;
                peluru[i].bulletType = 1; //lingkaran

                float offsetX = 7.5f * player.SC;
                float offsetY = 0;
                if (count == 1) offsetY = 15 * player.SC; // Kanan
                else if (count == 2) offsetY = -15 * player.SC; // Kiri

                float rotX = offsetX * c - (-offsetY) * s;
                float rotY = offsetX * s + (-offsetY) * c;

                peluru[i].x = player.x + rotX;
                peluru[i].y = player.y + rotY;

                peluru[i].vx = -c * bulletSpeed;
                peluru[i].vy = -s * bulletSpeed;

                peluru[i].weight = 3 * player.SC;
                peluru[i].height = 3 * player.SC;

                count++;
                if (count == 3) break;
            }
        }
        //PlaySound(TEXT("laserps.wav"), NULL, SND_ASYNC | SND_FILENAME);
        mciSendString("play laserSound from 0", NULL, 0, NULL);
    }
    else if (type == 2) //Tembak 1 Laser 3px
    {
        for (int i = 0; i < Max_Peluru; i++)
        {
            if (peluru[i].aktif == false)
            {
                peluru[i].aktif = true;
                peluru[i].bulletType = 2; //laser

                float offsetX = 7.5f * player.SC;
                float offsetY = 0;
                float rotX = offsetX * c - (-offsetY) * s;
                float rotY = offsetX * s + (-offsetY) * c;

                peluru[i].x = player.x + rotX;
                peluru[i].y = player.y + rotY;

                peluru[i].vx = -c * bulletSpeed;
                peluru[i].vy = -s * bulletSpeed;

                peluru[i].weight = 4;
                peluru[i].height = 1;

               // PlaySound(TEXT("laserps.wav"), NULL, SND_ASYNC | SND_FILENAME);
                mciSendString("play laserSound from 0", NULL, 0, NULL);

                break;
            }
        }
    }
}

void resetGame()
{
    player.x = Layar_w_Actual / 2;
    player.y = Layar_H_Actual - 100;
    player.SC = Player_SC;
    player.aktif = true;

    player.weight = 7.5 * Player_SC;
    player.height = 8 * Player_SC;

    score = 0;
    nyawa = 1;

    for (int i = 0; i < Max_Peluru; i++)
        peluru[i].aktif = false;

    for (int i = 0; i < Max_musuh; i++)
        musuhMuncul(i);

    for (int i = 0; i < Max_musuh2; i++)
        musuh2Muncul(i);
}

void initGame()
{
    Layar_w_Actual = GetSystemMetrics(SM_CXSCREEN);
    Layar_H_Actual = GetSystemMetrics(SM_CYSCREEN);

    initwindow(Layar_w_Actual, Layar_H_Actual, "Game Arcade Pesawat");
    int errorcode = graphresult();

    if (errorcode != grOk)
    {
        printf("Error grafis: %s\n", grapherrormsg(errorcode));
        exit(1);
    }

    HWND hwnd = GetForegroundWindow();
    if (hwnd != NULL)
    {
        SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(hwnd, HWND_TOP, 0, 0, Layar_w_Actual, Layar_H_Actual, SWP_SHOWWINDOW);
    }
    srand(time(NULL));

    for (int i = 0; i < Max_Bintang; i++)
    {
        bintang[i].x = rand() % Layar_w_Actual;
        bintang[i].y = rand() % Layar_H_Actual;
        bintang[i].speed = (rand() % 3) + 1;
        bintang[i].aktif = true;
    }

    resetGame();
    currentState = STATE_MENU;

    mciSendString("open \"laserps.wav\" alias laserSound", NULL, 0, NULL);
    mciSendString("open \"laserfull.wav\" alias laserFullSound", NULL, 0, NULL);
    mciSendString("open \"laserps.wav\" alias explosionSound", NULL, 0, NULL);
    mciSendString("open \"laserps.wav\" alias hitSound", NULL, 0, NULL);
    mciSendString("open \"gameover.wav\" alias gameOverSound", NULL, 0, NULL);
    //mciSendString("open \"menu.wav\" alias menuSound", NULL, 0, NULL);

    //delay(1000);

    setbkcolor(BLACK);
    cleardevice();
    //mciSendString("play menuSound from 0 repeat", NULL, 0, NULL);
    //isMenuSoundPlaying = true;
}

void updateMenu()
{
    if (kbhit())
    {
        char key = getch();

        if (key == 13)
        {
            currentState = STATE_PLAYING;
            if (isMenuSoundPlaying)
            {
                mciSendString("stop menuSound", NULL, 0, NULL);
                isMenuSoundPlaying = false;
            }
        }
    }
}

void updateGameOver()
{
    if (kbhit())
    {
        char key = getch();
        if (key == 13) // ENTER
        {
            resetGame();
            currentState = STATE_MENU;
            if (isGameOverSoundPlaying)
            {
                mciSendString("stop gameOverSound", NULL, 0, NULL);
                isGameOverSoundPlaying = false;
            }
            if (!isMenuSoundPlaying)
            {
                mciSendString("play menuSound from 0 repeat", NULL, 0, NULL);
                isMenuSoundPlaying = true;
            }
        }
    }
}

void updatePlaying()
{
    gameTime += FRAME_RATE_IN_SECONDS;

    bool keyW = GetAsyncKeyState('W');
    bool keyA = GetAsyncKeyState('A');
    bool keyS = GetAsyncKeyState('S');
    bool keyD = GetAsyncKeyState('D');
    bool keyQ = GetAsyncKeyState('Q');

    bool lmb_held = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    bool rmb_held = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
    bool mmb_held = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
    bool x_held = GetAsyncKeyState('X') & 0x8000;

    if (keyQ)
    {
        if (currentState != STATE_GAME_OVER)
        {
            currentState = STATE_GAME_OVER;
            if (isLaserFullSoundPlaying)
            {
                mciSendString("stop laserFullSound", NULL, 0, NULL);
                isLaserFullSoundPlaying = false;
            }
            mciSendString("play gameOverSound from 0", NULL, 0, NULL);
            isGameOverSoundPlaying = true;
        }
    }

    isBeamFiring = false;

    if (mmb_held)
    {
        isBeamFiring = true;
        if (!isLaserFullSoundPlaying)
        {
            mciSendString("play laserFullSound from 0 repeat", NULL, 0, NULL);
            isLaserFullSoundPlaying = true;
        }
    }
    else
    {
        if (isLaserFullSoundPlaying)
        {
            mciSendString("stop laserFullSound", NULL, 0, NULL);
            isLaserFullSoundPlaying = false;
        }
    }

    if (lmb_held || x_held)
    {
        if (gameTime - lastShotTime_Mode1 > SHOT_COOLDOWN)
        {
            Peluru(1);
            lastShotTime_Mode1 = gameTime;
        }
    }

    if (rmb_held)
    {
        if (gameTime - lastShotTime_Mode2 > SHOT_COOLDOWN)
        {
            Peluru(2);
            lastShotTime_Mode2 = gameTime;
        }
    }

    float playerVelX = 0;
    float playerVelY = 0;

    if (keyW) {
        playerVelY = -PLAYER_SPEED;
        TA_player = PI / 2.0f;
    }
    if (keyS) {
        playerVelY = PLAYER_SPEED;
        TA_player = -PI / 2.0f;
    }
    if (keyA) {
        playerVelX = -PLAYER_SPEED;
        TA_player = 0.0f;
    }
    if (keyD) {
        playerVelX = PLAYER_SPEED;
        TA_player = PI;
    }

    //if (!keyW && !keyA && !keyS && !keyD)
    //{
    //    TA_player = PI;
    // }

    if (playerVelX != 0 && playerVelY != 0) {
        playerVelX *= 0.707f;
        playerVelY *= 0.707f;

        if(keyW && keyD) TA_player = 3.0f * PI / 4.0f; // Atas-Kanan (Dibalik)
        else if(keyW && keyA) TA_player = PI / 4.0f; // Atas-Kiri (Dibalik)
        else if(keyS && keyD) TA_player = -3.0f * PI / 4.0f; // Bawah-Kanan (Dibalik)
        else if(keyS && keyA) TA_player = -PI / 4.0f; // Bawah-Kiri (Dibalik)
    }

    player.x += playerVelX;
    player.y += playerVelY;

    if(player.x - player.weight/2 < 0) player.x = player.weight/2;
    if(player.x + player.weight/2 > Layar_w_Actual) player.x = Layar_w_Actual - player.weight/2;
    if(player.y - player.height/2 < 0) player.y = player.height/2;
    if(player.y + player.height/2 > Layar_H_Actual) player.y = Layar_H_Actual - player.height/2;

    for (int i = 0; i < Max_Bintang; i++) {
        bintang[i].y += bintang[i].speed;
        if (bintang[i].y > Layar_H_Actual) {
            bintang[i].y = 0;
            bintang[i].x = rand() % Layar_w_Actual;
        }
    }

    for (int i = 0; i < Max_Peluru; i++) {
        if (peluru[i].aktif) {
            peluru[i].x += peluru[i].vx;
            peluru[i].y += peluru[i].vy;
            if (peluru[i].y < 0 || peluru[i].y > Layar_H_Actual || peluru[i].x < 0 || peluru[i].x > Layar_w_Actual)
                peluru[i].aktif = false;
        }
    }

    for (int i = 0; i < Max_musuh; i++) {
        if (musuh[i].aktif) {
            musuh[i].x += musuh[i].speed * musuh[i].arah;
            if ( (musuh[i].arah == 1 && musuh[i].x > Layar_w_Actual + 50) ||
                 (musuh[i].arah == -1 && musuh[i].x < -50) )
            {
                musuhMuncul(i);
            }
        }
    }

    for (int i = 0; i < Max_musuh2; i++)
    {
        if (musuh2[i].aktif)
        {
            musuh2[i].x += musuh2[i].vx;
            musuh2[i].y += musuh2[i].vy;

            if (musuh2[i].y > Layar_H_Actual + (musuh2[i].height) ||
                musuh2[i].x < -(musuh2[i].weight) ||
                musuh2[i].x > Layar_w_Actual + (musuh2[i].weight) )
            {
                musuh2Muncul(i);
            }
        }
    }

    for (int i = 0; i < Max_Peluru; i++) {
        if (peluru[i].aktif) {
            for (int j = 0; j < Max_musuh; j++) {
                if (musuh[j].aktif) {
                    if (tabrakan(peluru[i], musuh[j])) {
                        peluru[i].aktif = false;
                        musuh[j].aktif = false;
                        musuhMuncul(j);
                        score += 100;
                        Beep(200, 50);
                    }
                }
            }
            for (int j = 0; j < Max_musuh2; j++)
            {
                if (musuh2[j].aktif)
                {
                    if (tabrakan(peluru[i], musuh2[j]))
                    {
                        peluru[i].aktif = false;
                        musuh2[j].aktif = false;
                        musuh2Muncul(j);
                        score += 250;
                        Beep(300, 50);
                    }
                }
            }
        }
    }

    if (isBeamFiring)
    {
        float c = cos(TA_player);
        float s = sin(TA_player);
        float offsetX = 7.5f * player.SC;
        float offsetY = 0;
        float rotX = offsetX * c - (-offsetY) * s;
        float rotY = offsetX * s + (-offsetY) * c;
        float beamX1 = player.x + rotX;
        float beamY1 = player.y + rotY;

        float beamVX = -c;
        float beamVY = -s;

        for (int j = 0; j < Max_musuh; j++) {
            if (musuh[j].aktif) {
                float dx = musuh[j].x - beamX1;
                float dy = musuh[j].y - beamY1;
                float dot = dx * beamVX + dy * beamVY;

                if (dot > 0)
                {
                    float dist_sq = (dx*dx + dy*dy) - (dot*dot);
                    float combined_width = musuh[j].weight/2;

                    if (dist_sq < (combined_width * combined_width))
                    {
                        musuh[j].aktif = false;
                        musuhMuncul(j);
                        score += 50;
                    }
                }
            }
        }
        for (int j = 0; j < Max_musuh2; j++) {
             if (musuh2[j].aktif) {
                float dx = musuh2[j].x - beamX1;
                float dy = musuh2[j].y - beamY1;
                float dot = dx * beamVX + dy * beamVY;
                if (dot > 0) {
                    float dist_sq = (dx*dx + dy*dy) - (dot*dot);
                    float combined_width = musuh2[j].weight/2;
                    if (dist_sq < (combined_width * combined_width)) {
                        musuh2[j].aktif = false;
                        musuh2Muncul(j);
                        score += 20;
                    }
                }
            }
        }
    }


    for (int i = 0; i < Max_musuh; i++) {
        if (musuh[i].aktif) {
            if (tabrakan(player, musuh[i])) {
                nyawa--;
                musuhMuncul(i);
                Beep(100, 100);
                if (nyawa <= 0) {
                    if (currentState != STATE_GAME_OVER)
                    {
                        currentState = STATE_GAME_OVER;
                        if (isLaserFullSoundPlaying)
                        {
                            mciSendString("stop laserFullSound", NULL, 0, NULL);
                            isLaserFullSoundPlaying = false;
                        }
                        mciSendString("play gameOverSound from 0", NULL, 0, NULL);
                        isGameOverSoundPlaying = true;
                    }
                }
        }
    }
    for (int i = 0; i < Max_musuh2; i++) {
        if (musuh2[i].aktif) {
            if (tabrakan(player, musuh2[i])) {
                nyawa--;
                musuh2Muncul(i);
                Beep(100, 100);
                if (nyawa <= 0) {
                    if (currentState != STATE_GAME_OVER) // <-- FIX: Tambahkan cek ini
                    {
                        currentState = STATE_GAME_OVER;
                        if (isLaserFullSoundPlaying)
                        {
                            mciSendString("stop laserFullSound", NULL, 0, NULL);
                            isLaserFullSoundPlaying = false;
                        }
                        mciSendString("play gameOverSound from 0", NULL, 0, NULL);
                        isGameOverSoundPlaying = true;
                    }
                }
            }
        }
    }
}
}
void renderMenu()
{
    cleardevice();
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
    outtextxy(Layar_w_Actual / 2 - 280, Layar_H_Actual / 2 - 100, (char*)"SPACE COMBAT");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(Layar_w_Actual / 2 - 350, Layar_H_Actual / 2, (char*)"Tekan ENTER untuk Mulai Game!");
    PesawatPlayer(Layar_w_Actual / 2, Layar_H_Actual / 2 + 150, 5, 0.0f);
}

void renderGameOver()
{
    cleardevice();
    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(Layar_w_Actual / 2 - 150, Layar_H_Actual / 2 - 100, (char*)"GAME OVER");

    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    sprintf(scoreText, "Skor Akhir Anda: %d", score);
    outtextxy(Layar_w_Actual / 2 - 170, Layar_H_Actual / 2, scoreText);

    outtextxy(Layar_w_Actual / 2 - 200, Layar_H_Actual / 2 + 50, (char*)"Tekan [ENTER] ke Menu Utama");
}

void renderPlaying()
{
    cleardevice();

    setcolor(WHITE);
    for (int i = 0; i < Max_Bintang; i++)
    {
        int x = bintang[i].x;
        int y = bintang[i].y;
        int scale = 5;

        if (rand() % 10 == 0)
            setcolor(DARKGRAY);
        else
            setcolor(WHITE);
        bar(x, y, x + scale - 1, y + scale - 1);
    }

    for (int i = 0; i < Max_musuh; i++)
    {
        if (musuh[i].aktif)
        {
            PesawatMusuh((int)musuh[i].x, (int)musuh[i].y, musuh[i].SC, musuh[i].posisiY, musuh[i].arah, COLOR(220, 220, 220), COLOR(180, 180, 180));
        }
    }

    for (int i = 0; i < Max_musuh2; i++)
    {
        if (musuh2[i].aktif)
        {
            PesawatMusuh2((int)musuh2[i].x, (int)musuh2[i].y, musuh2[i].SC, 0, 1, COLOR(144, 238, 144), COLOR(34, 139, 34));
        }
    }

    for (int i = 0; i < Max_Peluru; i++)
    {
        if (peluru[i].aktif)
        {
            if (peluru[i].bulletType == 1) // ini Lingkaran
            {
                setcolor(WHITE);

                setfillstyle(SOLID_FILL, RED);
                fillellipse((int)peluru[i].x, (int)peluru[i].y, peluru[i].weight, peluru[i].weight);
            }

            else if (peluru[i].bulletType == 2) //ini laser
            {
                setcolor(YELLOW);

                line((int)peluru[i].x, (int)peluru[i].y,
                     (int)(peluru[i].x + peluru[i].vx * 10), (int)(peluru[i].y + peluru[i].vy * 4));
            }
        }
    }

    setcolor(WHITE);

    if (isBeamFiring)
    {
        float c = cos(TA_player);
        float s = sin(TA_player);

        float offsetX = 7.5f * player.SC;
        float offsetY = 0;

        float rotX = offsetX * c - (-offsetY) * s;
        float rotY = offsetX * s + (-offsetY) * c;

        int x1 = player.x + rotX;
        int y1 = player.y + rotY;

        int x2 = x1 + (-c) * 2000;
        int y2 = y1 + (-s) * 2000;

        setcolor(COLOR(255, 100, 100));
        setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

        line(x1, y1, x2, y2);
        setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    }

    setcolor(COLOR(0, 150, 255));
    PesawatPlayer((int)player.x, (int)player.y, player.SC, TA_player);

    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);

    sprintf(scoreText, "Skor: %d", score);
    sprintf(nyawaText, "Nyawa: %d", nyawa);

    outtextxy(10, 10, scoreText);
    outtextxy(10, 30, nyawaText);
}

int main()
{
    int buffer = 0;
    initGame();

    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE))
        {
            break;
        }
        setactivepage(buffer);
        cleardevice();

        if (currentState == STATE_MENU)
        {
            updateMenu();
            renderMenu();
        }

        else if (currentState == STATE_PLAYING)
        {
            updatePlaying();
            renderPlaying();
        }

        else if (currentState == STATE_GAME_OVER)
        {
            updateGameOver();
            renderGameOver();
        }

        setvisualpage(buffer);
        buffer = 1 - buffer;

        delay(10);
    }
    mciSendString("close laserSound", NULL, 0, NULL);
    mciSendString("close laserFullSound", NULL, 0, NULL);
    mciSendString("close explosionSound", NULL, 0, NULL);
    mciSendString("close hitSound", NULL, 0, NULL);
    mciSendString("close gameOverSound", NULL, 0, NULL);
   // mciSendString("close menuSound", NULL, 0, NULL);

    closegraph();
    return 0;
}
