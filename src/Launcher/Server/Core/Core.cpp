#include "./Core.hpp"

using nlohmann::json;

lau::Core::Core(int port): _port(port), _db(LIB_DTB_ENV_PATH), _tokenExpiration(2)
{

}

void lau::Core::init()
{
    // _db = std::make_unique<dtb::MySqlDb>(LIB_DTB_ENV_PATH);
    if (!_db.connect()) {
        std::cout << "[LauncherServer]: Connection to database impossible" << std::endl;
        exit(EXIT_FAILURE);
    }
    addRoute();
}

void lau::Core::run()
{
    if (!_server.start()) {
        std::cout << "[LauncherServer]: Server cannot be started" << std::endl;
        exit(EXIT_FAILURE);
    }
    while (1) {
        _server.update();
    }
}

void lau::Core::addRoute()
{
    _server.addRoute("login", [this](const json &j, tcp::Connection &c) {
        login(j, c);
    });
    _server.addRoute("loginToken", [this](const json &j, tcp::Connection &c){
        loginToken(j, c);
    });
    _server.addRoute("signup", [this](const json &j, tcp::Connection &c) {
        signup(j, c);
    });
}

void lau::Core::signup(const json &req, tcp::Connection &con)
{
    if (req.find("data") == req.end())
        return missingArguments(con, "data");
    if (req["data"].find("pseudo") == req["data"].end())
        return missingArguments(con, "data.pseudo");
    if (req["data"].find("password") == req["data"].end())
        return missingArguments(con, "data.password");
    std::string salt = generateSalt(true); 
    std::string password = hashString(req["data"]["password"], salt);
    std::string sqlQuery = "insert into clientest (pseudo, password, kslt)\n"
    "values (?, ?, ?)";

    

    json sqlResult = _db.executeQuery(sqlQuery, std::make_tuple(req["data"]["pseudo"].get<std::string>(), password.c_str(), salt));
    
    if (sqlResult["code"] != dtb::OK) {
        con.writeMessage(sqlResult.dump());
    } else {
        con.writeMessage(json{
            {"message", "succesfully signed up"},
            {"code", OK}
        }.dump());
    }
}

void lau::Core::loginToken(const json &req, tcp::Connection &con)
{
    if (req.find("data") == req.end())
        return missingArguments(con, "data");
    if (req["data"].find("pseudo") == req["data"].end())
        return missingArguments(con, "data.pseudo");
    if (req["data"].find("token") == req["data"].end())
        return missingArguments(con, "data.token");
    
    if (!verifyToken(req["data"]["pseudo"], req["data"]["token"], con))
        return;
    con.writeMessage(json{
        {"message", "connection succedded"},
        {"pseudo", req["data"]["pseudo"]},
        {"code", LOGGEDIN}
    }.dump());
    std::string query = "update clientest\n"
                        "set tokenExpiration=adddate(current_timestamp(), interval ? day)\n"
                        "where pseudo=?";
    _db.executeQuery(query, std::make_tuple(_tokenExpiration, req["data"]["pseudo"].get<std::string>()));
}


void lau::Core::login(const json &req, tcp::Connection &con)
{
    if (req.find("data") == req.end())
        return missingArguments(con, "data");
    if (req["data"].find("pseudo") == req["data"].end())
        return missingArguments(con, "data.pseudo");
    if (req["data"].find("password") == req["data"].end())
        return missingArguments(con, "data.password");

    json sqlResult = _db.selectLoginLauncher(std::make_tuple(req["data"]["pseudo"].get<std::string>()));
    
    if (sqlResult["code"] != dtb::OK) {
        con.writeMessage(json{
            {"message", "incorrect pseudo or password"},
            {"code", WRONG}
        }.dump());
        return;
    }

    if (hashString(req["data"]["password"], sqlResult["kslt"]) != sqlResult["password"])
        con.writeMessage(json{
            {"message", "incorrect pseudo or password"},
            {"code", WRONG}
        }.dump());
    else {
        std::string token = generateToken();
        std::string query = "update clientest\n"
                        "set token=?, tokenExpiration=adddate(current_timestamp(), interval ? day)\n"
                        "where pseudo=?";
        _db.executeQuery(query, std::make_tuple(token, _tokenExpiration, sqlResult["pseudo"].get<std::string>()));
        con.writeMessage(json{
            {"message", "connection succedded"},
            {"pseudo", sqlResult["pseudo"]},
            {"token", token},
            {"code", LOGGEDIN}
        }.dump());
    }
}

void lau::Core::missingArguments(tcp::Connection &cli, const std::string &arg)
{
    json msg = {
        {"StatusCode", ERROR_ARCARDE_SERVER_BAD_ARGUMENTS},
        {"Message", "Fields " + arg + " is missing"}
    };
    cli.writeMessage(msg.dump());
}

