#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>    
#include <chrono>    

class Logger
{
    static bool saveToFile;
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
        std::cout << std::endl;
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
        
        std::cout << std::endl;
        ResetConsoleColor();
    }

    template <typename... Ty>
    static void Info(Ty... args)
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
    static void Prefixless(Ty... args)
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
    static void Error(Ty... args)
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
    static void Warn(Ty... args)
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
    static void Debug(Ty... args)
    {
        PrintWithColorAndPrefix(ConsoleColors::WHITE, ConsoleColors::PURPLE, "[Debug]", args...);
        if (saveToFile)
        {
            std::stringstream output;
            (output << ... << args);
            SaveToFile(output.str());
        }
    }
    static void EmptyLine(int linesAmount=1);
};

#endif // LOGGER_H