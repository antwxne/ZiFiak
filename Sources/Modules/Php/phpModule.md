# PHP-CGI Module

## Description

This module is execute php by request.

The type of request method can be GET or POST.

## Description

You have a configFile like this 
```
    "PHP-CGI": {
      "path_info" : "/",
      "full_path" : "/opt/homebrew/bin/php-cgi",
      "remote_host" : "wsh",
      "script_filename" : [
        {
          "url": "/toto",
          "filePath": "./lol.php"
        },
        {
          "url": "/pipi",
          "filePath": "./lol.php"
        }
      ],
      "ip_client" : "127.0.0.1",
      "exec_name" : "lol.php",
      "protocol" : "HTTP/1.1",
      "server_name" : "ZiFiak",
      "servet_port" : "8080",
      "server_software" : "Ziapi/4.0.0",
      "remote_ident" : "",
      "auth_type" : "",
      "content_type" : "text/php",
      "gateway_interface" : "CGI/1.1",
      "path_translated" : "translationmagle",
      "remote_addr" : "remote",
      "remote_user" : "bibicharlie",
      "server_addr" : "localhost"
    }
```
And you must enter good information