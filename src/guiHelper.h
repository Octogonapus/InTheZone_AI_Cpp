#pragma once

#include <Windows.h>
#include <string>

HWND itzHWND;

BOOL CALLBACK FindTheDesiredWnd(HWND hWnd, LPARAM lParam) {
    *(reinterpret_cast<HWND *>(lParam)) = hWnd;

    int length = ::GetWindowTextLength(hWnd);
    if (length == 0) {
        return TRUE;
    }

    TCHAR *buf = new TCHAR[length + 1];
    memset(buf, 0, (length + 1) * sizeof(TCHAR));
    GetWindowText(hWnd, buf, length + 1);
    std::string text(buf);
    if (text.compare("VEX Robotics Competition  - In The Zone RVW") == 0) {
        itzHWND = hWnd;
        delete buf;
        return FALSE;
    }
    delete buf;
    return TRUE;
}

void click(const int x, const int y) {
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
}

void placeWaypoints() {
    click(1260, 567);
    Sleep(350);
    click(1764, 567);
}

void resetRobot() {
    click(1170, 1013);
    Sleep(350);
    click(1775, 1013);
}

void startStopProgram() {
    click(1135, 1013);
}
