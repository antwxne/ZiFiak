## Module: Php CGI

#### Description:

- This module has for objectives to interpret php code.

#### Config:

To implement this module correctly, you need to add a variable in your server config file named "php-cgi" which is an object containing 1 variables "path" to know the location the php-cgi executable.

```json
{
    "modules": {
        "PHP-CGI": {
            "path": "/usr/bin/php-cgi"
        }
    }
}
```

#### How it works ?

This module setup some environment variables, for the good use of php-cgi, like the path of the CGI executable or the type of the request.
If the path isn't good or an error is returned from the php-cgi, this module will return an error value InternalServerError.