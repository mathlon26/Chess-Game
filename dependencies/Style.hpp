#ifndef STYLE__HPP
#define STYLE__HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <regex>
#include <limits>
#include <cstdio>

// Extended color enumeration, including a DEFAULT for resetting styles.
enum class Color { RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, LIGHT_GREEN, PINK, DEFAULT };

class Style
{    
public:
    // Convert a Color to its corresponding ANSI escape code.
    static inline std::string Cta(Color color)
    {
        switch(color)
        {
            case Color::RED:         return "\033[31m";
            case Color::GREEN:       return "\033[32m";
            case Color::YELLOW:      return "\033[33m";
            case Color::BLUE:        return "\033[34m";
            case Color::MAGENTA:     return "\033[35m";
            case Color::CYAN:        return "\033[36m";
            case Color::LIGHT_GREEN: return "\033[92m"; // Bright green.
            case Color::PINK:        return "\033[95m"; // Bright magenta.
            case Color::DEFAULT:
            default:                 return "\033[0m";
        }
    }

    static inline std::string BoldText(const std::string &text)
    {
        return "\033[1m" + text + "\033[0m";
    }

    static inline std::string UnderlineText(const std::string &text)
    {
        return "\033[4m" + text + "\033[0m";
    }

    static inline std::string LinkText(const std::string &text, Color continueColor = Color::DEFAULT)
    {
        return "\033[4m" + text + "\033[0m" + Cta(continueColor);
    }

    // Reset the console color.
    static inline std::string ResetColor()
    {
        return "\033[0m";
    }

    // Helper function to calculate the visual length of a string,
    // ignoring ANSI escape sequences.
    static inline size_t DisplayLength(const std::string &text)
    {
        size_t length = 0;
        bool in_escape = false;
        for (size_t i = 0; i < text.size(); ++i)
        {
            if (!in_escape)
            {
                if (text[i] == '\033')
                    in_escape = true;
                else
                    ++length;
            }
            else
            {
                // Typically ANSI codes end with 'm'
                if (text[i] == 'm')
                    in_escape = false;
            }
        }
        return length;
    }

    // Print text in a given color.
    static inline void PrintColored(const std::string &text, Color color = Color::DEFAULT)
    {
        std::cout << Cta(color) << text << ResetColor();
    }

    // Print text in a given color followed by a newline.
    static inline void PrintColoredLine(const std::string &text, Color color = Color::DEFAULT, bool endl = true)
    {
        PrintColored(text, color);
        if (endl)
            std::cout << std::endl;
    }

    // Print a horizontal line using a specified Unicode character.
    static inline void PrintLine(std::string ch = "─", int length = 50, Color color = Color::DEFAULT)
    {
        std::string line{};
        for (int i = 0; i < length; i++)
            line += ch;
        
        PrintColoredLine(line, color, false);
    }

    // Print a styled title with a border made of Unicode box-drawing characters.
    static inline void PrintTitle(const std::string &title, Color color = Color::CYAN)
    {
        int padding = 12;
        int titleLength = DisplayLength(title);
        int totalWidth = titleLength + padding;
        // Title line.
        std::cout << "\033[1m" << Cta(color)
                    << '*' << std::string(totalWidth, '=') << ' ' 
                    << title << ' ' << std::string(totalWidth, '=')
                    << '*' << ResetColor() << std::endl;
    }

    // Print a box around a given (possibly multi-line) text.
    static inline void PrintBox(const std::string &text, const std::string &title = "", Color color = Color::GREEN, int fixOffset = 0)
    {
        std::istringstream iss(text);
        std::string line;
        int maxLength = 0;
        std::vector<std::string> lines;
        while (std::getline(iss, line))
        {
            lines.push_back(line);
            int dlen = DisplayLength(line);
            if (dlen > maxLength)
                maxLength = dlen;
        }
        // Top border.
        std::cout << Cta(color) << "┌─" << title;
        PrintLine("─", maxLength + 1 - static_cast<int>(DisplayLength(title)) + fixOffset, color);
        std::cout << Cta(color) << "┐" << ResetColor() << std::endl;
        // Content lines with side borders.
        for (const auto &l : lines)
        {
            std::cout << Cta(color) << "│ " << ResetColor();
            std::cout << l << std::string(maxLength - DisplayLength(l), ' ') 
                        << Cta(color) << " │" << ResetColor() << std::endl;
        }
        // Bottom border.
        std::cout << Cta(color) << "└";
        PrintLine("─", maxLength + 2, color);
        std::cout << Cta(color) << "┘" << ResetColor() << std::endl;
    }

    // Print a success message with a check mark.
    static inline void PrintSuccess(const std::string &message)
    {
        PrintColoredLine(u8"✔ | " + message, Color::GREEN);
    }

    // Print a warning message with a warning sign.
    static inline void PrintWarning(const std::string &message)
    {
        PrintColoredLine(u8"⚠ | " + message, Color::YELLOW);
    }

    // Print an error message with a cross mark.
    static inline void PrintError(const std::string &message)
    {
        PrintColoredLine(u8"✖ | " + message, Color::RED);
    }

    // Exit the program with an error message.
    static inline void PrintErrorExit(const std::string &error, int code = -1)
    {
        PrintError(error);
        exit(code);
    }

