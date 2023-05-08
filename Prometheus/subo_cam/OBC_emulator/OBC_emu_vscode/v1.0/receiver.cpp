/******************************************/
/* OBC emulator code for testing avionics */
/******************************************/
/* Kreinicker Gabor */
/* BME Suborbitals  */
/* 2023.04.25.      */
/****************************************/
/* https://www.lddgo.net/en/encrypt/bcc */
/****************************************/

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>

std::string get_num(char* c){
    std::string num_index1 = "0";
    std::string num_index2 = "0";

    for (int l = 0; l < sizeof(c)*1024; l++) {
        if (c[l] == '*' || c[l] == '\0') {
            num_index1 = c[l-2];
            num_index2 = c[l-1];
            break;
        }
    }
    
    std::string num_index = num_index1 + num_index2;
    return num_index;
}

std::string decimalToHex(int decimal) {
    std::stringstream ss;
    ss << std::hex << decimal;
    return ss.str();
}

int calculateChecksum(const char* data)
{
    unsigned char checksum = 0;
    for (int i = 0; i < strlen(data); i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int main() {
    HANDLE hSerial = CreateFile(
        "COM8",                         // Serial port name
        GENERIC_READ | GENERIC_WRITE,   // Access mode
        0,                              // Share mode
        NULL,                           // Security attributes
        OPEN_EXISTING,                  // Creation disposition
        0,                              // Flags and attributes
        NULL                            // Template file
    );
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cout << "Error opening serial port\n";
        return 1;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cout << "Error getting serial port state\n";
        CloseHandle(hSerial);
        return 1;
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cout << "Error setting serial port state\n";
        CloseHandle(hSerial);
        return 1;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cout << "Error setting serial port timeouts\n";
        CloseHandle(hSerial);
        return 1;
    }

    char buffer[1024] = { 0 };
    DWORD bytesRead = 0;

    while (true) {
        if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
            std::cout << "Error reading from serial port\n";
            CloseHandle(hSerial);
            return 1;
        }

        if (bytesRead > 0) {
            std::cout << "Received:\n" << buffer << std::endl;
            const char* answer = "CMDNM,PARAMETER1,PARAMETER2#";
            DWORD bytesWritten = 0;
            if (!WriteFile(hSerial, answer, strlen(answer), &bytesWritten, NULL)) {
                std::cout << "Error writing to serial port\n";
                CloseHandle(hSerial);
                return 1;
            }
            std::cout << "Transmitted:\n$" << answer << get_num(buffer) << "*" << decimalToHex(calculateChecksum(answer)) << std::endl;
        }
    }

    CloseHandle(hSerial);
    return 0;
}