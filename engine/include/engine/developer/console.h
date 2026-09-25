// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_DEVELOPER_CONSOLE_H
#define ASTERACITY_ENGINE_DEVELOPER_CONSOLE_H

#include <string>
#include <span>
#include <vector>

namespace engine {
    class Engine;

    using CommandCallback = void (*)(Engine& engine, std::span<const std::string_view> args);

    struct ConsoleCommand {
        std::string name;
        std::string usage;
        CommandCallback execute;
    };

    class Console {
        friend class ConsoleSystem;
    public:
        void registerCommand(std::string name, std::string usage, CommandCallback callback);

        void execute(Engine& engine, std::string_view input);

        void print(std::string message);

        bool isVisible() const { return mVisible; }
        void setVisible(bool visible) { mVisible = visible; }

        int getScrollOffset() const { return mScrollOffset; }
        void setScrollOffset(int offset) { mScrollOffset = offset; }

    private:
        bool mVisible = false;

        int mScrollOffset = 0;

        std::vector<ConsoleCommand> mCommands;
        std::vector<std::string> mLog;

        ConsoleCommand* findCommand(std::string_view name);
    };
}

#endif //ASTERACITY_ENGINE_DEVELOPER_CONSOLE_H
