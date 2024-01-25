/*
** EPITECH PROJECT, 2023
** RType
** File description:
** Includes
*/

#pragma once
    #include "DatabaseInclude.hpp"
    #include "../ECS/ECS.hpp"
#if defined(_WIN32)
// To avoid conflicting windows.h symbols with raylib, some flags are defined
// WARNING: Those flags avoid inclusion of some Win32 headers that could be required
// by user at some point and won't be included...
//-------------------------------------------------------------------------------------

// If defined, the following flags inhibit definition of the indicated items.
#define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
#define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      // SM_*
#define NOMENUS           // MF_*
#define NOICONS           // IDI_*
#define NOKEYSTATES       // MK_*
#define NOSYSCOMMANDS     // SC_*
#define NORASTEROPS       // Binary and Tertiary raster ops
#define NOSHOWWINDOW      // SW_*
#define OEMRESOURCE       // OEM Resource values
#define NOATOM            // Atom Manager routines
#define NOCLIPBOARD       // Clipboard routines
#define NOCOLOR           // Screen colors
#define NOCTLMGR          // Control and Dialog routines
#define NODRAWTEXT        // DrawText() and DT_*
#define NOGDI             // All GDI defines and routines
#define NOKERNEL          // All KERNEL defines and routines
#define NOUSER            // All USER defines and routines
//#define NONLS             // All NLS defines and routines
#define NOMB              // MB_* and MessageBox()
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // typedef METAFILEPICT
#define NOMINMAX          // Macros min(a,b) and max(a,b)
#define NOMSG             // typedef MSG and associated routines
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // SB_* and scrolling routines
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // Sound driver routines
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#define NOWH              // SetWindowsHook and WH_*
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#define NOCOMM            // COMM driver routines
#define NOKANJI           // Kanji support stuff.
#define NOHELP            // Help engine interface.
#define NOPROFILER        // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX             // Modem Configuration Extensions

// Type required before windows.h inclusion
typedef struct tagMSG *LPMSG;
#define usleep(x) Sleep((x) / 1000)
#include <windows.h>

// Type required by some unused function...
typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#include <objbase.h>
#include <mmreg.h>
#include <mmsystem.h>

// Some required types defined for MSVC/TinyC compiler
#if defined(_MSC_VER) || defined(__TINYC__)
    #include "propidl.h"
#endif
#endif
#if defined (PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif
    //#define _WINDOWS_ // DÃ©sactiver l'inclusion de windows.h dans Raylib
    #include "raylib.h"

#define INCLUDES_NETFRAMEWORK

#define MAIN_SERVER_PORT 4242
#define MAIN_FOR_GAME_PORT 4243
#define DEFAULT_GAME_SERVER_PORT 4244

#define NB_PLAYERS_PER_GAME 2
#define NB_PLAYERS_PER_TEAM 1

#define MAX_GAME_SERVERS 10

#define TICK_RATE 32
#define ROUND_TO_WIN 3

//! this is the comunication protocol between the client and the game server
enum class GameServerMsgTypes : uint32_t
{
    ConnectToGameServer, //* this is the request of the client to the game server (connect to a game server)
    ClientConnected,
    PlayerJump, // this is an example (only for the client)
    OtherPlayerJump, // send the jump of the other player
    SpawnPosition, // send the spawn position of the player
    OtherSpawnPosition, // send the spawn position of the other player
    PlayerMove, // this is an example (only for the client)
    OtherPlayerMove, // send the move of the other player
    PlayerLook, // this is an example (only for the client)
    OtherPlayerLook, // send the jump of the other player
    PlayerShoot, // this is an example (only for the client)
    OtherPlayerShoot, // send the shoot of the other player
    PlayerHitted, // send the player hitted
    SomeoneGotHitted, // send when someone got hitted whatever the team
    NextRound, // send when the round is finished
    UpdateScoreboard, // the score displayed when we press tab
    GameEnded, // send when the game is finished
};

//! this is the comunication protocol between the main server and the game server
enum class CustomCommunicationServer : uint32_t
{
    StartAcceptingClients,

    JoinGame,

    Infos, //* this message is sent by the main server to the game server to tell him his ip and port (infos)
    GameServer, //* this message is sent by the game server to the main server to tell him his ip and port (infos)

    GameFinished, //* this message is sent by the game server to the main server to tell him the game is finished
    GameHistoryInfos, //* this message is sent by the game server to the main server to tell him the game history infos
    GameHistoryPlayerInfos, //* this message is sent by the game server to the main server to tell him the game history player infos
};

//! this is the comunication protocol between the main server and the client
enum class MainServerMsgTypes : uint32_t
{
    Login, //* this is the request of the client to the server
    Register, //* this is the request of the client to the server
    Logout, //* this is the request of the client to the server

    LoginFailed, //* this is the response of the server to the client
    LoginSuccess, //* this is the response of the server to the client

    Success, //* this is the response of the server to the client (when the request is successfull)
    LobbyId, //* this is the response of the server to the client (when the login is succesfull the server send the client lobby id)

    Ready, //* this is the request of the client to the server
    NotReady, //* this is the request of the client to the server
    Profile, //* this is the request of the client to the server (profile per user id)
    GameHistory, //* this is the request of the client to the server (game history per user id)
    Settings, //* this is the request of the client to the server (settings per user id)
    UpdateSettings, //* this is the request of the client to the server (set settings per user id)

    ConnectToGameServer, //* this is the request of the client to the server (connect to a game server)

    DisplaySettings, //* this is the request of the server to the client (display settings)
    AudioSettings, //* this is the request of the server to the client (audio settings)
    Binds, //* this is the request of the server to the client (binds)
    MouseSettings, //* this is the request of the server to the client (mouse settings)
    ProfileInfos, //* this is the request of the server to the client (profile infos)
    GameHistoryInfos, //* this is the request of the server to the client (game history infos)

    JoinFriend, //* this is the request of the client to the server (join a friend in a lobby)
    LeaveLobby, //* this is the request of the client to the server (leave a friend in a lobby)
    AddFriend, //* this is the request of the client to the server (add a friend)
    RemoveFriend, //* this is the request of the client to the server (remove a friend)
    AcceptFriend, //* this is the request of the client to the server (accept a friend)
    RefuseFriend, //* this is the request of the client to the server (refuse a friend)

    SearchFriend, //* this is the request of the client to the server (search a user by name to add him as a friend)

    PendingFiendRequest, //* this is the request of the server to the client (pending friend request)

    LobbyInfos, //* this is the request of the both (infos of profile_t in the lobby)
    FriendsInfos, //* this is the request of the server to the client (friend connected)
    NewFriendConnected, //* this is the request of the server to the client (new friend connected)
    Alone, //* this is the request of the server to the client (when the client is alone in the lobby)
};

typedef struct login_s {
    char username[21];
    char password[21];
} login_t;

typedef struct player_info_s {
    char username[21];
    uint16_t id;
    uint16_t score;
    bool team;
    int kills;
    int deaths;
    int assists;
} player_info_t;

typedef struct gameServerInfo_s
{
    char ip[16];
    uint16_t port;
    bool isGameStarted;
    uint16_t maxPlayers;
    uint16_t nbPlayers;
    int map;
    player_info_t players[10];
} gameServerInfo_t;

#define INCLUDES_ECS
