/*
	Copyright (C) 2015, 2019 by Daniel T. Borelli <danieltborelli@gmail.com>

	This file is part of moc_notify.

	moc_notify is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	moc_notify is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with moc_notify.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stddef.h>
#include <limits.h>

#include "monitor.h"
#include "utils.h"

#define SERVER_FIFO "/tmp/moc_notify"

#define LEN_DATA 1024 * 4 // 4KiB


struct fifo_file {
	int fd;
	ssize_t len;
	char data[LEN_DATA];

	void (*close)(void);
	void (*open)(void);
	void (*read)(void);
};

extern struct fifo_file fifo;

void close_fifo();

void open_fifo();

void read_fifo();
