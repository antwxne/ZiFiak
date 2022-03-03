# BasicNetwork

## Description

This module handle HTTP connection and read sockets.

It stores all connections and close the connection when needed.

It handles the `Keep-Alive` option.

It will fill up `ziapi::http::IRequestOutputQueue` with incoming request.

## Configuration

**You must provide the following configuration in the server configuration file.**

```cpp
const ziapi::config::Node &cfg; // server configuration
int port = cfg["http"]["port"].AsInt(); // port to listen
```

## Context

This module sets:
- `client.socket.address`
- `client.socket.port` 
