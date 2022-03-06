## Module: Ruby CGI

#### Description:

- This module has for objectives to interpret code from all language with the cgi method.

#### Config:

To implement this module correctly, you need to add variables in your server config file named "GLOBAL-CGI" which is an object containing 1 variables "execPath" to know the location the cgi executable.

```json
{
    "GLOBAL-CGI": {
      "path_info" : "/",
      "remote_host" : "wsh",
      "script_filename" : [
        {
          "url": "/rubyGet",
          "filePath": "/Users/tcampos/Epitech/WorkInProgress/ZiFiak/Sources/Modules/Global/test/rubyGet.rb",
          "execPath": "Sources/Modules/Global/test/rubyGet.rb"
        },
        {
          "url": "/phpGet",
          "filePath": "/Users/tcampos/Epitech/WorkInProgress/ZiFiak/Sources/Modules/Global/test/phpGet.php",
          "execPath": "/opt/homebrew/bin/php-cgi"
        }
      ],
      "ip_client" : "127.0.0.1",
      "protocol" : "HTTP/1.1",
      "server_name" : "ZiFiak",
      "server_port" : "8080",
      "server_software" : "Ziapi/5.0.0",
      "remote_ident" : "",
      "auth_type" : "",
      "gateway_interface" : "CGI/1.1",
      "path_translated" : "translationmagle",
      "remote_addr" : "remote",
      "remote_user" : "bibicharlie",
      "server_addr" : "localhost"
    }
}
```

#### How it works ?

This module setup some environment variables, for the good use of global-cgi, like the path of the CGI executable or the type of the request.
If the path isn't good or an error is returned from the global-cgi, this module will return an error value InternalServerError.