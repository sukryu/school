#pragma once
#include "base_window.hpp"
#include "../../utils/types.hpp"

namespace dune {
    namespace ui {

        /**
         * @brief 자원 정보를 표시하는 상단 바 클래스입니다.
         */
        class ResourceBar : public BaseWindow {
        public:
            /**
             * @brief ResourceBar 클래스의 생성자입니다.
             * @param width 바의 너비.
             */
            explicit ResourceBar(int width);

            /**
             * @brief 자원 정보를 업데이트합니다.
             * @param resource 업데이트할 자원 정보.
             */
            void update(const types::Resource& resource);

            /**
             * @brief 바를 그립니다.
             * @param renderer 렌더러 객체.
             */
            void draw(Renderer& renderer) override;

        private:
            types::Resource currentResource_;
            std::wstring formatResourceInfo() const;
        };
    } // namespace ui
} // namespace dune
