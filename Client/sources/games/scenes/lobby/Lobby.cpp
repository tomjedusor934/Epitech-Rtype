/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Lobby
*/

#include "Lobby.hpp"

static std::vector<rtype::pair> sortMap(std::map<std::pair<std::string, int>, int> friends)
{
	// create an empty vector of pairs
	std::vector<rtype::pair> vec;

	// copy key-value pairs from the map to the vector
	std::copy(friends.begin(), friends.end(), std::back_inserter<std::vector<rtype::pair>>(vec));

	// sort the vector by increasing the order of its pair's second value
	// if the second value is equal, order by the pair's first value
	std::sort(vec.begin(), vec.end(),
			[](const rtype::pair &l, const rtype::pair &r)
			{
				if (l.first.second != r.first.second) {
					return l.first.second < r.first.second;
				}

				return l.first.first < r.first.first;
			});

    // std::cout << "Sorted Map" << std::endl;
    // for (auto const &pair: vec) {
	// 	std::cout << '{' << pair.first.first << "," << pair.first.second << '}' << std::endl;
	// }

    return vec;
}

static std::vector<rtype::pair> GetOnlinePlayers(std::vector<rtype::pair> friends)
{
    std::vector<rtype::pair> onlinePlayers;

    for (auto const &pair: friends) {
        if (pair.first.second == 1)
            onlinePlayers.push_back(pair);
    }
    return onlinePlayers;
}

static std::string TimerToString(float timer)
{
    int seconds = (int)timer % 60;
    int min = (int)timer / 60;
    int hour = min / 24;
    std::string secondsStr = ((seconds < 10) ? ("0" + std::to_string(seconds)) : std::to_string(seconds));

    if (hour > 0)
        return "IN QUEUE: " + std::to_string(hour) + ":" + std::to_string(min) + ":" + secondsStr;
    else if (min > 0)
        return "IN QUEUE: " + std::to_string(min) + ":" + secondsStr;
    else
        return "IN QUEUE: " + secondsStr;
}

void rtype::Lobby::UpdateMap(std::map<std::vector<int>, Rectangle> &playerMap, bool visible, std::map<int, std::string> &playerName, int index)
{
    playerMap.erase({0, !visible});
    playerMap.erase({1, !visible});
    playerMap.erase({3, !visible});
    playerMap.erase({4, !visible});
    _readyMap.clear();
    // iterate the playermap of the teamContainer
    // std::map<int, rtype::teamContainer::Player> &getPlayers()
    int i = 0;
    int j = 1;
    this->_ready = false;
    for (auto &it : this->_teamContainer.getPlayers()) {
        if (it.second.getId() == this->_me) {
            this->playerName[2] = it.second.getProfile().username;
        } else {
            _readyMap[it.second.getId()] = {rectangles[2 - j].x + 20, rectangles[2 - j].y + 10, 40, 40};
            this->_ready = true;
            this->playerName[2 - j] = it.second.getProfile().username;
            playerMap[{2 - j, !visible}] = playerMap[{2 - j, visible}];
            i++;
            j = -j;
        }
        if (i % 2 == 0 && i != 0)
            j > 0 ? j++ : j--;
    }
}

