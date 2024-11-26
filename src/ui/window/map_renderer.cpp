#include "map_renderer.hpp"

namespace dune {
    namespace ui {

        MapRenderer::MapRenderer(int x, int y, int width, int height)
            : BaseWindow(x, y, width, height) {}

        void MapRenderer::draw(Renderer& renderer) {
            drawBorder(renderer);
        }

        void MapRenderer::draw(Renderer& renderer, const core::Map& map) {
            drawBorder(renderer);
            drawTerrain(renderer, map);
            drawBuildings(renderer, map);
            drawGroundUnits(renderer, map);
        }

        void MapRenderer::drawTerrain(Renderer& renderer, const core::Map& map) {
            for (int row = 0; row < height_ - 2; ++row) {
                for (int col = 0; col < width_ - 2; ++col) {
                    types::Position pos{ row, col };
                    const auto& terrain = map.getTerrainManager().getTerrain(pos);
                    wchar_t ch = terrain.getRepresentation();
                    int color = terrain.getColor();
                    renderer.drawChar(x_ + col + 1, y_ + row + 1, ch, color);
                }
            }
        }

        void MapRenderer::drawBuildings(Renderer& renderer, const core::Map& map) {
            for (const auto& building : map.getBuildingManager().getBuildings()) {
                types::Position pos = building->getPosition();
                for (int i = 0; i < building->getHeight(); ++i) {
                    for (int j = 0; j < building->getWidth(); ++j) {
                        int drawX = x_ + pos.column + j + 1;
                        int drawY = y_ + pos.row + i + 1;
                        if (drawX >= x_ + 1 && drawX < x_ + width_ - 1 &&
                            drawY >= y_ + 1 && drawY < y_ + height_ - 1) {
                            renderer.drawChar(drawX, drawY,
                                building->getRepresentation(),
                                building->getColor());
                        }
                    }
                }
            }
        }

        void MapRenderer::drawGroundUnits(Renderer& renderer, const core::Map& map) {
            for (const auto& unit : map.getUnitManager().getUnits()) {
                if (unit->getType() != types::UnitType::DesertEagle) {
                    types::Position pos = unit->getPosition();
                    int drawX = x_ + pos.column + 1;
                    int drawY = y_ + pos.row + 1;
                    if (drawX >= x_ + 1 && drawX < x_ + width_ - 1 &&
                        drawY >= y_ + 1 && drawY < y_ + height_ - 1) {
                        renderer.drawChar(drawX, drawY,
                            unit->getRepresentation(),
                            unit->getColor());
                    }
                }
            }
        }
    } // namespace ui
} // namespace dune
