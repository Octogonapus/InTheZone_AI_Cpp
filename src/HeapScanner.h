#pragma once

#include <Windows.h>
#include <tlhelp32.h>
#include <vector>
#include <algorithm>
#include <psapi.h>
#include <iostream>
#include <cmath>
#include <string>
#include <array>

class HeapScanner {
public:
    HeapScanner(HWND hwnd);

    virtual ~HeapScanner();

    /**
     * Scans process memory for an array of bytes
     *
     * @param cmp     Array of 6 bytes to search for
     * @param address Base address to scan up from
     * @return        Starting address of matching byte series
     *                Returns 0 if byte series is not found
     */
    DWORD scan(const std::array<byte, 6> &cmp, DWORD address = 0x02500000) const;

    /**
     * Reads bytes from an address in process memory
     *
     * @param address  Address to read up from
     * @param numBytes Number of bytes to read
     * @return         Vector of bytes read
     */
    std::vector<byte> readMemory(const DWORD address, const unsigned int numBytes) const;

    /**
     * Converts a vector of bytes into a float (assumes "2.1 m " or "2.13 m" format)
     *
     * @param bytes Bytes to convert
     * @return      Float representation
     */
    float numFromDistanceString(std::vector<byte> bytes) const;

    /**
     * Converts a vector of bytes into a float (assumes "-54.91" or "+99.91" format)
     *
     * @param bytes Bytes to convert
     * @return      Float representation
     */
    float numFromAngleString(std::vector<byte> bytes) const;

private:
    HWND m_hwnd;
    DWORD m_pid;
    HANDLE m_pHandle;
};
