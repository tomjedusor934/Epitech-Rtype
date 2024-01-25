/*
** EPITECH PROJECT, 2024
** RType
** File description:
** login
*/

#ifndef LOGIN_HPP_
    #define LOGIN_HPP_
    #include "../../../../includes/MainInclusdes.hpp"
    #include <filesystem>

namespace rtype {

    class Login : public ECS::Ecs3D::AScene {
        public:
            Login(std::unique_ptr<rtype::ClientToServer> &clientToServer, std::unique_ptr<rtype::GameManager> &clientToGame, _Scene &scene, SoundManager &soundManager) : _clientToServer(clientToServer), _clientToGame(clientToGame), _scene(scene), _soundManager(soundManager)
            , _ftp(std::make_shared<sf::Ftp>()), _response()
            {
                _updated = 0;
            }
            ~Login();

            void draw() override;
            void update(float deltatime, float getTime) override;

            std::unique_ptr<rtype::ClientToServer> &_clientToServer;
            std::unique_ptr<rtype::GameManager> &_clientToGame;

            SoundManager &_soundManager;

            _Scene &_scene;

        protected:
        private:
            void systemConnexionButton(std::shared_ptr<rtype::ECS::Ecs3D::IEntity> login, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> usernameTextArea, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> passwordTextArea, std::unique_ptr<rtype::ClientToServer> &_clientToServer, _Scene &actualScene);
            void systemRegisterButton(std::shared_ptr<rtype::ECS::Ecs3D::IEntity> registerBtn, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> usernameTextArea, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> passwordTextArea, std::unique_ptr<rtype::ClientToServer> &_clientToServer);

            void connectToFtp();

            void checkPotentialUpdate();

            std::vector<std::string> listImages(const std::filesystem::path& directory, const std::string& outputFile);

            void iterateRemoteDirectory(const std::string& directory, sf::Ftp::Response response);

        private:
            std::shared_ptr<sf::Ftp> _ftp;
            sf::Ftp::Response _response;

        private:
            int _updated;
    };
}

#endif /* !LOGIN_HPP_ */
