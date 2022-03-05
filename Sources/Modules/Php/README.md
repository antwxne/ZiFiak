## Module: Php CGI

#### Description:

- This module has for objectives to interpret php code.

#### Config:

To implement this module correctly, you need to add a variable in your server config file named "php-cgi" which is an object containing 1 variables "path" to know the location the php-cgi executable.

```json
{
    "PHP-CGI": {
      "path_info" : "/",
      "full_path" : "/opt/homebrew/bin/php-cgi",
      "remote_host" : "wsh",
      "script_filename" : [
        {
          "url": "/toto",
          "filePath": "/Users/tcampos/Epitech/WorkInProgress/ZiFiak/Sources/Modules/Php/lol.php"
        }
      ],
      "ip_client" : "127.0.0.1",
      "exec_name" : "lol.php",
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
    },
}
```

#### How it works ?

This module setup some environment variables, for the good use of php-cgi, like the path of the CGI executable or the type of the request.
If the path isn't good or an error is returned from the php-cgi, this module will return an error value InternalServerError.