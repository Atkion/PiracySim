#include <iostream>
#include <cstdio>
//the following line is necessary for the GetConsoleWindow() function to work!
//it basically says that you are running this program on Windows 2000 or higher
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
//it is important that the above line be typed BEFORE <windows.h> is included
#include <windows.h>
#include "WindowSetup.h"
using namespace std;

void setupGameWindow() {
  int width = 1200, height = 800;
  HWND console = GetConsoleWindow();
  RECT r, d;
  GetWindowRect(console, &r); //stores the console's current dimensions
  GetWindowRect(GetDesktopWindow(), &d); //stores the desktop dimensions
  int drawX = d.right - width - (d.right - width)/2, drawY = d.bottom - height - (d.bottom - height)/2; // center window
  MoveWindow(console, drawX, drawY, width, height, TRUE); //MoveWindow(window_handle, x, y, width, height, redraw_window);
}