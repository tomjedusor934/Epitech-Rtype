/*
** TALWEB PROJECT, 2023
** ecs_rType
** File description:
** entity
*/

#include "./common.hpp"

enum Place {
    LOBBY,               // Represents the lobby place
    LOADING,             // Represents the loading place
    STORE,               // Represents the store place
    VIDEOSETTINGS,       // Represents the video settings place
    MOUSESETTINGS,       // Represents the mouse settings place
    CONTROLSSETTINGS,    // Represents the controls settings place
    AUDIOSETTINGS,       // Represents the audio settings place
    PROFILSETTINGS       // Represents the profile settings place
};

namespace rtype {
    /**
     * @brief The Game class represents the game state and current place.
     */
    class Game {
        public:
            Place currentPlace;  // The current place in the game.

            /**
             * @brief Default constructor for the Game class.
             *        Sets the current place to LOBBY.
             */
            Game() : currentPlace(LOBBY) {}

            /**
             * @brief Sets the current place in the game.
             * @param place The place to set.
             */
            void setPlace(Place place) { currentPlace = place; }

            /**
             * @brief Gets the current place in the game.
             * @return The current place.
             */
            Place getPlace() { return currentPlace; }

        private:
    };
}

