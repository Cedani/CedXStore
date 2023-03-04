#pragma once
#include <iostream>
#include <shared_mutex>
#include <optional>
#include <nlohmann/json.hpp>
#include "Connection.hpp"
// #include ""
// #include <boost/asio.hpp>

namespace rmh {

    
    typedef enum {
        WAITING,
        LAUNCHING,
        ROOM_FULL = 300,
        NOT_IN_ROOM = 400,
        WRONG_CODE = 401,
        NOT_FOUND = 404,
        ROOM_SUCCESS = 200
    }roomStatus;

    typedef struct avatar_s {
        avatar_s(const std::string &path = "", bool hasAv = false): _path(path), _hasAvatar(false){};
        std::string _path;
        bool _hasAvatar;
        void setAvatar(const std::string &path) {
            if (path.empty())
                return;
            _path = path;
            _hasAvatar = true;
        }
    }avatar;

    typedef struct client_s {
        client_s(const std::string &name = "Player", avatar av = avatar()): _name(name), _avatar(av){}
        // client_s(const struct client_s &) = delete;
        // struct client_s & operator=(const struct client_s &) = delete;
        // const tcp::Connection &_con;
        std::string _name;
        avatar _avatar;
    }client;

    typedef struct room_s {
        room_s(int maxc = 4, int nbc = 0, const std::string &pC = "", const std::string &name = "") : _nbMaxClients(maxc), _nbClient(nbc), _passCode(pC), _name(name) {};
        int _id;
        int _nbMaxClients;
        int _nbClient;
        std::vector<client> _clients;
        std::string _passCode;
        std::string _name;
    }room;

    class RoomHandler {

        public:
            RoomHandler();
            ~RoomHandler();

        public:
            // ROOM EDITOR
            nlohmann::json createNewRoom(int maxc, const client &cli, const std::string &roomName = "", const std::string &roomCode="");
            // nlohmann::json addToRoom(int id, client &cli, const std::string &code="");
            nlohmann::json addToRoom(const std::string &name, const client &cli, const std::string &code="");
            // nlohmann::json quitRoom(int id);
            nlohmann::json quitRoom(const std::string &, const std::string &);
            // nlohmann::json quitRoom(const std::string &, int);

        public:
            //GETTER
            // const room *getRoom(int id) const;
            const room *getRoom(const std::string &name) const;
            room *getRoom(const std::string &name);
            // room *getRoom(int id);
            const nlohmann::json getRoomInfo(const std::string &);
            // const nlohmann::json getRoomInfo(int);



        private:
            mutable std::shared_mutex _mutex;
            std::vector<room> _rooms;
            int _idCount;

        private:
            // PRIVATE FUNCTION
            void deleteRoom(int id);
            void deleteRoom(const std::string &);
    };
}