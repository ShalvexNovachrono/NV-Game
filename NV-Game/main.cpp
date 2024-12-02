#include <iostream>
#include <windows.h> 
#include <random> 
#include <vector> 
#include "Keyboard_.h" 

using namespace std;

int RandomInt(int MIN, int MAX) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(MIN, MAX);
    return dis(gen);
}

int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;

void GetDesktopResolution()
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    WINDOW_WIDTH = desktop.right;
    WINDOW_HEIGHT = desktop.bottom;
}

vector<int> SmoothPointAtoB(int StartPoint, int EndPoint) {
    vector<int> Smoothed = {};
    
    int CheckDir = StartPoint - EndPoint;
    bool IsNeg = false;

    int CheckDirAbs = abs(CheckDir);

    if (CheckDir < 0) {
        IsNeg = true;
    } 

    for (int i = 0; i <= CheckDirAbs; i++) {
        if (StartPoint == EndPoint) {
            break;
        } else {
            if (IsNeg) {
                StartPoint += 1;
            }
            else {
                StartPoint -= 1;
            }
            Smoothed.push_back(StartPoint);
        }

    }

    return Smoothed;
}


HDC Screen = GetDC(NULL);
bool MainLoop() {

    int x = 1;
    int y = 1;

    int LastR = RandomInt(0, 255);
    int LastG = RandomInt(0, 255);
    int LastB = RandomInt(0, 255);
    HBRUSH redColourBrush = CreateSolidBrush(RGB(LastR, LastR, LastR));
    SelectObject(Screen, redColourBrush);

    int R, G, B;
    while (true) {

        R = RandomInt(0, 255);
        G = RandomInt(0, 255);
        B = RandomInt(0, 255);

        vector<int> SmoothR = SmoothPointAtoB(LastR, R);
        vector<int> SmoothG = SmoothPointAtoB(LastG, G);
        vector<int> SmoothB = SmoothPointAtoB(LastB, B);

        int LoopLimit = SmoothR.size();
        LoopLimit = (LoopLimit < SmoothG.size()) ? SmoothG.size() : LoopLimit;
        LoopLimit = (LoopLimit < SmoothB.size()) ? SmoothB.size() : LoopLimit;
        for (int i = 0; i < LoopLimit; i++) {
            int LR = SmoothR[(i < SmoothR.size()) ? i : SmoothR.size() - 1];
            int LG = SmoothG[(i < SmoothG.size()) ? i : SmoothG.size() - 1];
            int LB = SmoothB[(i < SmoothB.size()) ? i : SmoothB.size() - 1];
            redColourBrush = CreateSolidBrush(RGB(LR, LG, LB));
            if (IsThisKeyPressed('A')) {
                if (x > 0) x -= 1; // Decrease x if it's greater than 0
            }
            if (IsThisKeyPressed('D')) {
                if (x < WINDOW_WIDTH) x += 1; // Increase x if it's less than 4
            }
            if (IsThisKeyPressed('W')) {
                if (y > 0) y -= 1; // Decrease y if it's greater than 0
            }
            if (IsThisKeyPressed('S')) {
                if (y < WINDOW_HEIGHT) y += 1; // Increase y if it's less than 4
            }
            SelectObject(Screen, redColourBrush);
            Rectangle(Screen, 50 + x, 50 + y, 500 + x, 500 + y);
        }
        LastR = R;
        LastG = G;
        LastB = B; 
        DeleteObject(redColourBrush); // Before creating a new brush
        Sleep(10);
    }
    return true;
}

int main() {
    GetDesktopResolution();
    bool Running = MainLoop();
}