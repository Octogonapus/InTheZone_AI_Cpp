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

class heapScanner {
public:
    heapScanner(HWND hwnd) :
            m_hwnd(hwnd) {
        GetWindowThreadProcessId(m_hwnd, &m_pid);
        m_pHandle = OpenProcess(PROCESS_VM_READ, 0, m_pid);
        if (!m_pHandle) {
            std::cerr << "Could not get handle!" << std::endl;
        }
    }

    virtual ~heapScanner() {
        CloseHandle(m_pHandle);
    }

    DWORD scan(const std::array<byte, 6> &cmp, DWORD address=0x03500000) {
        while (address < 0x50000000) {
            std::array<byte, 1000000> buff;
            ReadProcessMemory(m_pHandle, (void *) address, &buff, sizeof(buff), 0);

            int i = 0;
            while (i < buff.size() - 6) {
                if (std::equal(buff.begin() + i, buff.begin() + i + 6, cmp.begin())) {
                    std::cout << "0x" << std::hex << address + i << ": " << std::string(buff.begin() + i, buff.begin() + i + 6) << std::endl;
                    delete &buff;
                    return address + i;
                }

                i++;
            }

            delete &buff;
            address += 1000000 - 6;
        }

        return 0;
    }

    std::vector<byte> readMemory(const DWORD address, const unsigned int numBytes) {
        std::vector<byte> buff;
        buff.resize(numBytes);
        std::array<byte, 6> temp;
        ReadProcessMemory(m_pHandle, (void *) address, &temp, sizeof(temp), 0);
        for (int i = 0; i < 6; ++i) {
            buff[i] = temp[i];
        }
        delete &temp;
        return buff;
    }

private:
    HWND m_hwnd;
    DWORD m_pid;
    HANDLE m_pHandle;
};