void rtype::Lobby::drawPlayer()
{
    this->_quitLobby->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x = rectangles[2].x + rectangles[2].width - 22;
    this->_quitLobby->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y = rectangles[2].y;
    auto baner = this->getEntity("Banner")->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture;
    baner.height = rectangles[0].height;
    baner.width = rectangles[0].width;

    for (int i = 0; i < buttons.size(); ++i) {
        if (playerMap.find({i, 1}) != playerMap.end()) {
            DrawTexture(baner, rectangles[i].x, rectangles[i].y, Fade(WHITE, 0.8));
            DrawText(playerName[i].c_str(), playerMap[{i, 1}].x + playerMap[{i, 1}].width / 2 - MeasureText(playerName[i].c_str(), 20) / 2, rectangles[i].y + rectangles[i].height - 200, 20, Fade(WHITE, 0.8));
            if (i != 2) {
                DrawRectangleRoundedLines(ellipsisButtons[i], 0.5, 0, 2, BLACK);
                DrawText("...", ellipsisButtons[i].x + ellipsisButtons[i].width / 2 - MeasureText("...", 20) / 2, ellipsisButtons[i].y + ellipsisButtons[i].height / 2 - 10, 20, WHITE);
            } else if (playerName.size() > 1) {
                DrawTexture(this->_quitLobby->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, rectangles[i].x + rectangles[i].width - 22, rectangles[i].y, WHITE);
            }
        } else {
            DrawRectangleRoundedLines(buttons[i], 1, 0, 2, BLACK);
            DrawText("+", buttons[i].x + buttons[i].width / 2 - MeasureText("+", 20) / 2, buttons[i].y + buttons[i].height / 2 - 10, 20, WHITE);
            DrawRectangleRoundedLines(rectangles[i], 0.2, 0, 5, BLACK);
        }
    }
    if (ellipsisButtonsVisible == true) {
        DrawRectangleRec(menuRect, DARKBLUE);
        DrawText("Exclure du groupe", menuRect.x + 10, menuRect.y + 10, 20, WHITE);
        DrawText("Supprimer de la liste d'amis", menuRect.x + 10, menuRect.y + 35, 20, WHITE);
    }
}

void rtype::Lobby::drawAddFriend()
{
    this->getEntity("Username")->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.x = (float)GetScreenWidth() / 2 - 140;
    this->getEntity("Username")->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.y = (float)GetScreenHeight() / 2;
    this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x = (float)GetScreenWidth() / 2 + 150 - 22;
    this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y = (float)GetScreenHeight() / 2 - 90;
    this->_buttonAddFriend->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.x = (float)GetScreenWidth() / 2 - 50;
    this->_buttonAddFriend->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.y = (float)GetScreenHeight() / 2 + 40;

    auto usernameTextArea = this->getEntity("Username");
    auto textUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>();
    auto buttonUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec;
    auto posUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>()->position;
    auto closePos = this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
    auto textAddFriend = this->_buttonAddFriend->getComponent<rtype::ECS::Ecs3D::Text>();

    DrawRectangleRounded({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 - 90, 300.0f, 180.0f}, 0.2f, 0, BLACK);
    DrawTextEx(textUsername->font, "ADD FRIENDS", {((float)GetScreenWidth() / 2) - (MeasureText("Add friends", textUsername->fontSize) / 2) + 15, (float)GetScreenHeight() / 2 - 60}, 20, textUsername->spacing, textUsername->tint);
    DrawTextEx(textUsername->font, textUsername->text.c_str(), {((float)GetScreenWidth() / 2) - 140, (float)GetScreenHeight() / 2}, textUsername->fontSize, textUsername->spacing, textUsername->tint);
    DrawRectangleLines(GetScreenWidth() / 2 - 145, GetScreenHeight() / 2 - 5, buttonUsername.width - 25, 30, GOLD);
    DrawRectangleLinesEx(this->_buttonAddFriend->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec, 1.0f, GOLD);
    DrawTextEx(textAddFriend->font, "Add friend", {this->_buttonAddFriend->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.x + 10, this->_buttonAddFriend->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec.y + 10}, textAddFriend->fontSize, textAddFriend->spacing, textAddFriend->tint);
    DrawTexture(this->_close->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, closePos.x, closePos.y, WHITE);
}

void rtype::Lobby::drawRequestFriend()
{
    this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x = (float)GetScreenWidth() / 2 + 150 - 22;
    this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y = (float)GetScreenHeight() / 2 - 90;

    auto usernameTextArea = this->getEntity("Username");
    auto closePos = this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
    auto textUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>();

    DrawRectangleRounded({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 - 90, 300.0f, 180.0f}, 0.2f, 0, BLACK);
    DrawTextEx(textUsername->font, "FRIENDS REQUEST", {((float)GetScreenWidth() / 2) - (MeasureText("FRIENDS REQUEST", textUsername->fontSize) / 2) + 15, (float)GetScreenHeight() / 2 - 70}, 20, textUsername->spacing, textUsername->tint);
    DrawTexture(this->_close->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, closePos.x, closePos.y, WHITE);
    this->_listRequest.draw();
}

