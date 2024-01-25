/*
** EPITECH PROJECT, 2023
** RType
** File description:
** Game
*/

#include "Game.hpp"

Game::Game()
{
    //! dis moi comment tu geres tes scenes pour l'instant c'est comme ca
    this->_scene = _Scene::LOGIN;

    // creer le client to game

    this->_clientToGame = std::make_unique<rtype::GameManager>(this->_gameLogic, this->_me, this->_scene, this->_soundManager);
    // this->_clientToGame->Connect("127.0.0.1", DEFAULT_GAME_SERVER_PORT);

    // creer le client to server lobby
    this->_clientToServer = std::make_unique<rtype::ClientToServer>(this->_scene, this->_me, this->_lobby_id, this->_clientToGame, this->_containerSettings, this->_teamContainer, this->_historyValues);
    //! for remote connection

    // this->_clientToServer->Connect("10.18.205.233", MAIN_SERVER_PORT);
    this->_clientToServer->Connect("127.0.0.1", MAIN_SERVER_PORT);

    //? Créer une fenêtre SFML (initialise tes truc quoi)
    //! et par pitie fait ca propre
}

Game::~Game()
{
    if (this->_thread.joinable())
        this->_thread.join();
}


void Game::run()
{
    bool shouldExit = false;

    const int screenWidth = 1200;
    const int screenHeight = 700;

    auto startTime = std::chrono::high_resolution_clock::now();
    double deltaTime = 0.0;

    // SetConfigFlags(FLAG_WINDOW_UNDECORATED);

    InitWindow(screenWidth, screenHeight, "Raccoon");
    SetTargetFPS(240);

    // ------------------ INIT SCENES ------------------ //
    float zoomFactor = 1.5f;
    Vector2 imagePosition = { 600, 200 };

    //Variables to initi scenes//
    float fontSize = 30.0f;
    float fontSizePlay = 60.0f;
    int frameCountBackground = 160;
    int sizeFont = 96;
    Rectangle zoneVideoSettings = Rectangle{1920 / 2 - 500.0f, 40, 120, 50};
    Rectangle zoneControlsSettings = Rectangle{1920 / 2 - 300.0f, 40, 220, 50};
    Rectangle zoneMouseSettings = Rectangle{1920 / 2 - 40.0f, 40, (float)MeasureText("mousesettings", fontSize), 50};
    Rectangle zoneAudioSettings = Rectangle{1920 / 2 + 300.0f, 40, 150, 50};
    Rectangle zoneProfilSettings = Rectangle{1920 / 2 + 500.0f, 40, 120, 50};
    Rectangle zoneAppliedButton = Rectangle{1550, 950, 300, 100};
    Rectangle zoneYes = Rectangle{1920 / 2 - 150.0f, 1080 / 2, 120, 50};
    Rectangle zoneNo = Rectangle{1920 / 2 + 125.0f, 1080 / 2, 120, 50};
    Rectangle zoneSave = Rectangle{1920 / 2 - 200.0f, 1080 / 2 - 100, 120, 50};
    Rectangle zoneLobby = Rectangle{1920 / 2 - 170.0f, 30, 130, 50};
    Rectangle zonePlay = Rectangle{(float)1920 / 2 - MeasureText("Find a match", fontSizePlay) / 2 - 20, 950, (float)MeasureText("Find a match", fontSizePlay), 50};
    Rectangle zoneSettings = Rectangle{1920 / 2 - 750.0f, 40, 120, 50};
    Rectangle zoneCareer = Rectangle{1920 / 2 + 120.0f, 40, 150, 50};
    Rectangle zoneCollection = Rectangle{1920 / 2 - 500.0f, 40, 220, 50};
    Rectangle zoneStore = Rectangle{1920 / 2 + 400.0f, 40, 120, 50};
    std::string filepathFont = "./Media/font/Prototype.ttf";
    std::string filepathBackground = "./Media/valobackground.gif";
    std::string filepathAppliedHover = "./Media/buttons/applyHover.png";
    std::string filepathApplied = "./Media/buttons/apply.png";


    // rtype::ECS::Ecs3D::Scene launcher;
    std::map<std::string, std::string> params;
    Font myFont = LoadFontEx("./Media/font/Prototype.ttf", 96, 0, 250);

    rtype::Login loginScene(this->_clientToServer, this->_clientToGame, this->_scene, _soundManager);
    rtype::GameScene gameScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_gameLogic, this->_me, _soundManager,this->_lobby_id, shouldExit, this->_containerSettings);
    rtype::Lobby lobbyScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, this->_lobby_id, shouldExit, this->_teamContainer, myFont, _soundManager);
    rtype::Loading loadingScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, this->_lobby_id, myFont, _soundManager);
    params = {
        {"Frame Per Second (FPS)", "Increment"},
        {"Vsync", "Coche"},
        {"Fov", "JaugeBar"},
        {"Brightness", "JaugeBar"},
        {"Gamma", "JaugeBar"},
        {"Contrast", "JaugeBar"},
        {"Saturation", "JaugeBar"},
        {"Sharpness", "JaugeBar"},
        {"Resolution/1280x720/1920x1080/Fullscreen", "Select"},
        {"Colorblind/None/Protanopia/Deuteranopia", "Select"}
    };
    rtype::VideoSettings videoSettingsScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, rtype::ECS::Ecs3D::BuilderParam(params, {100, 150, 1600, 730}, myFont, WHITE, 40.0f), this->_containerSettings);
    rtype::VideoSettings videoSettingsGameScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, rtype::ECS::Ecs3D::BuilderParam(params, {100, 150, 1600, 730}, myFont, WHITE, 40.0f), this->_containerSettings, _Scene::GAME);
    params = {
        {"Sensitivity", "JaugeBar"},
        {"Invert y", "Coche"},
        {"Invert x", "Coche"},
        {"Mouse acceleration", "Coche"},
        {"Mouse acceleration x", "JaugeBar"},
        {"Mouse acceleration y", "JaugeBar"},
        {"Mouse smoothing", "Coche"},
        {"Mouse smoothing x", "JaugeBar"},
        {"Mouse smoothing y", "JaugeBar"},
        {"Mouse filtering", "Coche"},
        {"Mouse filtering x", "JaugeBar"},
        {"Mouse filtering y", "JaugeBar"}
    };
    rtype::MouseSettings mouseSettingsScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, rtype::ECS::Ecs3D::BuilderParam(params, {100, 150, 1600, 730}, myFont, WHITE, 40.0f), this->_containerSettings);
    rtype::MouseSettings mouseSettingsGameScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, rtype::ECS::Ecs3D::BuilderParam(params, {100, 150, 1600, 730}, myFont, WHITE, 40.0f), this->_containerSettings, _Scene::GAME);
    rtype::ProfilSettings profilSettingsScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, this->_containerSettings, myFont);
    rtype::ProfilSettings profilSettingsGameScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, this->_containerSettings, myFont,  _Scene::GAME);
    rtype::ControlsSettings controlsSettingsScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, myFont, this->_containerSettings);
    rtype::ControlsSettings controlsSettingsGameScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, myFont, this->_containerSettings, _Scene::GAME);
    params = {
        {"MasterVolume", "JaugeBar"},
        {"Music", "JaugeBar"},
        {"Effects", "JaugeBar"},
        {"Voice", "JaugeBar"},
        {"Voice chat", "JaugeBar"},
        {"Voice chat push to talk", "Coche"}
    };
    rtype::AudioSettings audioSettingsScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, rtype::ECS::Ecs3D::BuilderParam(params, {100, 150, 1600, 730}, myFont, WHITE, 40.0f), this->_containerSettings, _Scene::LOBBY, _soundManager);
    rtype::AudioSettings audioSettingsGameScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, rtype::ECS::Ecs3D::BuilderParam(params, {100, 150, 1600, 730}, myFont, WHITE, 40.0f), this->_containerSettings, _Scene::GAME, _soundManager);
    rtype::Store storeScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, this->_lobby_id, shouldExit, _soundManager);
    rtype::Collection collectionScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, this->_lobby_id, shouldExit, _soundManager);
    rtype::Career careerScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, this->_lobby_id, shouldExit, _soundManager, this->_historyValues);
    rtype::ResultScene resultScene(this->_clientToServer, this->_clientToGame, this->_scene, this->_me, this->_lobby_id, myFont, _soundManager, this->_gameLogic, this->_me);


    Font Spiegel = LoadFontEx("./Media/Fonts_Package/Fonts_Package/Spiegel-TTF/Spiegel_TT_Bold.ttf", 96, 0, 250);


    // ------------------ BUILDER SCENES ------------------ //
    // build login scene
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> backgroundGameentity = rtype::ECS::Ecs3D::createImg("backgroundGame", "./Media/backgroundValoLobby.png");
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> fontentity = rtype::ECS::Ecs3D::createFont("Font", "FontDefault", sizeFont, filepathFont);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> videoSettingsentity = rtype::ECS::Ecs3D::createButtonWithFont("VideoSettings", zoneVideoSettings, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> controlsSettingsentity = rtype::ECS::Ecs3D::createButtonWithFont("ControlsSettings", zoneControlsSettings, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> mouseSettingsentity = rtype::ECS::Ecs3D::createButtonWithFont("MouseSettings", zoneMouseSettings, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> audioSettingsentity = rtype::ECS::Ecs3D::createButtonWithFont("AudioSettings", zoneAudioSettings, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> profilSettingsentity = rtype::ECS::Ecs3D::createButtonWithFont("ProfilSettings", zoneProfilSettings, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> yesentity = rtype::ECS::Ecs3D::createButtonWithFont("Yes", zoneYes, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> noentity = rtype::ECS::Ecs3D::createButtonWithFont("No", zoneNo, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> saveentity = rtype::ECS::Ecs3D::createButtonWithFont("Do you want to save new settings?", zoneSave, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> backgroundMenuentity = rtype::ECS::Ecs3D::createImgAnim("backgroundMenu", filepathBackground, frameCountBackground);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> appliedButton1 = rtype::ECS::Ecs3D::createButtonWithHoverFirstPart("AppliedButton", filepathApplied, zoneAppliedButton);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> appliedButton2 = rtype::ECS::Ecs3D::createButtonWithHoverSecondPart("AppliedButton", filepathAppliedHover, zoneAppliedButton);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> storeentity = rtype::ECS::Ecs3D::createButtonWithFont("Store", zoneStore, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> collectionentity = rtype::ECS::Ecs3D::createButtonWithFont("Collection", zoneCollection, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> careerentity = rtype::ECS::Ecs3D::createButtonWithFont("Career", zoneCareer, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> homeentity = rtype::ECS::Ecs3D::createButtonWithFont("Settings", zoneSettings, fontSize);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> lobbyentity = rtype::ECS::Ecs3D::createButtonWithFont("Lobby", zoneLobby, fontSizePlay);
    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> usernameentity = rtype::ECS::Ecs3D::createTextArea("Username", "", {1600, 50}, {50, 300, 300, 50}, Spiegel, 20, WHITE, 1, false, 20);

    builderImg("backgroundMenu", "./Media/back_launcher.png", loginScene);
    builderImg("logo", "./Media/logo.png", loginScene);
    builderImg("logoWindow", "./Media/WindowLogo.png", loginScene);
    builderButtonWithImage("close", "./Media/close.png", {1165, 10, 22, 22}, loginScene);
    builderButtonWithImage("login", "./Media/buttons/connexion.png", {150, 600, 80, 80}, loginScene);
    builderButtonWithImage("register", "./Media/buttons/connexion.png", {150, 500, 80, 80}, loginScene);
    buildTextArea("username", "Username", {50, 300}, {50, 300, 300, 50}, loginScene, Spiegel, 30, {200, 200, 200, 255}, 1, false, 10);
    buildTextArea("password", "Password", {50, 400}, {50, 400, 300, 50}, loginScene, Spiegel, 30, {200, 200, 200, 255}, 1, false, 10);

    // build lobby scene
    lobbyScene.addEntity({backgroundGameentity, storeentity, collectionentity, careerentity, homeentity, lobbyentity, fontentity, usernameentity});
    builderButtonWithFont("Play", zonePlay, fontSizePlay, lobbyScene);
    builderImg("Banner", "./Media/banner_raccoon.png", lobbyScene);

    // build settings scene

    videoSettingsScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2});
    videoSettingsGameScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2});
    mouseSettingsScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2});
    mouseSettingsGameScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2});
    audioSettingsScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2});
    audioSettingsGameScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2});
    profilSettingsScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2, usernameentity});
    profilSettingsGameScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2, usernameentity});
    controlsSettingsScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2});
    controlsSettingsGameScene.addEntity({videoSettingsentity, controlsSettingsentity, mouseSettingsentity, audioSettingsentity, profilSettingsentity, yesentity, noentity, saveentity, backgroundMenuentity, fontentity, appliedButton1, appliedButton2});
    storeScene.addEntity({storeentity, collectionentity, careerentity, homeentity, lobbyentity, backgroundGameentity, fontentity});
    collectionScene.addEntity({storeentity, collectionentity, careerentity, homeentity, lobbyentity, backgroundGameentity, fontentity});
    careerScene.addEntity({storeentity, collectionentity, careerentity, homeentity, lobbyentity, backgroundGameentity, fontentity});


    // build game scene
    buildMyPlayer("Player", Vector3{ 0.0f, 0.0f, 0.0f }, gameScene);
    builderButtonWithFont("SettingsGame", Rectangle{(float)1920 / 2 - MeasureText("Settings", fontSize) / 2 + 20.0f, 1080 / 2 - 50, 120, 50}, fontSize, gameScene);
    builderButtonWithFont("Quit", Rectangle{(float)1920 / 2 - MeasureText("Quit", fontSize) / 2, 1080 / 2, 120, 50}, fontSize, gameScene);
    gameScene.addEntity(fontentity);


    std::shared_ptr<rtype::ECS::Ecs3D::IEntity> logo = loginScene.getEntity("logoWindow");
    SetWindowIcon(logo->getComponent<rtype::ECS::Ecs3D::ImageComp>()->image);


    this->allScenes[_Scene::LOGIN] = std::make_unique<rtype::Login>(loginScene);
    this->allScenes[_Scene::GAME] = std::make_unique<rtype::GameScene>(gameScene);
    this->allScenes[_Scene::LOBBY] = std::make_unique<rtype::Lobby>(lobbyScene);
    this->allScenes[_Scene::STORE] = std::make_unique<rtype::Store>(storeScene);
    this->allScenes[_Scene::CAREER] = std::make_unique<rtype::Career>(careerScene);
    this->allScenes[_Scene::COLLECTION] = std::make_unique<rtype::Collection>(collectionScene);

    this->allScenes[_Scene::VIDEOSETTINGS] = std::make_unique<rtype::VideoSettings>(videoSettingsScene);
    this->allScenes[_Scene::MOUSESETTINGS] = std::make_unique<rtype::MouseSettings>(mouseSettingsScene);
    this->allScenes[_Scene::PROFILSETTINGS] = std::make_unique<rtype::ProfilSettings>(profilSettingsScene);
    this->allScenes[_Scene::CONTROLSSETTINGS] = std::make_unique<rtype::ControlsSettings>(controlsSettingsScene);
    this->allScenes[_Scene::AUDIOSETTINGS] = std::make_unique<rtype::AudioSettings>(audioSettingsScene);

    this->allScenes[_Scene::VIDEOSETTINGSGAME] = std::make_unique<rtype::VideoSettings>(videoSettingsGameScene);
    this->allScenes[_Scene::MOUSESETTINGSGAME] = std::make_unique<rtype::MouseSettings>(mouseSettingsGameScene);
    this->allScenes[_Scene::PROFILSETTINGSGAME] = std::make_unique<rtype::ProfilSettings>(profilSettingsGameScene);
    this->allScenes[_Scene::CONTROLSSETTINGSGAME] = std::make_unique<rtype::ControlsSettings>(controlsSettingsGameScene);
    this->allScenes[_Scene::AUDIOSETTINGSGAME] = std::make_unique<rtype::AudioSettings>(audioSettingsGameScene);

    this->allScenes[_Scene::LOADING] = std::make_unique<rtype::Loading>(loadingScene);

    this->allScenes[_Scene::RESULT] = std::make_unique<rtype::ResultScene>(resultScene);

    float ANIMATION_SPEED = 100.0f;

    // ------------------ MAIN LOOP ------------------ //
    while (!WindowShouldClose() && !shouldExit) {
        float deltaTime = GetFrameTime();

        this->_clientToGame->handleMessages();

        this->allScenes[this->_scene]->update(deltaTime, GetTime());

        this->allScenes[this->_scene]->draw();
    }

    rtype::net::message<MainServerMsgTypes> msg;
    msg.header.id = MainServerMsgTypes::Logout;
    msg << this->_lobby_id << this->_me;
    this->_clientToServer->Send(msg);
    CloseWindow();
}
