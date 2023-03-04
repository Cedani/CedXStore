#include "RoomHandler.hpp"

using nlohmann::json;

rmh::RoomHandler::RoomHandler(): _idCount(0)
{

}

rmh::RoomHandler::~RoomHandler()
{

}

json rmh::RoomHandler::createNewRoom(int maxc, const client &cli, const std::string &roomName, const std::string &roomCode)
{
    room newRoom(maxc, 1, roomCode, roomName);
    newRoom._id = _idCount;
    if (newRoom._name.empty())
        newRoom._name = "room" + std::to_string(newRoom._id);

    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        newRoom._clients.push_back(cli);
        _rooms.push_back(newRoom);
        ++_idCount;
    }
    return getRoomInfo(_rooms.back()._name);
}

const rmh::room *rmh::RoomHandler::getRoom(const std::string &name) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    for (const auto &r: _rooms)
        if (r._name == name)
            return &r;
    return nullptr;
}

// const rmh::room *rmh::RoomHandler::getRoom(int id) const
// {
//     std::shared_lock<std::shared_mutex> lock(_mutex);
//     for (const auto &r: _rooms)
//         if (r._id == id)
//             return &r;
//     return nullptr;
// }

rmh::room *rmh::RoomHandler::getRoom(const std::string &name)
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    for (auto &r: _rooms)
        if (r._name == name)
            return &r;
    return nullptr;
}

// rmh::room *rmh::RoomHandler::getRoom(int id)
// {
//     std::shared_lock<std::shared_mutex> lock(_mutex);
//     for (auto &r: _rooms)
//         if (r._id == id)
//             return &r;
//     return nullptr;
// }

// json rmh::RoomHandler::addToRoom(int id, client &cli, const std::string &code)
// {
//     room *tmp = getRoom(id);
//     if (!tmp)
//         return json{
//             {"statusCode", NOT_FOUND},
//             {"Message", "room " + std::to_string(id) + " not found"}
//         };
//     if (tmp->_passCode != code)
//         json{
//             {"statusCode", WRONG_CODE},
//             {"Message", "code given for room " + std::to_string(id) + " is incorrect"}
//         };
//     if (tmp->_nbClient == tmp->_nbMaxClients)
//         json{
//             {"statusCode", ROOM_FULL},
//             {"Message", "room " + std::to_string(id) + " is already full"}
//         };
//     json result = json{
//         {"statusCode", ROOM_SUCCESS},
//         {"message", cli._name + " succesfully joined room " + std::to_string(id)}
//     };
//     {
//         std::unique_lock<std::shared_mutex> lock(_mutex);
//         tmp->_clients.push_back(std::move(cli));
//         tmp->_nbClient += 1;
//     }
//     result["roomInfo"] = getRoomInfo(id);
//     return (result);
// }

json rmh::RoomHandler::addToRoom(const std::string &name, const client &cli, const std::string &code)
{
    room *tmp = getRoom(name);
    if (!tmp)
        return json{
            {"statusCode", NOT_FOUND},
            {"Message", "room " + name + " not found"}
        };
    if (tmp->_passCode != code)
        json{
            {"statusCode", WRONG_CODE},
            {"Message", "code given for room " + name + " is incorrect"}
        };
    if (tmp->_nbClient == tmp->_nbMaxClients)
        json{
            {"statusCode", ROOM_FULL},
            {"Message", "room " + name + " is already full"}
        };
    json result = json{
        {"statusCode", ROOM_SUCCESS},
        {"message", cli._name + " succesfully joined room " + name}
    };

    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        tmp->_clients.push_back(std::move(cli));
        tmp->_nbClient += 1;
    }
    result["roomInfo"] = getRoomInfo(name);
    return (result);
}

const json rmh::RoomHandler::getRoomInfo(const std::string &name)
{
    const room *tmpRoom = getRoom(name);

    if (!tmpRoom)
        return json{
            {"statusCode", NOT_FOUND},
            {"Message", "room " + name + " not found"}
        };
    json info;

    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        info["roomName"] = tmpRoom->_name;
        info["nbPlayers"] = tmpRoom->_nbClient;
        info["MaxPlayer"] = tmpRoom->_nbMaxClients;

        for (int i = 0; i < tmpRoom->_nbClient; ++i) {
            std::string label("client" + std::to_string(i));
            info["clients"][label]["name"] = tmpRoom->_clients[i]._name;
            info["clients"][label]["avatar"]["hasAvatar"] = tmpRoom->_clients[i]._avatar._hasAvatar;
            info["clients"][label]["avatar"]["path"] = tmpRoom->_clients[i]._avatar._path;
        };
    }
    return (info);
}