void rtype::Lobby::drawFriendsHeader()
{
    if (_friendsHeader.size() == 0)
        return;

    auto addFriends = _friendsHeader[0];
    auto textutreAddFriends = addFriends->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture;
    auto posAddFriends = addFriends->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>()->position;

    auto requestFriends = _friendsHeader[1];
    auto textutrerequestFriends = requestFriends->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture;
    auto posrequestFriends = requestFriends->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>()->position;
    posrequestFriends.x = posAddFriends.x + (addFriends->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.width * 0.15) + 10.0f;
    posrequestFriends.y = posAddFriends.y;

    auto header = _friendsHeader[2];
    auto headerText = header->getComponent<rtype::ECS::Ecs3D::Text>();
    auto recHeader = header->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec;

    DrawRectangleRec(recHeader, BLACK);
    DrawRectangleLinesEx(recHeader, 1, GOLD);
    DrawTextEx(headerText->font, headerText->text.c_str(), {headerText->position.x, headerText->position.y}, headerText->fontSize, headerText->spacing, headerText->tint);
    DrawTextureEx(textutreAddFriends, posAddFriends, 0.0f, 0.15f, WHITE);
    DrawTextureEx(textutrerequestFriends, posrequestFriends, 0.0f, 0.15f, WHITE);
}

void rtype::Lobby::drawInviteFriend()
{
    this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.x = (float)GetScreenWidth() / 2 + 150 - 22;
    this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button.y = (float)GetScreenHeight() / 2 - 90;

    auto usernameTextArea = this->getEntity("Username");
    auto closePos = this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
    auto textUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>();

    DrawRectangleRounded({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 - 90, 300.0f, 180.0f}, 0.2f, 0, BLACK);
    DrawTextEx(textUsername->font, "INVITE FRIENDS", {((float)GetScreenWidth() / 2) - (MeasureText("INVITE FRIENDS", textUsername->fontSize) / 2) + 15, (float)GetScreenHeight() / 2 - 70}, 20, textUsername->spacing, textUsername->tint);
    DrawTexture(this->_close->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, closePos.x, closePos.y, WHITE);
    this->_listAdd.draw();
}

