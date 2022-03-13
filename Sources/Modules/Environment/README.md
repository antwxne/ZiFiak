##Module: Environment

####Description:

- This module is used for add/delete/modify environment variable 

####Config:

- This module does not need any configuration

####How it works ?

- In PreProcess, it will check in the body if there is the key:
  - setenv
  - unsetenv

  With this key, the module can detect the modification and alert handler module thanks to the context. There is some examples:

|Key          |Value                  |Description                      |
|-------------|-----------------------|---------------------------------|
|setenv       |SERVER_NAME=toto       |change server name to "toto"     |
|setenv       |test=HelloWorld        |set a new environment variable   |
|unsetenv     |REMOTE_USER            |remove remote user               |