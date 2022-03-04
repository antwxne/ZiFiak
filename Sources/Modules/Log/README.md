##Module: Log

####Description:

- This module is use to write server log in a log file

####Config:

- To implement this module correctly you need to add a variable in you server config file named "Log" wich contain two variables, "activated" and "path", respectively to able/disable the module and to specify the path of the log file.
```json
{
    "Log": {
        "activated": true,
        "path": "/tmp/zialog.txt"
    }
}
```

####How it works ?

- In PostProcess, if the module is activated, it writes some informations about the request in a log file with the following format => [method target] statuc_code: reason