// Copyright 2026 Jannik Laugmand Bülow

#include "engine/developer/console.h"

#include "engine/util/exceptions.h"

namespace engine {
    static constexpr size_t MAX_LOG = 512;

    void Console::registerCommand(std::string name, std::string usage, CommandCallback callback) {
        mCommands.emplace_back(std::move(name), std::move(usage), callback);
    }

    void Console::execute(Engine& engine, std::string_view input) {
        std::vector<std::string_view> tokens;
        size_t i = 0;
        while (i < input.size()) {
            while (i < input.size() && input[i] == ' ') i++;

            if (i == input.size()) break;

            size_t start = i;

            if (input[i] == '"') {
                i++;
                start = i;

                while (i < input.size() && input[i] != '"') i++;

                tokens.push_back(input.substr(start, i - start));

                if (i < input.size()) i++;
            } else {
                while (i < input.size() && input[i] != ' ') i++;

                tokens.push_back(input.substr(start, i - start));
            }
        }

        std::string_view command = tokens.empty() ? "" : tokens.front();
        std::span<const std::string_view> args = tokens.size() > 1 ? std::span(tokens).subspan(1) : std::span<const std::string_view>();

        ConsoleCommand* cmd = findCommand(command);
        if (cmd) {
            try {
                cmd->execute(engine, args);
            } catch (util::CommandException& e) {
                print(e.what());
            }
        }
    }

    void Console::print(std::string message) {
        if (mLog.size() >= MAX_LOG) {
            mLog.erase(mLog.begin());
        }

        //message.shrink_to_fit();
        mLog.push_back(std::move(message));
    }

    ConsoleCommand* Console::findCommand(std::string_view name) {
        for (auto& command : mCommands) {
            if (command.name == name) return &command;
        }
        return nullptr;
    }
}
