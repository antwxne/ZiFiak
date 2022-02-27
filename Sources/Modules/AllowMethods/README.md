##Module: Allow Methods

####Description:

- This module has for objectif to allow only some http methods for on a server for a directory and sub directories.

####Config:

To implement this module correctly you need to add a variable in you server config file named "AllowMethods" wich is a list of objects containing 2 variables "path" to know the location in wich the methods are restricted, and "methods" to list all allowed methods. 
```json
{
  "AllowMethods": [
    {
      "path": "/",
      "methods": [
        "POST",
        "GET"
      ]
    },
    {
      "path": "/coucou",
      "methods": [
        "GET"
      ]
    }
  ]
}
```

####How it works ?

This module setup a Context variable: ErrorOccured with a std::pair that represente the status code 405 and an error message
To make this module working properly, you need to use a fallback module that will act at the very end of the post process pipeline and will setup the response if ErrorOccured variable is setup in the Context.
