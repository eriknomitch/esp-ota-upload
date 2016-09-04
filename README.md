# esp-ota-upload

## Dependencies

* [PlatformIO](http://platformio.org/)
* Zsh (TEMPORARY)

## Installation

### Configuration

Clone the repository and create a file at `src/env.h` with the following contents:

```C
#define WIFI_ESSID    "YourWiFiEssid"
#define WIFI_PASSWORD "YourWiFiPassword"
```

Plug in your ESP8266 and find the `/dev/tty.*` port by listing `ls -1 /dev/tty.*`.

Create a file at `.env` with the following contents:

```Bash
ESP_UPLOAD_PORT=<IP or .local domain of your ESP>
ESP_LOCAL_TTY_DEV=</dev/tty.* port of your ESP>
```

## Usage

For the first run, plug in your ESP8266 and execute `./upload`.

After that, you'll be able to do OTA uploads. Edit `src/Main.ino` as you'd like for your specific application