std::string lau::Core::generateSalt(bool hex)
{
    const unsigned int BLOCKSIZE = 64;
    CryptoPP::SecByteBlock scratch(BLOCKSIZE);
    std::string encoded;

    // std::cout << scratch.SizeInBytes() << std::endl;
    CryptoPP::AutoSeededRandomPool rng;

    rng.GenerateBlock(scratch, scratch.size());
    // CryptoPP::StringSource ss((const CryptoPP::byte *)scratch, 32, true, new CryptoPP::HexEncoder(
    //     new CryptoPP::StringSink(encoded)
    // ));
    if (hex) {
        fromHashToHex(scratch, encoded, scratch.size() / 4);
        return (encoded);
    } 
    return std::string((const char *)scratch.data(), scratch.size());
}

std::string lau::Core::hashString(const std::string &toHash, const std::string &st)
{
    std::string salt;
    std::string result;
    CryptoPP::SHA256 hash;
    std::string encoded;

    fromHexToHash(st, salt);

    salt += toHash;
    hash.Update((const CryptoPP::byte *)salt.data(), salt.size());
    result.resize(hash.DigestSize());
    hash.Final((CryptoPP::byte *)&result[0]);


    fromHashToHex(result, encoded, hash.DigestSize());
    return (encoded);
    // return result;
}

std::string lau::Core::hashString(const std::string &toHash)
{
    std::string salt = generateSalt(true);
    std::string result;
    CryptoPP::SHA256 hash;
    std::string encoded;

    salt += toHash;
    hash.Update((const CryptoPP::byte *)salt.data(), salt.size());
    result.resize(hash.DigestSize());
    hash.Final((CryptoPP::byte *)&result[0]);

    CryptoPP::StringSource ss((const CryptoPP::byte *)result.data(), hash.DigestSize(), true, new CryptoPP::HexEncoder(
        new CryptoPP::StringSink(encoded)
    ));
    return (result);
}

void lau::Core::fromHashToHex(const std::string &src, std::string &dest, int size)
{
    CryptoPP::StringSource ss((const CryptoPP::byte *)src.data(), size, true, new CryptoPP::HexEncoder(
        new CryptoPP::StringSink(dest)
    ));
}

void lau::Core::fromHashToHex(CryptoPP::SecByteBlock &src, std::string &dest, int size)
{
    sizeof(CryptoPP::SecByteBlock);
    CryptoPP::StringSource ss(src.BytePtr(), size, true, new CryptoPP::HexEncoder(
        new CryptoPP::StringSink(dest)
    ));
}

void lau::Core::fromHexToHash(const std::string &src, std::string &dest)
{
    CryptoPP::StringSource ssalt((const CryptoPP::byte *)src.data(), src.size(), true, new CryptoPP::HexDecoder(
        new CryptoPP::StringSink(dest)
    ));
}

void lau::Core::fromHashToBase64(const std::string &src, std::string &dest)
{
    CryptoPP::StringSource ss((const CryptoPP::byte *)src.data(), src.size(), true, new CryptoPP::Base64Encoder(
        new CryptoPP::StringSink(dest), false
    ));
    // std::cout << dest << std::endl;
}

void lau::Core::fromBase64ToHash(const std::string &src, std::string &dest)
{
    CryptoPP::StringSource ssalt((const CryptoPP::byte *)src.data(), src.size(), true, new CryptoPP::Base64Decoder(
        new CryptoPP::StringSink(dest)
    ));
}

std::string lau::Core::generateToken()
{
    std::string token = generateSalt(false);
    std::string encoded;
    std::string result;
    CryptoPP::SHA256 hash;

    hash.Update((const CryptoPP::byte *)token.data(), token.size());
    result.resize(hash.DigestSize());
    hash.Final((CryptoPP::byte *)&result[0]);
    fromHashToBase64(result, encoded);
    return (encoded);
}

bool lau::Core::verifyToken(const std::string &pseudo, const std::string &token, tcp::Connection &con)
{
    json sqlResult = _db.selectToken(std::make_tuple(pseudo, pseudo));

    if (sqlResult["code"] != dtb::OK || token != sqlResult["token"]) {
        con.writeMessage(json{
            {"message", "incorrect token, you need to login back"},
            {"code", RELOG}
        }.dump());
        return false;
    }
    if (!sqlResult["isValid"].get<bool>()) {
        con.writeMessage(json{
            {"message", "incorrect token, you need to login back"},
            {"code", RELOG}
        }.dump());
        return false;
    }
    return true;
}