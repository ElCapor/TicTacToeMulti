#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#ifdef _WIN32
    #include <Windows.h>
#endif

class Logger {
public:
    enum class ConsoleColors {
        WHITE, RED, GREEN, GREY, YELLOW, BLUE, PURPLE, PINK, RESET
    };
    
    static bool saveToFile;
    
    static bool SetConsoleColor(const ConsoleColors &color);
    static void ResetConsoleColor();
    static bool SaveToFile(const std::string &output, bool saveTime);
    static void EmptyLine(int linesAmount);
};

bool Logger::saveToFile = false;

bool Logger::SetConsoleColor(const ConsoleColors &color)
{
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!console) {
        std::cerr << "Failed to get handle to console.\n";
        return false;
    }
    
    WORD setColor = 7;
    switch (color) {
        case ConsoleColors::WHITE: setColor = 7; break;
        case ConsoleColors::RED: setColor = 4; break;
        case ConsoleColors::GREEN: setColor = 2; break;
        case ConsoleColors::GREY: setColor = 8; break;
        case ConsoleColors::YELLOW: setColor = 14; break;
        case ConsoleColors::BLUE: setColor = 9; break;
        case ConsoleColors::PURPLE: setColor = 5; break;
        case ConsoleColors::PINK: setColor = 13; break;
        case ConsoleColors::RESET: setColor = 15; break;
    }
    return SetConsoleTextAttribute(console, setColor) != 0;
#else
    const char* colorCode = "\033[0m";
    switch (color) {
        case ConsoleColors::WHITE: colorCode = "\033[97m"; break;
        case ConsoleColors::RED: colorCode = "\033[91m"; break;
        case ConsoleColors::GREEN: colorCode = "\033[92m"; break;
        case ConsoleColors::GREY: colorCode = "\033[90m"; break;
        case ConsoleColors::YELLOW: colorCode = "\033[93m"; break;
        case ConsoleColors::BLUE: colorCode = "\033[94m"; break;
        case ConsoleColors::PURPLE: colorCode = "\033[95m"; break;
        case ConsoleColors::PINK: colorCode = "\033[96m"; break;
        case ConsoleColors::RESET: colorCode = "\033[0m"; break;
    }
    std::cout << colorCode;
    return true;
#endif
}

void Logger::ResetConsoleColor()
{
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE) {
        SetConsoleTextAttribute(console, 7);
    }
#else
    std::cout << "\033[0m";
#endif
}

bool Logger::SaveToFile(const std::string &output, bool saveTime)
{
    std::fstream outputFile("output.txt", std::ios::out | std::ios::app);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output.txt\n";
        return false;
    }

    std::stringstream outputString;
    if (saveTime) {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string timeString(30, '\0');
        std::strftime(&timeString[0], timeString.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
        timeString = timeString.c_str();
        outputString << timeString << " | " << output;
    } else {
        outputString << output;
    }
    
    outputFile << outputString.str();
    outputFile.close();
    return true;
}

void Logger::EmptyLine(int linesAmount)
{
    std::string outputFile;
    outputFile.reserve(linesAmount);
    for (int i = 0; i < linesAmount; i++) {
        std::cout << "\n";
        outputFile.append("\n");
    }
    SaveToFile(outputFile, false);
}
