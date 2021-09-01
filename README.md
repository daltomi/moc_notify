# moc_notify
*Notifier for 'Music On Console' using libnotify.*


<img src="https://github.com/daltomi/moc_notify/raw/master/screenshots/01.png"/>

<img src="https://github.com/daltomi/moc_notify/raw/master/screenshots/02.png"/>



### Platform
* GNU/Linux


### Dependencies
* Libraries : **libnotify**
* Build:  **gcc**, **make**, **pkg-config**
* External:
	* Our [onsongchange.sh](https://github.com/daltomi/moc_notify/blob/master/scripts/onsongchange.sh) script.
	* Some notifier running: **notify-osd**, **xfce4-notifyd**, **dunst**, etc.

### Build
```bash
make
-- or --
make debug
```

### Install

```bash
make install (default PREFIX=/usr)
-- or --
PREFIX=/usr/local make install
```
_Note: If you don't install change [this line](https://github.com/daltomi/moc_notify/blob/0b3e2595811322f720365743e14d0119851510a8/scripts/onsongchange.sh#L23) of the `onsongchange.sh` since the script needs `moc_notify` to know the name of the FIFO. To get this name, manually run  `moc_notify -f`_

### Run
```bash
moc_notify [option] &

 -v  Show the version.
 -f  Show the name of the fifo file, internaly.
 -h  Show help.
```

*Note: run in background (&)*


### How to use with MOC

1 - Copy the script [onsongchange.sh](https://github.com/daltomi/moc_notify/blob/master/scripts/onsongchange.sh) to your MOC directory:

```bash
cp scripts/onsongchange.sh ~/.moc

chmod +x ~/.moc/onsongchange.sh
```
_Note: If you installed with `make install` you will find the script in `$PREFIX/share/moc_notify`_


2 - Edit your MOC configuration file: *~/.moc/config*
```
# Enable the "OnSongChange" event.
 OnSongChange="/home/YOU/.moc/onsongchange.sh %a %t %r %f"
```

3 - Restart MOC.

Please see [onsongchange.sh](https://github.com/daltomi/moc_notify/blob/master/scripts/onsongchange.sh)
script to see how to get the cover art.

---

### Known issues - moc

* The script file does not have the attribute of execution (see `man chmod`)

```bash
user.err: mocp: Error when running OnSongChange command ~/.moc/onsongchange.sh : permission denied
```

* Wrong shell path or name, see `#!/usr/bin/bas`

```bash
user.err: mocp: Error when running OnSongChange command ~/.moc/onsongchange.sh : The file or directory does not exist
```

---

### Extra information

### FIFO format
```bash
printf "title @ body @ icon" > $FIFO

printf "title @ body \r newline @ icon" > $FIFO
```

*"I do not want that (@) delimiter in my script. How I can change it?"*

```bash
MOC_NOTIFY_DELIM="?" /usr/bin/moc_notify &
```

### How to test (without using MOC)

Make sure `moc_notify` is running, then:
```
echo "Artist@Title@audio-x-generic" > $(moc_notify -f)
```
and test the script:
```bash
~/.moc/onsongchange.sh "Artist" "Title" "Album"
```

### inotify
`moc_notify` see if the `FIFO` file is deleted or moved, for this it uses `inotify`
from version `2.x`.

## Syslog

- Journalctl - systemd
```bash
journalctl -f -t moc_notify
```

- Socklog - runit
```bash
sudo tail -f /var/log/socklog/everything/current
```
---