void rtype::Lobby::draw()
{
    BeginDrawing();
        ClearBackground(RAYWHITE);
        rtype::ECS::Ecs3D::drawMenu(this->getEntity("backgroundGame")->getComponent<rtype::ECS::Ecs3D::Texture2d>()->texture, { -110, -50 }, 2.0f);
        DrawFPS(1800, 0);

        auto font = this->getEntity("Font")->getComponent<rtype::ECS::Ecs3D::FontComponent>()->font;

        auto play = this->getEntity("Play");
        auto home = this->getEntity("Settings");
        auto career = this->getEntity("Career");
        auto collection = this->getEntity("Collection");
        auto store = this->getEntity("Store");
        auto lobby = this->getEntity("Lobby");

        auto buttonPlay = play->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
        auto buttonHome = home->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
        auto buttonCareer = career->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
        auto buttonCollection = collection->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
        auto buttonStore = store->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
        auto buttonLobby = lobby->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;

        Rectangle rect_tmp = {rectangles[2].x + 20, rectangles[2].y + 10, 40, 40};

        drawPlayer();
        this->_list.draw();
        drawFriendsHeader();
        if (this->_addFriendsVisible)
            drawAddFriend();
        else if (this->_requestFriendsVisible)
            drawRequestFriend();
        else if (this->_inviteFriendsVisible)
            drawInviteFriend();

        DrawTextEx(font, "LOBBY", {buttonLobby.x, buttonLobby.y}, lobby->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        if (this->_ready && !this->_notReadyVisible) {
            buttonPlay.x = (float)1920 / 2 - MeasureText("READY", 60.0f) / 2 - 60;
            DrawTextEx(font, "READY", {buttonPlay.x, buttonPlay.y}, play->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
            DrawRectangleRounded(rect_tmp, 1, 0, RED);
        } else if (this->_notReadyVisible) {
            buttonPlay.x = (float)1920 / 2 - MeasureText("NOT READY", 60.0f) / 2 - 50;
            DrawTextEx(font, "NOT READY", {buttonPlay.x, buttonPlay.y}, play->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
            DrawRectangleRounded(rect_tmp, 1, 0, GREEN);
        } else {
            buttonPlay.x = (float)1920 / 2 - MeasureText("FIND A MATCH", 60.0f) / 2 - 20;
            DrawTextEx(font, "FIND A MATCH", {buttonPlay.x, buttonPlay.y}, play->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        }
        if (this->_startTimer)
            DrawTextEx(font, TimerToString(this->_elapsedTimer).c_str(), {(float)1920 / 2 - MeasureText(TimerToString(this->_elapsedTimer).c_str(), 40.0f) / 2 - 20, buttonPlay.y - 50}, 30.0f, 0, WHITE);

        //ici je dessine les ronds ready et not ready des joueurs du lobby
        std::map<int, rtype::teamContainer::Player> tmp = this->_teamContainer.getPlayersRef();
        for (auto &it : tmp) {
            if (it.second.getId() != this->_me) {
                Color color = RED;
                if (it.second.getIsReady())
                    color = GREEN;
                if (_readyMap.find(it.second.getId()) != _readyMap.end())
                    DrawRectangleRounded(_readyMap[it.second.getId()], 1, 0, color);
            }
        }

        DrawTextEx(font, "SETTINGS", {buttonHome.x, buttonHome.y}, home->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "CAREER", {buttonCareer.x, buttonCareer.y}, career->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "COLLECTION", {buttonCollection.x, buttonCollection.y}, collection->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);
        DrawTextEx(font, "STORE", {buttonStore.x, buttonStore.y}, store->getComponent<rtype::ECS::Ecs3D::FontSize>()->size, 0, WHITE);

    EndDrawing();
}

void rtype::Lobby::updatePlayer()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (ellipsisButtonsVisible && CheckCollisionPointRec(GetMousePosition(), {menuRect.x, menuRect.y, menuRect.width, menuRect.height})) {
            UpdateMap(playerMap, false, playerName, index);
        }
        else if (ellipsisButtonsVisible && CheckCollisionPointRec(GetMousePosition(), {menuRect.x, menuRect.y + 25, menuRect.width, menuRect.height})) {
            UpdateMap(playerMap, false, playerName, index);
        }
        ellipsisButtonsVisible = false;
        for (int i = 0; i < buttons.size(); ++i) {
            if (CheckCollisionPointRec(GetMousePosition(), buttons[i]) && playerMap.find({i, 1}) == playerMap.end()) {
                this->_inviteFriendsVisible = true;
            }
            if (i != 2 && playerMap.find({i, 1}) != playerMap.end() && CheckCollisionPointRec(GetMousePosition(), ellipsisButtons[i])) {
                ellipsisButtonsVisible = true;
                index = i;
                menuRect = {(float)GetMouseX() - 300, (float)GetMouseY(), (float)300, (float)65};
            }
        }
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        ellipsisButtonsVisible = false;

    auto quitLobby = this->_quitLobby->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
    quitLobby.height = 20.0f;
    quitLobby.width = 20.0f;

    if (CheckCollisionPointRec(GetMousePosition(), quitLobby))
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    else
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), quitLobby)) {
        rtype::net::message<MainServerMsgTypes> msg;
        msg.header.id = MainServerMsgTypes::LeaveLobby;
        msg << this->_lobby_id << this->_me;
        this->_clientToServer->Send(msg);
    }
    sizeof(float);
}

