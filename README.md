# moc_notify
*Notifier for 'Music On Console' using libnotify.*


<img src="https://github.com/daltomi/moc_notify/raw/master/screenshots/01.png"/>

<img src="https://github.com/daltomi/moc_notify/raw/master/screenshots/02.png"/>



### Platform

* GNU/Linux
* FreeBSD 13.x


### Package - ArchLinux - AUR
[moc_notify-git](https://aur.archlinux.org/packages/moc_notify-git/)

  * GPG key
    ```bash
    gpg --keyserver gozer.rediris.es --recv-keys EA8BDDF776B54DD1
    ```

### Dependencies
* Libraries : **libnotify** **libinotify**

* Build:
    - GNU/Linux: **gcc**, **make**, **pkg-config**
    - FreeBSD  : **clang**, **gmake** **pkg-config**

* External:
    * Our [onsongchange.sh](https://github.com/daltomi/moc_notify/blob/master/scripts/onsongchange.sh) script.
    * Some notifier running: **notify-osd**, **xfce4-notifyd**, **dunst**, etc.


### Build GNU/Linux
```bash
make
-- or --
make debug
```

### Build FreeBSD
```bash
CC=clang gmake
-- or --
CC=clang gmake debug
```

### Install GNU/Linux

```bash
sudo make install (default PREFIX=/usr)
-- or --
sudo PREFIX=/usr/local make install
```

### Install FreeBSD
```bash
sudo gmake install-freebsd (default PREFIX=/usr)
-- or --
sudo PREFIX=/usr/local gmake install-freebsd
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
