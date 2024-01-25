/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Login
*/

#include "Login.hpp"

rtype::Login::~Login()
{
}

void rtype::Login::draw()
{
    BeginDrawing();
        ClearBackground(RAYWHITE);
        drawTextureAndRecLoginScene(this->getEntity("backgroundMenu"), this->getEntity("logo"), this->getEntity("close"), this->getEntity("username"), this->getEntity("password"), this->getEntity("login"), this->getEntity("register"));
        drawTextLoginScene(this->getEntity("username"), this->getEntity("password"));
    EndDrawing();
}

void rtype::Login::update(float deltatime, float getTime)
{
    systemLoginScene(this->getEntity("close"), this->getEntity("username"), this->getEntity("password"), this->getEntity("login"));
    this->systemConnexionButton(this->getEntity("login"), this->getEntity("username"), this->getEntity("password"), this->_clientToServer, this->_scene);
    this->systemRegisterButton(this->getEntity("register"), this->getEntity("username"), this->getEntity("password"), this->_clientToServer);
}

void rtype::Login::systemConnexionButton(std::shared_ptr<rtype::ECS::Ecs3D::IEntity> login, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> usernameTextArea, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> passwordTextArea, std::unique_ptr<rtype::ClientToServer> &_clientToServer, _Scene &actualScene)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), login->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button)) {
        std::cout << "username: " << usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text << std::endl;
        std::cout << "password: " << passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text << std::endl;

        if (usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text == "" || passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text == "") {
            std::cout << "Please enter a username and a password" << std::endl;
            return;
        }


        login_t login;
        std::string username = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text;
        std::string password = passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text;
        strncpy(login.username, username.c_str(), sizeof(login.username) - 1);
        login.username[sizeof(login.username) - 1] = '\0';
        strncpy(login.password, password.c_str(), sizeof(login.password) - 1);
        login.password[sizeof(login.password) - 1] = '\0';
        // Send login to server
        rtype::net::message<MainServerMsgTypes> msg;
        msg.header.id = MainServerMsgTypes::Login;
        msg << login;
        _clientToServer->Send(msg);
        std::cout << "Sending login" << std::endl;
        std::cout << "Username: " << login.username << std::endl;
        std::cout << "Password: " << login.password << std::endl;


        std::cout << "Login button pressed" << std::endl;

    }
}

void rtype::Login::systemRegisterButton(std::shared_ptr<rtype::ECS::Ecs3D::IEntity> registerBtn, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> usernameTextArea, std::shared_ptr<rtype::ECS::Ecs3D::IEntity> passwordTextArea, std::unique_ptr<rtype::ClientToServer> &_clientToServer)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), registerBtn->getComponent<rtype::ECS::Ecs3D::ButtonMenu>()->button)) {
        std::cout << "username: " << usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text << std::endl;
        std::cout << "password: " << passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text << std::endl;

        // TODO: connect client to server and redirect him to the launcher scene

        login_t login;
        std::string username = usernameTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text;
        std::string password = passwordTextArea->getComponent<rtype::ECS::Ecs3D::Text>()->text;
        strncpy(login.username, username.c_str(), sizeof(login.username) - 1);
        login.username[sizeof(login.username) - 1] = '\0';
        strncpy(login.password, password.c_str(), sizeof(login.password) - 1);
        login.password[sizeof(login.password) - 1] = '\0';
        // Send login to server
        rtype::net::message<MainServerMsgTypes> msg;
        msg.header.id = MainServerMsgTypes::Register;
        msg << login;
        _clientToServer->Send(msg);
        std::cout << "Sending login" << std::endl;
        std::cout << "Username: " << login.username << std::endl;
        std::cout << "Password: " << login.password << std::endl;


        // scene = _Scene::LAUNCHER;
    }
}

