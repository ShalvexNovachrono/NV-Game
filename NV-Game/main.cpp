#include <iostream>
#include <windows.h> 
#include <random> 
#include <vector> 
#include "Keyboard_.h" 

using namespace std;

int RandomInt(int MIN, int MAX) { // Gives random number based on requirements
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
    vector<int> Smoothed = {}; // Creates an empty vector array
    
    int CheckDir = StartPoint - EndPoint; // Calculates the difference
    bool IsNeg = false;

    int CheckDirAbs = abs(CheckDir); // Makes it positive

    if (CheckDir < 0) { // Checks if "CheckDir" is less than 0
        IsNeg = true;
    } 

    for (int i = 0; i <= CheckDirAbs; i++) { // 
        if (StartPoint == EndPoint) {
            break; // Breaks the loop if it reached the goal
        } else {
            if (IsNeg) { // Adds one if its true
                StartPoint += 1;
            }
            else {
                StartPoint -= 1;
            }
            Smoothed.push_back(StartPoint); // Add it to a slot
        }

    }

    return Smoothed;
}


HDC Screen = GetDC(NULL); // Created a window?
bool MainLoop() {

    int x = 1; //This will be used for moveing the rectangle
    int y = 1;

    int LastR = RandomInt(0, 255); // Gets a random number from 0 to 255. 0 = black, 255 = white 
    int LastG = RandomInt(0, 255);
    int LastB = RandomInt(0, 255);
    HBRUSH ColourBrush = CreateSolidBrush(RGB(LastR, LastR, LastR)); // Creates Brush with random colour
    SelectObject(Screen, ColourBrush); // Puts the colour on the screen that was created.

    int R, G, B;
    while (true) {

        R = RandomInt(0, 255);
        G = RandomInt(0, 255);
        B = RandomInt(0, 255);

        vector<int> SmoothR = SmoothPointAtoB(LastR, R);  // Gets a list of numbers between point a to b
        vector<int> SmoothG = SmoothPointAtoB(LastG, G);
        vector<int> SmoothB = SmoothPointAtoB(LastB, B);

        // The code from here 
        int LoopLimit = SmoothR.size(); 
        LoopLimit = (LoopLimit < SmoothG.size()) ? SmoothG.size() : LoopLimit;
        LoopLimit = (LoopLimit < SmoothB.size()) ? SmoothB.size() : LoopLimit;
        // to here which will get the biggest array size for a loop

        for (int i = 0; i < LoopLimit; i++) {
            int LR = SmoothR[(i < SmoothR.size()) ? i : SmoothR.size() - 1]; // Checks if "i" less than Size of the "SmoothR", if its true then it  will give "i" if its not it will give the last element index from array "SmoothR"
            int LG = SmoothG[(i < SmoothG.size()) ? i : SmoothG.size() - 1];
            int LB = SmoothB[(i < SmoothB.size()) ? i : SmoothB.size() - 1];
            ColourBrush = CreateSolidBrush(RGB(LR, LG, LB)); // Creates the brush again
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
            SelectObject(Screen, ColourBrush); 
            Rectangle(Screen, x, y, 450 + x, 450 + y); // creates a rectangle and adds the position it needs to be
        }
        LastR = R; //Last colour changes to current colour
        LastG = G;
        LastB = B; 
        DeleteObject(ColourBrush); // Before creating a new brush
        Sleep(10); 
    }
    return true;
}

int main() {
    GetDesktopResolution();
    bool Running = MainLoop();
}
