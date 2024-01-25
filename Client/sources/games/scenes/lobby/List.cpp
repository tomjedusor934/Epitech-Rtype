/*
** EPITECH PROJECT, 2024
** RType
** File description:
** List
*/

#include "List.hpp"

void rtype::List::update(std::unique_ptr<rtype::ClientToServer> &clientToServer, int me, int lobby_id)
{
    int scrollIndex = 0;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (this->_request && this->_paramFriends[2] && CheckCollisionPointRec(GetMousePosition(), {this->_menuRect.rec.x, this->_menuRect.rec.y, this->_menuRect.rec.width, this->_menuRect.rec.height - 32.5f})) {
            auto it = std::next(this->_friends.begin(), this->_indexSelected);
            if (it != this->_friends.end()) {
                std::cout << "Accepter" << it->first.first << " " << it->second << std::endl;
                rtype::net::message<MainServerMsgTypes> msg;
                msg.header.id = MainServerMsgTypes::AcceptFriend;
                // il faut mettre mon id et l'id du joueur que je veux accepter la demande d'ami
                std::cout << "me: " << me << " friend: " << it->second << std::endl;
                msg << it->second << me;
                clientToServer->Send(msg);
                // il faut le supprimer de la liste des demandes d'amis
                this->_friends.erase(it);
                this->_listChange = true;
            }
            this->_paramFriends[2] = false;
        } else if (this->_request && this->_paramFriends[2] && CheckCollisionPointRec(GetMousePosition(), {this->_menuRect.rec.x, this->_menuRect.rec.y + 32.5f, this->_menuRect.rec.width, this->_menuRect.rec.height - 32.5f})) {
            auto it = std::next(this->_friends.begin(), this->_indexSelected);
            if (it != this->_friends.end()) {
                std::cout << "Refuser" << it->first.first << " " << it->second << std::endl;
                rtype::net::message<MainServerMsgTypes> msg;
                msg.header.id = MainServerMsgTypes::RefuseFriend;
                // il faut mettre mon id et l'id du joueur que je veux refuser la demande d'ami
                msg << me << it->second;
                clientToServer->Send(msg);
                // il faut le supprimer de la liste des demandes d'amis
                this->_friends.erase(it);
                this->_listChange = true;
            }

            this->_paramFriends[2] = false;
        } else if (!this->_request && this->_paramFriends[2] && CheckCollisionPointRec(GetMousePosition(), {this->_menuRect.rec.x, this->_menuRect.rec.y, this->_menuRect.rec.width, this->_menuRect.rec.height - 60})) {
            auto it = std::next(this->_friends.begin(), this->_indexSelected);
            if (it != this->_friends.end()) {
                std::cout << "Rejoindre le joueur " << it->first.first << " " << it->second << std::endl;
                rtype::net::message<MainServerMsgTypes> msg;
                msg.header.id = MainServerMsgTypes::JoinFriend;
                // il faut mettre mon id, mon lobby id, l'id du joueur que je veux rejoindre
                msg << lobby_id << me << it->second;
                clientToServer->Send(msg);
                this->_listChange = true;
            }
            this->_paramFriends[2] = false;
        } else if (!this->_request && this->_paramFriends[2] && CheckCollisionPointRec(GetMousePosition(), {this->_menuRect.rec.x, this->_menuRect.rec.y + 30, this->_menuRect.rec.width, this->_menuRect.rec.height - 60})) {
            auto it = std::next(this->_friends.begin(), this->_indexSelected);
            if (it != this->_friends.end())
                std::cout << "Inviter le joueur " << it->first.first << " " << it->second << std::endl;
            this->_paramFriends[2] = false;
        } else if (!this->_request && this->_paramFriends[2] && CheckCollisionPointRec(GetMousePosition(), {this->_menuRect.rec.x, this->_menuRect.rec.y + 60, this->_menuRect.rec.width, this->_menuRect.rec.height - 60})) {
            auto it = std::next(this->_friends.begin(), this->_indexSelected);
            if (it != this->_friends.end()) {
                std::cout << "Supprimer de la liste d'amis " << it->first.first << " " << it->second << std::endl;
                rtype::net::message<MainServerMsgTypes> msg;
                msg.header.id = MainServerMsgTypes::RemoveFriend;
                // il faut mettre mon id et l'id du joueur que je veux supprimer de ma liste d'amis
                msg << it->second << me;
                clientToServer->Send(msg);
                this->_friends.erase(it);
                this->_listChange = true;
            }
            this->_paramFriends[2] = false;
        } else {
            this->_paramFriends[2] = false;
            for (int i = 0; i < this->_visibleFriends; i++) {
                int index = this->_paramFriends[1] + i;
                if (index < this->_friends.size()) {
                    Rectangle itemRect = {(float)this->_posX, (float)this->_posY + 30 * i, (float)200, (float)30};
                    if (CheckCollisionPointRec(GetMousePosition(), itemRect)) {
                        if (this->_request) {
                            this->_menuRect.rec = {(float)GetMouseX() - 300, (float)GetMouseY(), (float)300, (float)65};
                            this->_paramFriends[2] = true;
                        }
                        this->_paramFriends[0] = index;
                        this->_paramFriends[3] = this->_paramFriends[0]; // Enregistre l'élément cliqué
                        this->_indexSelected = index;
                        break;
                    }
                }
            }
        }
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        this->_paramFriends[2] = false;
        for (int i = 0; i < this->_visibleFriends; i++) {
            int index = this->_paramFriends[1] + i;
            if (index < this->_friends.size()) {
                Rectangle itemRect = {(float)this->_posX, (float)this->_posY + 30 * i, (float)200, (float)30};
                if (CheckCollisionPointRec(GetMousePosition(), itemRect)) {
                    this->_paramFriends[2] = true;
                    if (this->_request)
                        this->_menuRect.rec = {(float)GetMouseX() - 300, (float)GetMouseY(), (float)300, (float)65};
                    else
                        this->_menuRect.rec = {(float)GetMouseX() - 300, (float)GetMouseY(), (float)300, (float)90};
                    this->_paramFriends[0] = index;
                    this->_paramFriends[3] = this->_paramFriends[0]; // Enregistre l'élément cliqué
                    this->_indexSelected = index;
                    break;
                }
            }
        }
    }
    if (this->_visibleFriends < this->_friends.size()) {
        int scroll = GetMouseWheelMove();
        if (scroll != 0) {
            for (int i = 0; i < this->_visibleFriends; i++) {
                int index = this->_paramFriends[1] + i;
                if (index < this->_friends.size()) {
                    Rectangle itemRect = {(float)this->_posX, (float)this->_posY + 30 * i, (float)200, (float)30};
                    if (CheckCollisionPointRec(GetMousePosition(), itemRect)) {
                        this->_paramFriends[2] = false;
                        this->_paramFriends[1] -= scroll;
                        if (this->_paramFriends[1] < 0)
                            this->_paramFriends[1] = 0;
                        if (this->_paramFriends[1] > this->_friends.size() - this->_visibleFriends)
                            this->_paramFriends[1] = this->_friends.size() - this->_visibleFriends;
                        break;
                    }
                }
            }
        }

        // Barre de défilement
        int maxVisibleItems = this->_friends.size() - this->_visibleFriends;
        if (maxVisibleItems > 0) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), this->_scrollbarRect.rec)) {
                this->_paramFriends[1] = (int)((GetMouseY() - this->_scrollbarRect.rec.y) / this->_scrollbarRect.rec.height * maxVisibleItems);
            }
        }
    }
}

