# ofxTwinkly

openFrameworks addon for controlling Twinkly LED light strings over Wi-Fi, using the undocumented local (xled) HTTP API. Includes network discovery of strings on the LAN.

## Examples

- `discoveryExample`: finds Twinkly devices on the network.
- `example`: connects to a string and sends it frames, with a GUI.

## Dependencies

The HTTP side uses Christopher Baker's stack: `ofxHTTP`, `ofxIO`, `ofxMediaType`, `ofxNetworkUtils`, `ofxSSLManager` and `ofxPoco` (or [ofxPocoHeaders](https://github.com/fred-dev/ofxPocoHeaders)), plus `ofxNetwork` and `ofxGui`.

Written in 2019 against early Twinkly firmware; newer firmware may have changed the API.
