# moc_notify
*Notifier for 'Music On Console' using libnotify.*


<img src="https://git.disroot.org/daltomi/moc_notify/blob/master/screenshots/01.png"/>


<img src="https://git.disroot.org/daltomi/moc_notify/blob/master/screenshots/02.png"/>



### Platform
* GNU/Linux


### Dependencies
* Libraries : **libnotify**
* Build:  **gcc**, **make**, **pkg-config**
* External:
	* Our [onsongchange.sh](https://git.disroot.org/daltomi/moc_notify/blob/master/scripts/onsongchange.sh) script.
	* Some notifier running: **notify-osd**, **xfce4-notifyd**, **dunst**, etc.

### Build
```bash
make
-- or --
make debug
```


### Run
```bash
moc_notify [option] &

 -v  Show the version.
 -f  Show the name of the fifo file, internaly.
 -h  Show help.
```

*Note: run in background (&)*


### How to use with MOC

1 - Copy the script [onsongchange.sh](https://git.disroot.org/daltomi/moc_notify/blob/master/scripts/onsongchange.sh) to your MOC directory:

```bahsh
cp scripts/onsongchange.sh ~/.moc/
```

2 - Edit your MOC configuration file: *~/.moc/config*
```
# Enable the "OnSongChange" event.
 OnSongChange="/home/YOU/.moc/onsongchange.sh %a %t %r %f"
```

3 - Restart MOC.

Please see [onsongchange.sh](https://git.disroot.org/daltomi/moc_notify/blob/master/scripts/onsongchange.sh)
script to see how to get the cover art.

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

### inotify
`moc_notify` see if the `FIFO` file is deleted or moved, for this it uses `inotify`
from version `2.x`.


### Systemd journal
To follow the program messages.
```bash
journalctl -f -t moc_notify
```

---
