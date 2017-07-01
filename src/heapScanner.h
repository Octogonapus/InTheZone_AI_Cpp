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

    DWORD scan(const std::array<byte, 6> &cmp, DWORD address = 0x02500000) const {
        while (address < 0x10000000) {
            std::array<byte, 1000000> buff;
            ReadProcessMemory(m_pHandle, (void *) address, &buff, sizeof(buff), 0);

            auto found = std::search(buff.begin(), buff.end(), cmp.begin(), cmp.end());
            if (found < buff.end()) {
                std::cout << "0x" << std::hex << address + std::distance(buff.begin(), found) << ": "
                          << std::string(found, found + 6) << std::endl;
                delete &buff;
                return address + std::distance(buff.begin(), found);
            }

            address += 500000;
        }

        return 0;
    }

    std::vector<byte> readMemory(const DWORD address, const unsigned int numBytes) const {
        std::vector<byte> buff;
        buff.resize(numBytes);
        std::array<byte, 6> temp;
        ReadProcessMemory(m_pHandle, (void *) address, &temp, sizeof(temp), 0);
        for (int i = 0; i < 6; ++i) {
            buff[i] = temp[i];
        }
        return buff;
    }

    inline float numFromDistanceString(std::vector<byte> bytes) const {
        try {
            if (bytes.at(bytes.size() - 3) == 32) {
                //"2.1 m "
                //    ^
                return std::stof(std::string(bytes.begin(), bytes.end() - 3));
            } else {
                //"2.13 m"
                //     ^
                return std::stof(std::string(bytes.begin(), bytes.end() - 2));
            }
        } catch (std::invalid_argument e) {
            std::cerr << "Invalid argument numFromDistanceString: " << bytes.data() << std::endl;
        }

        return 0;
    }

    inline float numFromAngleString(std::vector<byte> bytes) const {
        try {
            return std::stof(std::string(bytes.begin(), bytes.end()));
        } catch (std::invalid_argument e) {
            std::cerr << "Invalid argument numFromAngleString: " << bytes.data() << std::endl;
        }

        return 0;
    }

private:
    HWND m_hwnd;
    DWORD m_pid;
    HANDLE m_pHandle;
};
