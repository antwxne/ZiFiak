##Module: SetID

####Description:

- This module is create a unique ID for each requests

####Config:

- To implement this module correctly you need to add a variable in you server config file named "SetID" wich contain the "activated" variable, respectively to able/disable the module.
```json
{
    "SetID": {
        "activated": true
    }
}
```

####How it works ?

- In PreProcess, if the module is activated, it create a unique id that depend of number of the previous requests.