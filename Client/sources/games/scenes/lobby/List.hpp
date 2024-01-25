/*
** EPITECH PROJECT, 2024
** RType
** File description:
** List
*/

#ifndef LIST_HPP_
    #define LIST_HPP_
    #include "../../../../includes/MainInclusdes.hpp"

    #define SCROLLBAR_WIDTH 10

namespace rtype {

    typedef std::pair<std::pair<std::string, int>, int> pair;

    class List {
        public:
            List(std::vector<pair> friends, int posX, int posY, int visibleFriends, bool request = false, bool online = true)
                : _friends(friends), _posX(posX), _posY(posY), _visibleFriends(visibleFriends), _request(request), _online(online) {
                    _paramFriends = {
                        0, // selected item
                        0, // visible friends start
                        0, // option active
                        -1 // clicked item
                    };
                    _menuRect = rtype::ECS::Ecs3D::Rectangle2d("menuRect", Rectangle{0, 0, 0, 0});
                    _scrollbarRect = rtype::ECS::Ecs3D::Rectangle2d("scrollbarRect", Rectangle{(float)(this->_posX + 200), (float)this->_posY, (float)SCROLLBAR_WIDTH, (float)(visibleFriends * 30) - 15});
                };
            List() {};
            ~List() {};

            void setPosition(int posX, int posY) {
                this->_posX = posX;
                this->_posY = posY;
                this->_scrollbarRect.rec.x = (float)(this->_posX + 200);
                this->_scrollbarRect.rec.y = (float)this->_posY;
            }

            Vector2 getPosition() {
                return {(float)this->_posX, (float)this->_posY};
            }

            void draw();
            void update(std::unique_ptr<rtype::ClientToServer> &clientToServer, int me, int lobby_id);

            void setFriends(std::vector<pair> friends) { this->_friends = friends; };
            size_t getFriendsSize() { return this->_friends.size(); };
            std::vector<pair> getFriends() { return this->_friends; };
            int getFriend(int id);

            bool ListChange() { return _listChange; };
            void setListChange(bool listChange) { _listChange = listChange; };

        protected:
        private:
            std::vector<pair> _friends;
            std::vector<int> _paramFriends;
            int _posX;
            int _posY;
            int _visibleFriends;
            rtype::ECS::Ecs3D::Rectangle2d _menuRect;
            rtype::ECS::Ecs3D::Rectangle2d _scrollbarRect;
            bool _request;
            bool _online;

            bool _listChange;
            int _indexSelected = -1;
    };
};

#endif /* !LIST_HPP_ */
