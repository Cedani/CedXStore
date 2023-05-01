#include "./Core.hpp"

using nlohmann::json;

lau::Core::Core(int port): _port(port), _db(LIB_DTB_ENV_PATH)
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
    std::string salt = generateSalt(); 
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

void lau::Core::login(const json &req, tcp::Connection &con)
{
    if (req.find("data") == req.end())
        return missingArguments(con, "data");
    if (req["data"].find("pseudo") == req.end())
        return missingArguments(con, "data.pseudo");
    if (req["data"].find("password") == req.end())
        return missingArguments(con, "data.password");
    json sqlQuery = {
        {"command", "select"},
        {"table", TABLE},
        {"fields", {"pseudo", "password", "kslt"}},
        {"where", {
            {{"label", "pseudo"}, {"operator", "="}}
        }},
        {"data", {
            {{"label", "pseudo"}, {"value", req["data"]["pseudo"]}}
        }}
    };

    json sqlResult;
    // json sqlResult = _db->executeQuery(sqlQuery);
    json toSend;
    
    if (sqlResult["code"] != dtb::OK) {
        con.writeMessage(sqlResult);
        return;
    }

    if (sqlResult.find("data") == sqlResult.end()) {
        con.writeMessage(json{
            {"message", "incorrect pseudo or password"},
            {"code", WRONG}
        });
        return;
    }

    std::string salt = sqlResult["data"]["kslt"]; 
    std::string password = hashString(sqlResult["data"]["password"], salt);

    if (password != sqlQuery["data"]["password"])
        con.writeMessage(json{
            {"message", "incorrect pseudo or password"},
            {"code", WRONG}
        });
    else
        con.writeMessage(json{
            {"message", "connection succedded"},
            {"code", OK}
        });
}

void lau::Core::missingArguments(tcp::Connection &cli, const std::string &arg)
{
    json msg = {
        {"StatusCode", ERROR_ARCARDE_SERVER_BAD_ARGUMENTS},
        {"Message", "Fields " + arg + " is missing"}
    };
    cli.writeMessage(msg.dump());
}

std::string lau::Core::generateSalt()
{
    const unsigned int BLOCKSIZE = 16 * 8;
    CryptoPP::SecByteBlock scratch(BLOCKSIZE);
    std::string encoded;

    CryptoPP::AutoSeededRandomPool rng;

    rng.GenerateBlock(scratch, scratch.size());
    CryptoPP::StringSource ss((const CryptoPP::byte *)scratch, 32, true, new CryptoPP::HexEncoder(
        new CryptoPP::StringSink(encoded)
    ));
    return (encoded);
}

std::string lau::Core::hashString(const std::string &toHash, const std::string &st)
{
    std::string salt;
    std::string result;
    CryptoPP::SHA256 hash;
    std::string encoded;

    CryptoPP::StringSource ssalt((const CryptoPP::byte *)st.data(), st.size(), true, new CryptoPP::HexDecoder(
        new CryptoPP::StringSink(salt)
    ));

    salt += toHash;
    hash.Update((const CryptoPP::byte *)salt.data(), salt.size());
    result.resize(hash.DigestSize());
    hash.Final((CryptoPP::byte *)&result[0]);


    CryptoPP::StringSource ss((const CryptoPP::byte *)result.data(), hash.DigestSize(), true, new CryptoPP::HexEncoder(
        new CryptoPP::StringSink(encoded)
    ));
    return (encoded);
}

std::string lau::Core::hashString(const std::string &toHash)
{
    std::string salt = generateSalt();
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