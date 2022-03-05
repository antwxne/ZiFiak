## Module: Ruby CGI

#### Description:

- This module has for objectives to interpret ruby code.

#### Config:

To implement this module correctly, you need to add a variable in your server config file named "ruby-cgi" which is an object containing 1 variables "path" to know the location the ruby-cgi executable.

```json
{
    "RUBY-CGI": {
      "path_info" : "/",
      "full_path" : "Sources/Modules/Ruby/lol.rb",
      "remote_host" : "wsh",
      "script_filename" : [
        {
          "url": "/popo",
          "filePath": "/Users/tcampos/Epitech/WorkInProgress/ZiFiak/Sources/Modules/Ruby/lol.rb"
        }
      ],
      "ip_client" : "127.0.0.1",
      "exec_name" : "lol.rb",
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

This module setup some environment variables, for the good use of ruby-cgi, like the path of the CGI executable or the type of the request.
If the path isn't good or an error is returned from the ruby-cgi, this module will return an error value InternalServerError.