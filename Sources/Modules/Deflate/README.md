##Module: Deflate

####Description:

- This module is use to compress response body just before sending it to client

####Config:

- To implement this module correctly you need to add a variable in you server config file named "Defalte" wich contain one variable, "activated" to able/disable the module. By default Deflate will be enable.
```json
{
    "Deflate": {
        "activated": true
    }
}
```

####How it works ?

- In PostProcess, if the module is activated, this module use zlib library to compresse the response body with deflate algorithm, it will update the body content and the header "Content-Length" variable with new body size. 
