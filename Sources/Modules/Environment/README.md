##Module: Environment

####Description:

- This module is used for add/delete/modify environment variable 

####Config:

- To implement this module correctly you need to add a variable in you server config file named "Environment" wich contain the "activated" variable, respectively to able/disable the module.
```json
{
    "Environment": {
        "activated": true
    }
}
```

####How it works ?

- In PreProcess, if the module is activated, it will check in the body if there is the key:
  - setenv
  - unsetenv

  With this key, the module can detect the modification and alert handler module thanks to the context. There is some examples:

|Key          |Value                  |Description                      |
|-------------|-----------------------|---------------------------------|
|setenv       |SERVER_NAME=toto       |change server name to "toto"     |
|setenv       |test=HelloWorld        |set a new environment variable   |
|unsetenv     |REMOTE_USER            |remove remote user               |