void rtype::List::draw()
{
    ClearBackground(RAYWHITE);
    for (int i = 0; i < this->_visibleFriends; i++) {
        int index = this->_paramFriends[1] + i;
        if (index < this->_friends.size()) {
            auto it = std::next(this->_friends.begin(), index);

            Rectangle itemRect = {(float)this->_posX, (float)this->_posY + 30 * i, (float)200, (float)30};
            DrawRectangleRec(itemRect, BLACK);
            DrawText(it->first.first.c_str(), this->_posX + 10, this->_posY + 10 + 30 * i, 20, GOLD);

            if (this->_online) {
                Rectangle circle = {(float)this->_posX + 170, (float)this->_posY + 10 + 30 * i, (float)20, (float)20};
                if (it->first.second == 1)
                    DrawRectangleRounded(circle, 1, 0, GREEN);
                else
                    DrawRectangleRounded(circle, 1, 0, RED);
            }

            if (index == this->_paramFriends[3]) {
                DrawRectangleLinesEx(itemRect, 2, WHITE);
            }
        }
    }

    int maxVisibleItems = this->_friends.size() - this->_visibleFriends;
    if (maxVisibleItems > 0) {
        DrawRectangle(this->_scrollbarRect.rec.x, this->_scrollbarRect.rec.y, this->_scrollbarRect.rec.width, this->_scrollbarRect.rec.height + 15, GRAY);
        float scrollbarHandleY = (float)(this->_paramFriends[1]) / maxVisibleItems * this->_scrollbarRect.rec.height;
        DrawRectangle(this->_scrollbarRect.rec.x, this->_scrollbarRect.rec.y + scrollbarHandleY, this->_scrollbarRect.rec.width, 15, WHITE);
    }

    if (this->_paramFriends[2]) {
        DrawRectangleRec(this->_menuRect.rec, ORANGE);
        if (this->_request) {
            DrawText("Accepter", this->_menuRect.rec.x + 10, this->_menuRect.rec.y + 10, 20, WHITE);
            DrawText("Refuser", this->_menuRect.rec.x + 10, this->_menuRect.rec.y + 35, 20, WHITE);
        } else {
            DrawText("Rejoindre le joueur", this->_menuRect.rec.x + 10, this->_menuRect.rec.y + 10, 20, WHITE);
            DrawText("Inviter le joueur", this->_menuRect.rec.x + 10, this->_menuRect.rec.y + 35, 20, WHITE);
            DrawText("Supprimer de la liste d'amis", this->_menuRect.rec.x + 10, this->_menuRect.rec.y + 60, 20, WHITE);
        }
    }
}

int rtype::List::getFriend(int id)
{
    for (int i = 0; i < this->_friends.size(); i++) {
        if (this->_friends[i].first.second == id)
            return this->_friends[i].first.second;
    }
    return -1;
}