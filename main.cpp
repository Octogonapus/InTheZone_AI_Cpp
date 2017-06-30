#include <iostream>
#include <d3d9.h>
#include <vector>

#include "ConePipeline.h"

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

void screencap(HWND hWnd) {
    RECT DesktopParams;
    HDC DevC = GetDC(hWnd);
    GetWindowRect(hWnd, &DesktopParams);
    DWORD Width = DesktopParams.right - DesktopParams.left;
    DWORD Height = DesktopParams.bottom - DesktopParams.top;

    DWORD FileSize =
            sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (sizeof(RGBTRIPLE) + 1 * (Width * Height * 4));
    char *BmpFileData = (char *) GlobalAlloc(0x0040, FileSize);

    PBITMAPFILEHEADER BFileHeader = (PBITMAPFILEHEADER) BmpFileData;
    PBITMAPINFOHEADER BInfoHeader = (PBITMAPINFOHEADER) &BmpFileData[sizeof(BITMAPFILEHEADER)];

    BFileHeader->bfType = 0x4D42; // BM
    BFileHeader->bfSize = sizeof(BITMAPFILEHEADER);
    BFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
    BInfoHeader->biPlanes = 1;
    BInfoHeader->biBitCount = 24;
    BInfoHeader->biCompression = BI_RGB;
    BInfoHeader->biHeight = Height;
    BInfoHeader->biWidth = Width;

    RGBTRIPLE *Image = (RGBTRIPLE *) &BmpFileData[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
    RGBTRIPLE color;

    HDC CaptureDC = CreateCompatibleDC(DevC);
    HBITMAP CaptureBitmap = CreateCompatibleBitmap(DevC, Width, Height);
    SelectObject(CaptureDC, CaptureBitmap);
    BitBlt(CaptureDC, 0, 0, Width, Height, DevC, 0, 0, SRCCOPY | CAPTUREBLT);
    GetDIBits(CaptureDC, CaptureBitmap, 0, Height, Image, (LPBITMAPINFO) BInfoHeader, DIB_RGB_COLORS);

    DWORD Junk;
    HANDLE FH = CreateFileA("hello.bmp", GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);
    WriteFile(FH, BmpFileData, FileSize, &Junk, 0);
    CloseHandle(FH);
    GlobalFree(BmpFileData);
}

int main() {
    grip::ConePipeline conePipeline;
    HWND hFoundWnd = NULL;
    ::EnumWindows(&FindTheDesiredWnd, reinterpret_cast<LPARAM>(&hFoundWnd));
    if (hFoundWnd != NULL) {
        screencap(itzHWND);
    }
    return 0;
}