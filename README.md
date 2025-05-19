# Trill Node

A C++ Wrapper for detecting touch for many, many Trill conductive touch sensors from a Node.js app.

Touches are transformed into a lo-fi dreamy melody using PureData.

A sound art installation for Future Frontend 2025

![front side of art](images/frontside.png)

![back side of art](images/backside.png)

<video src='images/plushy-music.mov' />

## Installation

```bash
npm i
npm run build
```




## RUNNING

### Setup pure data patch to run on boot

- cd `/etc/xdg/labwc`
- `sudo nano autostart`
- Add line `pd -nogui /home/pi/projects/trill-node/'Ambient test.pd' &`
- `sudo reboot now`

### Setup pm2:

```shell
  npm i -g pm2
  pm2 startup
  sudo env PATH=$PATH:/home/pi/.nvm/versions/node/v22.14.0/bin /home/pi/.nvm/versions/node/v22.14.0/lib/node_modules/pm2/bin/pm2 startup systemd -u pi --hp /home
  pm2 start npm --name "TRILL" -- start
  pm2 save
  pm2 logs
```

To restart: `pm2 restart TRILL`
To stop: `pm2 stop TRILL`