void rtype::Lobby::updateAddFriend(std::unique_ptr<rtype::ClientToServer> &clientToServer)
{
    auto usernameTextArea = this->getEntity("Username");

    auto textArea = usernameTextArea->getComponent<rtype::ECS::Ecs3D::TextAreaComp>();
    auto textUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>();
    auto posUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::PositionComponent2d>()->position;
    auto recUsername = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec;

    posUsername.x = (float)GetScreenWidth() / 2 - 140;
    posUsername.y = (float)GetScreenHeight() / 2;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), {posUsername.x, posUsername.y, recUsername.width, recUsername.height})) {
        textArea->isFocus = true;
    }

    if (textArea->isFocus) {
        int key = GetKeyPressed();
        if (key != KEY_NULL) {
            if (key == KEY_BACKSPACE) {
                if (textUsername->text.size() > 0)
                    textUsername->text.pop_back();
            } else if (key == KEY_ENTER) {
                textArea->isFocus = false;
                //C'EST LA OU TU FAIS TA DEMANDE D'AMIS ZEUBI, TU RECUPERES LE TEXTE AVEC textUsername->text
            } else {
                if (textUsername->text.size() < textArea->nbChar)
                    textUsername->text += rtype::ECS::Ecs3D::toLower(rtype::ECS::Ecs3D::GetKeyName(key, true));
            }
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), this->_buttonAddFriend->getComponent<rtype::ECS::Ecs3D::Rectangle2d>()->rec)) {
        textArea->isFocus = false;
        std::cout << "Ajouter " << textUsername->text << " en ami" << std::endl;
        //C'EST LA OU TU FAIS TA DEMANDE D'AMIS ZEUBI, TU RECUPERES LE TEXTE AVEC textUsername->text
        rtype::net::message<MainServerMsgTypes> msg;
        msg.header.id = MainServerMsgTypes::AddFriend;
        char username[21] = {0};
        strcpy(username, textUsername->text.c_str());
        msg << username << this->_me;
        clientToServer->Send(msg);
        this->_addFriendsVisible = false;
    }

    auto close = this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;

    if (this->_addFriendsVisible) {
        if (CheckCollisionPointRec(GetMousePosition(), close))
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), close)) {
            _addFriendsVisible = false;
        }
    }
}

void rtype::Lobby::updateRequestFriend()
{
    auto close = this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;

    if (this->_requestFriendsVisible) {
        if (CheckCollisionPointRec(GetMousePosition(), close))
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), close)) {
            _requestFriendsVisible = false;
        }
    }
}

void rtype::Lobby::updateFriendsHeader()
{
    if (_friendsHeader.size() == 0)
        return;

    auto addFriends = _friendsHeader[0];
    auto textutreAddFriends = addFriends->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
    textutreAddFriends.height = textutreAddFriends.height * 0.15f;
    textutreAddFriends.width = textutreAddFriends.width * 0.15f;

    auto requestFriends = _friendsHeader[1];
    auto textutrerequestFriends = requestFriends->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;
    textutrerequestFriends.height = textutrerequestFriends.height * 0.15f;
    textutrerequestFriends.width = textutrerequestFriends.width * 0.15f;
    textutrerequestFriends.x = textutreAddFriends.x + textutreAddFriends.width + 10.0f;
    textutrerequestFriends.y = textutreAddFriends.y;

    if (!_requestFriendsVisible && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), textutreAddFriends)) {
        _addFriendsVisible = !_addFriendsVisible;
    }

    if (!_addFriendsVisible && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), textutrerequestFriends)) {
        _requestFriendsVisible = !_requestFriendsVisible;
    }
}

void rtype::Lobby::updateInviteFriend()
{
    auto close = this->_close->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button;

    if (this->_inviteFriendsVisible) {
        if (CheckCollisionPointRec(GetMousePosition(), close))
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), close)) {
            _inviteFriendsVisible = false;
        }
    }
}

