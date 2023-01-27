#include "RoomHandler.hpp"

using nlohmann::json;

rmh::RoomHandler::RoomHandler()
{

}

rmh::RoomHandler::~RoomHandler()
{

}

const rmh::room &rmh::RoomHandler::createNewRoom(int maxc, client &&cli, const std::string &roomName = "", const std::string &roomCode="")
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    room newRoom(maxc, 1, roomCode, roomName);
    newRoom._clients.push_back(std::move(cli));
    _rooms.push_back(std::move(newRoom));
    return _rooms.back();
}

const rmh::room *rmh::RoomHandler::getRoom(const std::string &name) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    for (const auto &r: _rooms)
        if (r._name == name)
            return &r;
    return nullptr;
}

const rmh::room *rmh::RoomHandler::getRoom(int id) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    for (const auto &r: _rooms)
        if (r._id == id)
            return &r;
    return nullptr;
}

rmh::room *rmh::RoomHandler::getRoom(const std::string &name)
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    for (auto &r: _rooms)
        if (r._name == name)
            return &r;
    return nullptr;
}

rmh::room *rmh::RoomHandler::getRoom(int id)
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    for (auto &r: _rooms)
        if (r._id == id)
            return &r;
    return nullptr;
}

int rmh::RoomHandler::addToRoom(int id, client &&cli, const std::string &code)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    room *tmp = getRoom(id);
    if (!tmp)
        return (0);
    if (tmp->_passCode != code)
        return (-1);
    if (tmp->_nbClient == tmp->_nbMaxClients)
        return (-2);
    tmp->_clients.push_back(std::move(cli));
    tmp->_nbClient += 1;
    return (1);
}

int rmh::RoomHandler::addToRoom(const std::string &name, client &&cli, const std::string &code)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    room *tmp = getRoom(name);
    if (!tmp)
        return (0);
    if (tmp->_passCode != code)
        return (-1);
    if (tmp->_nbClient == tmp->_nbMaxClients)
        return (-2);
    tmp->_clients.push_back(std::move(cli));
    tmp->_nbClient += 1;
    return (1);
}

const std::string rmh::RoomHandler::getRoomInfo(const std::string &name)
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    const room *tmpRoom = getRoom(name);

    if (!tmpRoom)
        return ("");
    json info;
    info["roomName"] = tmpRoom->_name;
    info["nbPlayers"] = tmpRoom->_nbClient;
    info["MaxPlayer"] = tmpRoom->_nbMaxClients;

    for (int i = 0; i < tmpRoom->_nbClient; ++i) {
        std::string label("client" + std::to_string(i));
        info["clients"][label]["name"] = tmpRoom->_clients[i]._name;
        info["clients"][label]["avatar"]["hasAvatar"] = tmpRoom->_clients[i]._avatar._hasAvatar;
        info["clients"][label]["avatar"]["path"] = tmpRoom->_clients[i]._avatar._path;
    };
    return (info.dump());
}

const std::string rmh::RoomHandler::getRoomInfo(int id)
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    const room *tmpRoom = getRoom(id);

    if (!tmpRoom)
        return ("");
    json info;
    info["roomName"] = tmpRoom->_name;
    info["nbPlayers"] = tmpRoom->_nbClient;
    info["MaxPlayer"] = tmpRoom->_nbMaxClients;

    for (int i = 0; i < tmpRoom->_nbClient; ++i) {
        std::string label("client" + std::to_string(i));
        info["clients"][label]["name"] = tmpRoom->_clients[i]._name;
        info["clients"][label]["avatar"]["hasAvatar"] = tmpRoom->_clients[i]._avatar._hasAvatar;
        info["clients"][label]["avatar"]["path"] = tmpRoom->_clients[i]._avatar._path;
    };
    return (info.dump());
}

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

// void rmh::RoomHandler::quitRoom(int id)