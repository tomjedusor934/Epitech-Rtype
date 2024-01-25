/*
** EPITECH PROJECT, 2024
** RType
** File description:
** History
*/

#ifndef HISTORY_HPP_
    #define HISTORY_HPP_
    #include "../../../../includes/MainInclusdes.hpp"

    #define SCROLLBAR_WIDTH 10
    #define ITEM_WIDTH 1000
    #define ITEM_HEIGHT 100

namespace rtype {
    class History {
        public:
            History(int posX, int posY, int visibleFriends)
                : _posX(posX), _posY(posY), _visibleFriends(visibleFriends) {
                    _paramFriends = 0;
                    _menuRect = rtype::ECS::Ecs3D::Rectangle2d("menuRect", Rectangle{0, 0, 0, 0});
                    _scrollbarRect = rtype::ECS::Ecs3D::Rectangle2d("scrollbarRect", Rectangle{(float)(this->_posX + ITEM_WIDTH), (float)this->_posY, (float)SCROLLBAR_WIDTH, (float)(visibleFriends * ITEM_HEIGHT) - 15});
                    _map1 = LoadTexture("./Media/map1.png");
                };
            History() {};
            ~History() {};

            void draw(std::map<std::string, std::vector<int>> &historyValues);
            void update(std::map<std::string, std::vector<int>> &historyValues);

        protected:
        private:
            int _paramFriends;
            int _posX;
            int _posY;
            int _visibleFriends;
            rtype::ECS::Ecs3D::Rectangle2d _menuRect;
            rtype::ECS::Ecs3D::Rectangle2d _scrollbarRect;

            Texture2D _map1;

    };
};

#endif /* !HISTORY_HPP_ */