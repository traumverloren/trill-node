# Trill Node

A Node Wrapper for many, many Trill conductive touch sensors

A sound art installation for Future Frontend 2025

## Installation

```bash
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