    // Print an informational message with an info symbol.
    static inline void PrintInfo(const std::string &message)
    {
        PrintColoredLine(u8"🛈 | " + message, Color::CYAN);
    }

    // Clear the console screen (works on Unix-like systems with ANSI support).
    static inline void ClearConsole()
    {
        std::cout << "\033[2J\033[H";
    }

    static inline void GetInput(const std::string &prompt, const std::string &title = "", Color color = Color::YELLOW)
    {
        PrintColored(prompt, color);
        std::cin.get();
    }

    static inline bool GetBoolInput(const std::string &prompt, const std::string &title = "", Color color = Color::YELLOW, Color titleColor = Color::CYAN)
    {
        if (!title.empty())
            PrintTitle(title, titleColor);
        
        PrintColored(prompt + " [Y/n] ", color);
        char in = std::cin.get();
        // Flush any leftover characters (including newline)
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << std::endl;
        return in == 'Y' || in == 'y';
    }

    static inline std::string GetStringInput(const std::string &prompt, const std::string &title = "", Color color = Color::YELLOW, Color titleColor = Color::CYAN)
    {
        if (!title.empty())
            PrintTitle(title, titleColor);
        
        PrintColored(prompt, color);
        std::string input;
        std::getline(std::cin, input);
        return input;
    }


    // Print a divider line with an optional centered message.
    static inline void PrintDivider(const std::string &message = "", Color color = Color::DEFAULT)
    {
        int width = 50;
        if (message.empty())
        {
            PrintLine("─", width, color);
            std::cout << std::endl;
        }
        else
        {
            int pad = (width - static_cast<int>(DisplayLength(message)) - 2) / 2;
            std::cout << Cta(color) << std::string(pad, ' ') 
                        << " " << message << " " 
                        << std::string(pad, ' ') << ResetColor() << std::endl;
        }
    }

    // Print a table with headers and rows.
    // Each column's width is determined by the widest element (header or cell) in that column.
    static inline void PrintTable(const std::string& title, const std::vector<std::string>& header, const std::vector<std::vector<std::string>>& rows, Color color = Color::DEFAULT)
    {
        size_t cols = header.size();
        std::vector<size_t> colWidths(cols, 0);

        // Determine max width for each column from the header.
        for (size_t i = 0; i < cols; i++)
        {
            colWidths[i] = DisplayLength(header[i]);
        }

        // Update max width from the rows.
        for (const auto &row : rows)
        {
            for (size_t i = 0; i < row.size() && i < cols; i++)
            {
                size_t cellLength = DisplayLength(row[i]);
                if (cellLength > colWidths[i])
                    colWidths[i] = cellLength;
            }
        }

        // Helper lambda to repeat a string.
        auto repeat = [](const std::string &s, size_t count) {
            std::string result;
            for (size_t i = 0; i < count; ++i)
                result += s;
            return result;
        };

        std::ostringstream oss;

        // Top border.
        oss << Cta(color) << "┌";
        for (size_t i = 0; i < cols; i++)
        {
            if (i == 0)
                oss << title << repeat("─", colWidths[i] + 2 - DisplayLength(title));
            else
                oss << repeat("─", colWidths[i] + 2);
            oss << (i < cols - 1 ? "┬" : "┐");
        }
        oss << ResetColor() << std::endl;
        std::cout << oss.str();

        // Header row.
        oss.str("");
        oss.clear();
        oss << Cta(color) << "│" << ResetColor();
        for (size_t i = 0; i < cols; i++)
        {
            oss << " " << Cta(Color::YELLOW) << BoldText(header[i]);
            size_t pad = colWidths[i] > DisplayLength(header[i]) ? colWidths[i] - DisplayLength(header[i]) : 0;
            oss << std::string(pad, ' ') << " " << Cta(color) << "│" << ResetColor();
        }
        oss << std::endl;
        std::cout << oss.str();

        // Header separator.
        oss.str("");
        oss.clear();
        oss << Cta(color) << "├";
        for (size_t i = 0; i < cols; i++)
        {
            oss << repeat("─", colWidths[i] + 2);
            oss << (i < cols - 1 ? "┼" : "┤");
        }
        oss << ResetColor() << std::endl;
        std::cout << oss.str();

        // Data rows.
        for (const auto &row : rows)
        {
            oss.str("");
            oss.clear();
            oss << Cta(color) << "│" << ResetColor();
            for (size_t i = 0; i < cols; i++)
            {
                // Use an empty string if this row doesn't have enough columns.
                std::string cell = (i < row.size()) ? row[i] : "";
                oss << " " << cell;
                size_t pad = colWidths[i] > DisplayLength(cell) ? colWidths[i] - DisplayLength(cell) : 0;
                oss << std::string(pad, ' ') << " " << Cta(color) << "│" << ResetColor();
            }
            oss << std::endl;
            std::cout << oss.str();
        }

        // Bottom border.
        oss.str("");
        oss.clear();
        oss << Cta(color) << "└";
        for (size_t i = 0; i < cols; i++)
        {
            oss << repeat("─", colWidths[i] + 2);
            oss << (i < cols - 1 ? "┴" : "┘");
        }
        oss << ResetColor() << std::endl;
        std::cout << oss.str();
    }
};

#endif // STYLE__HPP