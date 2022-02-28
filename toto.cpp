#include <vector>
#include <string>
#include <map>
#include <filesystem>
#include <iostream>
#include <array>

int main() {
    std::vector<std::string> _env;


    _env.push_back("CONTENT_TYPE=text/php");
    _env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    _env.push_back("PATH_INFO=/");
    _env.push_back("PATH_TRANSLATED=path_translated");
    _env.push_back("REMOTE_IDENT=");
    _env.push_back("REMOTE_USER=bibicharlie");
    _env.push_back("SCRIPT_FILENAME=lol.php");
    _env.push_back("SCRIPT_NAME=lol.php");
    _env.push_back("SERVER_NAME=ZiFiak");
    _env.push_back("SERVER_PORT=8080");
    _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    _env.push_back("SERVER_SOFTWARE=Ziapi/4.0.0");
    _env.push_back("REMOTE_HOST=wsh");
    _env.push_back("REMOTE_ADDR=127.0.0.1");
    _env.push_back("AUTH_TYPE=");
    _env.push_back("REQUEST_METHOD=GET");
    _env.push_back("QUERY_STRING=");
    _env.push_back("CONTENT_LENGTH=0");
    _env.push_back("HTTP_ACCEPT=*/*");
    _env.push_back("HTTP_ACCEPT_LANGUAGE=fr_CA");
    _env.push_back("HTTP_USER_AGENT=Mozilla/5.0");
    _env.push_back("HTTP_COOKIE=");
    _env.push_back("REDIRECT_STATUS=200");
    //_env.push_back("HTTP_REFERER=" + req.headers.at(ziapi::http::header::kReferer));

    int i = 0;
    int pos = 0;
    int tokenPos = 0;
   // char buf[128];
    std::array<char, 128> buf;
    std::string body = "";
    std::string env = "";
    std::string resp = "";
    std::string token;
    std::string lilToken;
    std::map<std::string, std::string> headers;
    try {
        while (i != _env.size()) {
            env += " " + _env[i];
            i++;
        }

        //std::cout << env << std::endl;
        auto file = popen(("env -i " + env + " /opt/homebrew/bin/php-cgi").c_str(), "r");
        //auto file = popen("ls", "r");
        // std::cout << "env " << ("env -i " + env + " /opt/homebrew/bin/php-cgi") << std::endl;
        if (file != NULL) {
            while (std::fgets(buf.data(), 128, file) != nullptr) {
                resp += buf.data();
           //     std::cout <<"ici == " << resp << std::endl;
            }
            while ((pos = resp.find("\r\n")) != std::string::npos) {
                token = resp.substr(0, pos);
                while ((tokenPos = token.find(":")) != std::string::npos) {
                    lilToken = token.substr(0, tokenPos);
                    token.erase(0, tokenPos + 1);
                    headers.insert({lilToken, token});
                }
                resp.erase(0, pos + 2);
            }
            body = resp;
        } else {
            throw std::runtime_error("error CreateProces");
        }
    }
    catch (const std::exception &e) {
        // res.status_code = ziapi::http::Code::kInternalServerError;
        // res.reason = "Error in the path or with the cgi.";
    }
    std::cout << body << std::endl;
}
