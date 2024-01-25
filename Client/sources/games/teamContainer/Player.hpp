/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Player
*/

#pragma once
    #include "../../../../Includes/Includes.hpp"

namespace rtype {
    namespace teamContainer {
        class Player {
            public:
                Player(int id, profile_t *profile, bool isReady = false);
                Player();
                ~Player();

                void setId(int id) { _id = id; };
                int getId() { return _id; };

                void setIsReady(bool isReady) { _isReady = isReady; };
                bool getIsReady() { return _isReady; };

                void setProfile(profile_t *profile) { _profile = profile; };
                profile_t getProfile() { return *_profile; };

                void addFriend(profile_t friendProfile, int is_connected) { _friends.push_back(std::make_pair(friendProfile, is_connected)); };
                void removeFriend(int id) { _friends.erase(_friends.begin() + id); };
                const std::vector<std::pair<profile_t, int>> getFriends() { return _friends; };
                std::vector<std::pair<profile_t, int>> &getFriendsRef() { return _friends; };


                void addPendingFriend(profile_t friendProfile) { _pendingFriends.push_back(friendProfile); };
                void removePendingFriend(int id) { _pendingFriends.erase(_pendingFriends.begin() + id); };
                const std::vector<profile_t> getPendingFriends() { return _pendingFriends; };
                size_t getNbPendingFriends() { return _pendingFriends.size(); };
            protected:
            private:
                int _id;
                bool _isReady;
                profile_t *_profile;
                std::vector<std::pair<profile_t, int>> _friends;
                std::vector<profile_t> _pendingFriends;
            };
    }
}