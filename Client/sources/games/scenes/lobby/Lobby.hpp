/*
** EPITECH PROJECT, 2024
** RType
** File description:
** lobby
*/

#ifndef LOBBY_HPP_
    #define LOBBY_HPP_
    #include "List.hpp"
    #include "../../teamContainer/TeamContainer.hpp"

    #define RECTANGLE_WIDTH 300
    #define RECTANGLE_HEIGHT 700
    #define BUTTON_RADIUS 20
    #define ELLIPSIS_BUTTON_RADIUS 15

namespace rtype {
    class Lobby : public ECS::Ecs3D::AScene {
        public:
            Lobby(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, int &_me, int &_lobby_id, bool &shouldExit, rtype::TeamContainer &teamContainer, Font font, SoundManager &soundManager)
                : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _me(_me), _lobby_id(_lobby_id), _shouldExit(shouldExit), _teamContainer(teamContainer), _soundManager(soundManager) {
                    std::vector<pair> friends;
                    this->_list = rtype::List(friends, 1700, 100, 30);
                    this->_listRequest = rtype::List(friends, 1000, 100, 4, true, false);
                    this->_listAdd = rtype::List(friends, 1000, 100, 4, false, false);

                    for (int i = 0; i < 5; ++i) {
                        rectangles.push_back(Rectangle{(float)80 + i * (RECTANGLE_WIDTH + 20), 1080 / 2 - RECTANGLE_HEIGHT / 2, RECTANGLE_WIDTH, RECTANGLE_HEIGHT });
                        buttons.push_back(Rectangle{rectangles[i].x + rectangles[i].width / 2 - BUTTON_RADIUS, rectangles[i].y + rectangles[i].height / 2 - BUTTON_RADIUS, BUTTON_RADIUS * 2, BUTTON_RADIUS * 2 });
                        ellipsisButtons.push_back(Rectangle{rectangles[i].x + rectangles[i].width - ELLIPSIS_BUTTON_RADIUS * 2 - 20, rectangles[i].y + 20, ELLIPSIS_BUTTON_RADIUS * 2, ELLIPSIS_BUTTON_RADIUS * 2 });
                    }

                    playerMap[{0, 0}] = buttons[0];
                    playerMap[{1, 0}] = buttons[1];
                    playerMap[{2, 1}] = buttons[2];
                    playerMap[{3, 0}] = buttons[3];
                    playerMap[{4, 0}] = buttons[4];

                    _friendsHeader = rtype::ECS::Ecs3D::builderHeaderWithButton("friendsHeader", "./Media/NewFriend.png", "./Media/FriendRequest.png", "Social", {1700, 50, 200, 50}, font, 225, 225, 225, 225);
                    auto friendsHeaderPos = _friendsHeader[0]->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>();
                    auto friendsHeaderButton = _friendsHeader[0]->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.height;
                    _friendsHeader[0]->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y = friendsHeaderPos->position.y + (((friendsHeaderButton / 2) * 0.15f) / 2.0f);
                    _friendsHeader[0]->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>()->position = {friendsHeaderPos->position.x, friendsHeaderPos->position.y + (((friendsHeaderButton / 2) * 0.15f) / 2.0f)};
                    _close = rtype::ECS::Ecs3D::createButtonWithImage("close", "./Media/close.png", {(float)GetScreenWidth() / 2 + 150, (float)GetScreenHeight() / 2 - 50, 22.0f, 22.0f});
                    _quitLobby = rtype::ECS::Ecs3D::createButtonWithImage("close", "./Media/close.png", {rectangles[2].x + rectangles[2].width - 22, rectangles[2].y});
                    _buttonAddFriend = rtype::ECS::Ecs3D::createBoxWithText("buttonAddFriend", {(float)GetScreenWidth() / 2 + 150, (float)GetScreenHeight() / 2 - 50, 100, 40}, "Ajouter", font, 20, WHITE, 0);
                    _timer = GetTime();
                };
            ~Lobby() {};

            void draw() override;
            void update(float deltatime, float getTime) override;
            void drawPlayer();
            /*
                ca c'est pour groupe management
            */
            void updatePlayer();
            void drawAddFriend();
            void updateAddFriend(std::unique_ptr<rtype::ClientToServer> &clientToServer);
            void updateFriendsHeader();
            void drawFriendsHeader();
            void updateRequestFriend();
            void drawRequestFriend();
            void updateInviteFriend();
            void drawInviteFriend();
            void systemReady();

            void UpdateMap(std::map<std::vector<int>, Rectangle> &playerMap, bool visible, std::map<int, std::string> &playerName, int index = -1);

        protected:
        private:
            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;
            SoundManager &_soundManager;

            rtype::List _list;
            rtype::List _listRequest;
            rtype::List _listAdd;

            _Scene &_scene;

            int &_me;
            int &_lobby_id;
            bool &_shouldExit;

            std::vector<Rectangle> rectangles;
            std::vector<Rectangle> buttons;
            std::vector<Rectangle> ellipsisButtons;
            Rectangle menuRect = {0, 0, 0, 0};
            std::map<std::vector<int>, Rectangle> playerMap = {
                {{2, 1}, {0, 0, 0, 0}},
                {{3, 0}, {0, 0, 0, 0}},
                {{1, 0}, {0, 0, 0, 0}},
                {{4, 0}, {0, 0, 0, 0}},
                {{0, 0}, {0, 0, 0, 0}}
            //  {index, visible} : {x, y, width, height}
            };
            std::map<int, std::string> playerName;
            int index = -1;
            bool ellipsisButtonsVisible = false;

        private:
            rtype::TeamContainer &_teamContainer;

            std::vector<std::shared_ptr<rtype::ECS::Ecs3D::IEntity>> _friendsHeader;
            bool _addFriendsVisible = false;
            bool _requestFriendsVisible = false;
            bool _inviteFriendsVisible = false;
            bool _ready = false;
            bool _notReadyVisible = false;
            bool _startTimer = false;
            std::shared_ptr<rtype::ECS::Ecs3D::IEntity> _close;
            std::shared_ptr<rtype::ECS::Ecs3D::IEntity> _quitLobby;
            std::shared_ptr<rtype::ECS::Ecs3D::IEntity> _buttonAddFriend;
            float _timer;
            float _elapsedTimer;
            std::map<int, Rectangle> _readyMap;
    };
}

#endif /* !LOBBY_HPP_ */
