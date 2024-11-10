#pragma once
#include "base_window.hpp"
#include <vector>
#include <string>

namespace dune {
    namespace ui {

        class CommandWindow : public BaseWindow {
        public:
            CommandWindow(int x, int y, int width, int height);
            void update_commands(const std::vector<std::string>& commands);
            void draw(Renderer& renderer) override;

        private:
            std::vector<std::string> commands;
        };
    } // namespace ui
} // namespace dune