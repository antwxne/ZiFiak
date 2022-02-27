# HTTPSNetwork

## Description

This module handle HTTPS connection and read sockets.

It stores all connections and close the connection when needed.

It handles the `Keep-Alive` option.

It will fill up `ziapi::http::IRequestOutputQueue` with incoming request.

## Configuration

**You must provide the following configuration in the server configuration file.**

```cpp
const ziapi::config::Node &cfg; // server configuration
int port = cfg["https"]["port"].AsInt(); // port to listen
std::string certificate_path = cfg["https"]["certificate_path"].AsString(); // path to certificat
    std::string private_key_file = cfg["https"]["private_key_file"].AsString(); // path to private key
```

## Context

This module sets:
- `client.socket.address`
- `client.socket.port` 