void rtype::Login::checkPotentialUpdate()
{
    std::vector<std::string> images = this->listImages("./Client/sources/media", "assets.txt");

    // voici les etapes pour verifier que tout est bien a jour:
    /*
    1. on parcoure tous les fichiers du vecteur et on regarde si l'image est sur le serveur ftp
        si oui: on compare la taille du fichier local et du fichier sur le serveur
            si la taille est differente: on telecharge le fichier du serveur
            si la taille est la meme: on passe au fichier suivant
        si non: on supprime le fichier local
    2. on parcoure tous les fichiers du serveur ftp et on regarde si l'image est sur le client
        si oui: on passe au fichier suivant
        si non: on telecharge le fichier du serveur
    */

   //1
    for (auto &image : images) {
        std::string filename = image.substr(image.find_last_of("/") + 1);
        std::cout << "filename: " << filename << std::endl;
        this->_response = this->_ftp->sendCommand("size " + filename);
        if (!this->_response.isOk()) {
            std::cout << "Error: " << this->_response.getMessage() << std::endl;
            continue;
        }
        std::cout << "Directory listing:" << std::endl << this->_response.getMessage() << std::endl;
        std::string size = this->_response.getMessage().substr(this->_response.getMessage().find_last_of(" ") + 1);
        std::cout << "size: " << size << std::endl;
        std::cout << "size: " << std::stoi(size) << std::endl;
        std::cout << "size: " << std::filesystem::file_size(image) << std::endl;
        if (std::stoi(size) != std::filesystem::file_size(image)) {
            std::cout << "File size is different" << std::endl;
            this->_response = this->_ftp->download(image, filename);
            if (!this->_response.isOk())
                std::cout << "Error: " << this->_response.getMessage() << std::endl;
            else
                std::cout << "File downloaded successfully" << std::endl;
        }
    }

    //2
    sf::Ftp::ListingResponse response = this->_ftp->getDirectoryListing("rtype");
    if (!response.isOk())
        std::cout << "Error: " << response.getMessage() << std::endl;
    else
        for (std::size_t i = 0; i < response.getListing().size(); ++i) {
            // il faut que si c'est un dossier, on regarde dans le dossier et on reliste les fichiers

        }
}

std::vector<std::string> rtype::Login::listImages(const std::filesystem::path& directory, const std::string& outputFile)
{
    std::ofstream outFile(outputFile);
    std::vector<std::string> images;

    // verify that the path exists
    if (!std::filesystem::exists(directory)) {
        std::cout << "Le dossier " << directory << " n'existe pas" << std::endl;
        return images;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            if (extension == ".png" || extension == ".obj" || extension == ".glb" || extension == ".jpg" || extension == ".gif") {
                // outFile << entry.path() << std::endl;
                std::string imagePath = entry.path().string();
                images.push_back(imagePath);
            }
        }
    }

    // std::cout << "Liste des images générée dans " << outputFile << std::endl;
    return images;
}

void rtype::Login::iterateRemoteDirectory(const std::string& directory, sf::Ftp::Response response)
{
    sf::Ftp::ListingResponse dirResponse = this->_ftp->getDirectoryListing();
    if (!dirResponse.isOk()) {
        std::cout << "Error: " << response.getMessage() << std::endl;
        return;
    }
    std::vector<std::string> res = dirResponse.getListing();
    for (std::size_t i = 0; i < res.size(); ++i) {
        std::cout << res[i] << std::endl;
        // on va essayer de changer de repertoire pour voir si c'est un dossier ou pas
        response = this->_ftp->changeDirectory(res[i]);
        if (!response.isOk()) {
            // c'est un fichier
            // on va donc regarder si le fichier est sur le client et si la taille est la meme
            // si la taille est differente ou il n est pas present, on re/telecharge le fichier
            std::string filename = directory + "/" + res[i];
            std::cout << "filename: " << filename << std::endl;
            response = this->_ftp->sendCommand("size " + res[i]);
            if (!response.isOk()) {
                std::cout << "Error: " << response.getMessage() << std::endl;
                continue;
            }
            // compare la taille avec le fichier local
            std::ifstream file(filename, std::ios::binary);

            file.seekg(0, std::ios::end);
            std::streampos fileSize = file.tellg();

            if (std::stoi(response.getMessage().substr(response.getMessage().find_last_of(" ") + 1)) != fileSize || fileSize == -1) {
                std::cout << "File size is different" << std::endl;
                response = this->_ftp->download(res[i], filename);
                if (!response.isOk())
                    std::cout << "Error: " << response.getMessage() << std::endl;
                else
                    std::cout << "File downloaded successfully" << std::endl;
            }
        } else {
            std::cout << "Directory changed successfully" << std::endl;
            // on va donc regarder si le dossier est sur le client
            // si il n est pas present, on le cree
            // on relance la fonction avec le nouveau dossier et on update le path

            std::string newDirectory = directory + "/" + res[i];
            if (!std::filesystem::exists(newDirectory))
                std::filesystem::create_directory(newDirectory);
            this->iterateRemoteDirectory(newDirectory, response);
        }
    }
    this->_ftp->changeDirectory("..");
}