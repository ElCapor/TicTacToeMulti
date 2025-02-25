#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>    
#include <chrono>    

class Logger
{
public:
    enum ConsoleColors
    {
        WHITE,  // Done
        RED,    // Done
        GREEN,  // Done
        BLUE,   // Done
        PURPLE, // Done
        GREY,   // Done
        BLACK,  // Doesn't seem to exist
        PINK,   // Done
        YELLOW, // Done
        RESET   // Done
    };

    static bool SetConsoleColor(const ConsoleColors &color);
    static void ResetConsoleColor();

    static bool SaveToFile(const std::string& output, bool saveTime = true);

    template <typename... Ty>
    static void PrintWithColor(const ConsoleColors &color, Ty... args)
    {
        std::stringstream output;
        (output << ... << args);

        SetConsoleColor(color);
        std::cout << output.str();
        ResetConsoleColor();
    }
    template <typename... Ty>
    static void PrintWithColorAndPrefix(const ConsoleColors &textColor, const ConsoleColors &prefixColor, const std::string &prefix, Ty... args)
    {
        std::stringstream output;

        SetConsoleColor(prefixColor);
        std::cout << prefix << " ";

        SetConsoleColor(textColor);
        (output << ... << args);

        std::cout << output.str();
        ResetConsoleColor();
    }

    template <typename... Ty>
    static void Info(bool saveToFile = false, Ty... args)
    {
        PrintWithColorAndPrefix(ConsoleColors::WHITE, ConsoleColors::GREY, "[Info]", args...);
        if (saveToFile)
        {
            std::stringstream output;
            (output << ... << args);
            SaveToFile(output.str());
        }
    }
    template <typename... Ty>
    static void Prefixless(bool saveToFile = false, Ty... args)
    {
        PrintWithColor(ConsoleColors::WHITE, ConsoleColors::GREY, args...);
        if (saveToFile)
        {
            std::stringstream output;
            (output << ... << args);
            SaveToFile(output.str());
        }
    }
    template <typename... Ty>
    static void Error(bool saveToFile = false, Ty... args)
    {
        PrintWithColorAndPrefix(ConsoleColors::WHITE, ConsoleColors::RED, "[Error]", args...);
        if (saveToFile)
        {
            std::stringstream output;
            (output << ... << args);
            SaveToFile(output.str());
        }
    }
    template <typename... Ty>
    static void Warn(bool saveToFile = false, Ty... args)
    {
        PrintWithColorAndPrefix(ConsoleColors::WHITE, ConsoleColors::YELLOW, "[Warning]", args...);
        if (saveToFile)
        {
            std::stringstream output;
            (output << ... << args);
            SaveToFile(output.str());
        }
    }
    template <typename... Ty>
    static void Debug(bool saveToFile = false, Ty... args)
    {
        PrintWithColorAndPrefix(ConsoleColors::WHITE, ConsoleColors::PURPLE, "[Debug]", args...);
        if (saveToFile)
        {
            std::stringstream output;
            (output << ... << args);
            SaveToFile(output.str());
        }
    }
    static void EmptyLine(bool saveToFile = false, int linesAmount=1);
};

#ifndef LOGGER_H
#define LOGGER_H

#include <Logger.hpp>

bool Logger::SetConsoleColor(const ConsoleColors &color)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!console)
    {
        // Replace with a custom error function
        std::cerr << "Failed to get handle to console.\n";
        return false;
    }

    WORD setColor = 0;
    switch (color)
    {
    case ConsoleColors::WHITE:
        setColor = 7;
        break;
    case ConsoleColors::RED:
        setColor = 4;
        break;
    case ConsoleColors::GREEN:
        setColor = 2;
        break;
    case ConsoleColors::GREY:
        setColor = 8;
        break;
    case ConsoleColors::YELLOW:
        setColor = 14;
        break;
    case ConsoleColors::BLUE:
        setColor = 9;
        break;
    case ConsoleColors::PURPLE:
        setColor = 5;
        break;
    case ConsoleColors::PINK:
        setColor = 13;
        break;
    case ConsoleColors::RESET:
        setColor = 15;
        break;
    default:
        setColor = 7;
    }

    return SetConsoleTextAttribute(console, setColor) != 0;
}

void Logger::ResetConsoleColor()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console != INVALID_HANDLE_VALUE)
    {
        SetConsoleTextAttribute(console, 7);
    }
}

bool Logger::SaveToFile(const std::string &output, bool saveTime)
{
    std::fstream outputFile("output.txt", std::ios::out | std::ios::app);
    if (!outputFile.is_open())
    {
        std::cerr << "Failed to open output.txt\n";
        return false;
    }

    std::stringstream outputString;
    if (saveTime)
    {
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string timeString(30, '\0');
        std::strftime(&timeString[0], timeString.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
        timeString = timeString.c_str();

        outputString << timeString << " | " << output;
        outputFile << outputString.str();
    }
    else
    {
        outputString << output;
        outputFile << outputString.str();
    }

    outputFile.close();
    return true;
}

void Logger::EmptyLine(bool saveToFile, int linesAmount)
{
    std::string outputFile;
    outputFile.reserve(linesAmount);
    for (int emptyLineCounter = 0; emptyLineCounter < linesAmount; emptyLineCounter++)
    {
        std::cout << "\n";
        outputFile.append("\n");
    }
    SaveToFile(outputFile, false);
}


#endif // LOGGER_H