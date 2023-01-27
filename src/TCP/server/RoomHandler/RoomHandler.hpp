#pragma once
#include <iostream>
#include <shared_mutex>
#include <optional>
#include "json.hpp"
#include "Connection.hpp"
// #include ""
// #include <boost/asio.hpp>

namespace rmh {

    
    typedef enum {
        WAITING,
        LAUNCHING
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
        client_s(const tcp::Connection &con, const std::string &name = "Player", avatar av = avatar()): _con(con), _name(name), _avatar(av){}
        const tcp::Connection &_con;
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
            //GETTER
            const room *getRoom(int id) const;
            const room *getRoom(const std::string &name) const;
            room *getRoom(const std::string &name);
            room *getRoom(int id);
            const std::string getRoomInfo(const std::string &);
            const std::string getRoomInfo(int);

        public:
            // ROOM EDITOR
            const room & createNewRoom(int maxc, client &&cli, const std::string &roomName = "", const std::string &roomCode="");
            int addToRoom(int id, client &&cli, const std::string &code="");
            int addToRoom(const std::string &name, client &&cli, const std::string &code="");
            void deleteRoom(int id);
            void deleteRoom(const std::string &);
            int quitRoom(int id);
            int quitRoom(const std::string &);
        private:
            mutable std::shared_mutex _mutex;
            std::vector<room> _rooms;
    };
}