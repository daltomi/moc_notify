ifeq ("$(shell which pkg-config  2> /dev/null)","")
$(error 'pkg-config' NOT FOUND)
endif

ifeq ("$(shell pkg-config --libs libnotify  2> /dev/null)","")
$(error 'libnotify' NOT FOUND)
endif

ifeq ("$(shell which install 2> /dev/null)","")
$(error 'install (coreutils)' NOT FOUND)
endif

ifeq ("$(shell pkg-config --libs libinotify  2> /dev/null)","")
$(error 'libnotify' NOT FOUND)
endif

APP := moc_notify

APP_VER := "2.4"

PREFIX ?= /usr

CC ?= gcc

CLIBS := $(shell pkg-config --libs libnotify libinotify) -lpthread

CFLAGS := -Wall -std=c99 -D_GNU_SOURCE $(shell pkg-config --cflags libnotify libinotify)

OBJ := $(subst .c,.o,$(wildcard src/*.c))

default: release

version:
	-@sed 's/@APP_VER@/$(APP_VER)/g' src/version.h.in > src/version.h

release: CFLAGS+= -O3 -DNDEBUG
release: CLIBS+= -Wl,-s
release: version $(APP)

debug: CFLAGS+= -O0 -ggdb -DDEBUG -Wimplicit-fallthrough
debug: version $(APP)

small: CFLAGS+= -Os -DNDEBUG
small: CLIBS+= -Wl,-s
small: version $(APP)

$(APP): $(OBJ)
	$(CC) $(OBJ) $(CLIBS) -o $(APP)

.o:
	$(CC) -c $< -o src/

.PHONY: release

install:
	install -Dt $(PREFIX)/bin/ -m0755 $(APP)
	install -Dt $(PREFIX)/share/moc_notify -m0755 scripts/onsongchange.sh

install-freebsd:
	install -m0755  $(APP) $(PREFIX)/bin/
	install -m0755 -d $(PREFIX)/share/moc_notify
	install -m0755 scripts/onsongchange.sh $(PREFIX)/share/moc_notify

clean:
	rm  src/*.o src/version.h $(APP)