// const json rmh::RoomHandler::getRoomInfo(int id)
// {
//     const room *tmpRoom = getRoom(id);

//     if (!tmpRoom)
//         return json{
//             {"statusCode", NOT_FOUND},
//             {"Message", "room " + std::to_string(id) + " not found"}
//         };
//     json info;

//     {
//         std::shared_lock<std::shared_mutex> lock(_mutex);
//         info["roomName"] = tmpRoom->_name;
//         info["nbPlayers"] = tmpRoom->_nbClient;
//         info["MaxPlayer"] = tmpRoom->_nbMaxClients;

//         for (int i = 0; i < tmpRoom->_nbClient; ++i) {
//             std::string label("client" + std::to_string(i));
//             info["clients"][label]["name"] = tmpRoom->_clients[i]._name;
//             info["clients"][label]["avatar"]["hasAvatar"] = tmpRoom->_clients[i]._avatar._hasAvatar;
//             info["clients"][label]["avatar"]["path"] = tmpRoom->_clients[i]._avatar._path;
//         };
//     }
//     return (info);
// }

void rmh::RoomHandler::deleteRoom(int id)
{
    _rooms.erase(
        std::remove_if(
            _rooms.begin(),
            _rooms.end(),
            [this, &id](const room &rm) {
                return (id == rm._id);
            }
        ),
        _rooms.end()
    );
}

void rmh::RoomHandler::deleteRoom(const std::string &name)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    _rooms.erase(
        std::remove_if(
            _rooms.begin(),
            _rooms.end(),
            [this, &name](const room &rm) {
                return (name == rm._name);
            }
        ),
        _rooms.end()
    );
}

json rmh::RoomHandler::quitRoom(const std::string &name, const std::string &rName)
{
    room *tmp = getRoom(rName);

    if (!tmp) {
        return json{
            {"statusCode", NOT_FOUND},
            {"Message", std::string("room " + rName + " not found")}
        };
    }

    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        tmp->_clients.erase(std::remove_if(
            tmp->_clients.begin(),
            tmp->_clients.end(),
            [&name](const client &cli) {
                return (cli._name == name);
            }
        ), tmp->_clients.end());
    }
    if (tmp->_nbClient == tmp->_clients.size())
        return json{
            {"statusCode", NOT_IN_ROOM},
            {"Message", "client " + name + " was not in room " + rName}
        };
    tmp->_nbClient = tmp->_clients.size();
    if (tmp->_nbClient == 0)
        deleteRoom(rName);
    return json{
        {"statusCode", ROOM_SUCCESS},
        {"Message", "Client " + name + " succesfully quited room " + rName}
    };
}

// json rmh::RoomHandler::quitRoom(const std::string &name, int id)
// {
//     room *tmp = getRoom(id);

//     if (!tmp) {
//         return json{
//             {"statusCode", NOT_FOUND},
//             {"Message", std::string("room " + std::to_string(id) + " not found")}
//         };
//     }
//     {
//         tmp->_clients.erase(std::remove_if(
//             tmp->_clients.begin(),
//             tmp->_clients.end(),
//             [this, &name](const client &cli) {
//                 return (cli._name == name);
//             }
//         ), tmp->_clients.end());
//     }
//     if (tmp->_nbClient == tmp->_clients.size())
//         return json{
//             {"statusCode", NOT_IN_ROOM},
//             {"Message", "client " + name + " was not in room " + std::to_string(id)}
//         };
//     tmp->_nbClient = tmp->_clients.size();
//     if (tmp->_nbClient == 0)
//         deleteRoom(id);
//     return json{
//         {"statusCode", ROOM_SUCCESS},
//         {"Message", "Client " + name + " succesfully quited room " + std::to_string(id)}
//     };
// }

// void rmh::RoomHandler::quitRoom(int id)