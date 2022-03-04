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
int port = cfg["https"]["port"].AsInt(); // port to listen -> by default the port is 443
std::string certificate_path = cfg["https"]["certificate_path"].AsString(); // path to certificat -> by default the path is ./Certificat/cert.pem
std::string private_key_file = cfg["https"]["private_key_file"].AsString(); // path to private key -> by default the path is ./Certificat/key.pem
bool activated = cfg["https"]["private_key_file"].AsBool(); // use this module or not -> by default it's false 
```

## Context

This module sets:
- `client.socket.address`
- `client.socket.port` 