void rtype::Lobby::update(float deltatime, float getTime)
{
    auto home = this->getEntity("Settings");
    auto career = this->getEntity("Career");
    auto collection = this->getEntity("Collection");
    auto store = this->getEntity("Store");
    auto lobby = this->getEntity("Lobby");


    if (this->_soundManager.menuSound.getStatus() != sf::SoundSource::Status::Playing)
        this->_soundManager.menuSound.play();

    SetExitKey(0);

    this->_lobby_id = this->_teamContainer.getLobbyId();
    // ici pour savoir si le lobby est ready
    if (this->_teamContainer.getLobbyReady()) {
        float currentTime = GetTime();
        this->_elapsedTimer = currentTime - this->_timer;
        this->_startTimer = true;
    } else {
        this->_timer = GetTime();
        this->_startTimer = false;
    }

    // ici pour savoir si je suis ready
    if (this->_teamContainer.getPlayerRef(this->_me).getIsReady())
        this->_notReadyVisible = true;
    else
        this->_notReadyVisible = false;

    if ((this->_list.getFriendsSize() != this->_teamContainer.getPlayerRef(this->_me).getFriends().size()) || this->_teamContainer.hasChangeFor("friend") || this->_list.ListChange()) {
        std::cout << "Update friends" << std::endl;
        this->_teamContainer.setChangeFor(0, "friend");
        // si c'est la liste qui a change je dois ajouter l'amis au team container
        if (this->_list.ListChange()) {
            std::cout << "List change" << std::endl;            // this->_teamContainer.getPlayerRef(this->_me).addFriend(this->_list.getFriendChange());
            this->_list.setListChange(0);
        }
        std::map<std::pair<std::string, int>, int> friends;
        for (auto &it : this->_teamContainer.getPlayerRef(this->_me).getFriends()) {
            friends[std::make_pair(it.first.username, it.second)] = it.first.user_id;
        }
        this->_list.setFriends(sortMap(friends));
    }

    if ((this->_listRequest.getFriendsSize() != this->_teamContainer.getPlayerRef(this->_me).getNbPendingFriends()) || this->_teamContainer.hasChangeFor("request") || this->_listRequest.ListChange()) {
        std::cout << "Update request" << std::endl;
        this->_teamContainer.setChangeFor(0, "request");
        this->_listRequest.setListChange(0);
        std::map<std::pair<std::string, int>, int> friends;
        for (auto &it : this->_teamContainer.getPlayerRef(this->_me).getPendingFriends())
            friends[std::make_pair(it.username, 0)] = it.user_id;
        this->_listRequest.setFriends(sortMap(friends));
        this->_listAdd.setFriends(GetOnlinePlayers(sortMap(friends)));
    }

    if (this->_addFriendsVisible) {
        this->updateAddFriend(this->_clientToServer);

        if (IsKeyPressed(KEY_ESCAPE))
            this->_addFriendsVisible = false;
    } else if (this->_requestFriendsVisible) {
        this->updateRequestFriend();
        this->_listRequest.setPosition(GetScreenWidth() / 2 - 105, GetScreenHeight() / 2 - 50);
        this->_listRequest.update(this->_clientToServer, this->_me, this->_lobby_id);

        if (IsKeyPressed(KEY_ESCAPE))
            this->_requestFriendsVisible = false;
    } else if (this->_inviteFriendsVisible) {
        this->updateInviteFriend();
        this->_listAdd.setPosition(GetScreenWidth() / 2 - 105, GetScreenHeight() / 2 - 50);
        this->_listAdd.update(this->_clientToServer, this->_me, this->_lobby_id);

        if (IsKeyPressed(KEY_ESCAPE))
            this->_inviteFriendsVisible = false;
    } else {
        if (IsKeyPressed(KEY_ESCAPE))
            this->_shouldExit = true;
        this->systemReady();
        rtype::ECS::Ecs3D::UpdateHoverEffect({lobby, home, career, collection, store});
        rtype::ECS::Ecs3D::systemButtonUpdatePlace(this->_scene, {lobby, home, career, collection, store});
        this->updateFriendsHeader();
        this->updatePlayer();
        this->_list.update(this->_clientToServer, this->_me, this->_lobby_id);
    }
    this->UpdateMap(this->playerMap, false, this->playerName);
}

void rtype::Lobby::systemReady()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePosition = GetMousePosition();

        auto button = this->getEntity("Play")->getComponent<rtype::ECS::Ecs3D::ButtonMenu>();
        if (CheckCollisionPointRec(mousePosition, button->button)) {
            if (this->_notReadyVisible) {
                rtype::net::message<MainServerMsgTypes> msg;
                msg.header.id = MainServerMsgTypes::NotReady;
                msg << this->_me << this->_lobby_id;
                _clientToServer->Send(msg);
                std::cout << "Sending not ready" << std::endl;
                this->_teamContainer.getPlayerRef(this->_me).setIsReady(0);
                this->_notReadyVisible = false;
            } else {
                rtype::net::message<MainServerMsgTypes> msg;
                msg.header.id = MainServerMsgTypes::Ready;
                msg << this->_me << this->_lobby_id;
                _clientToServer->Send(msg);
                std::cout << "Sending ready" << std::endl;
                this->_teamContainer.getPlayerRef(this->_me).setIsReady(1);
            }
        }
    }
}