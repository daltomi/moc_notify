/*
	Copyright (C) 2015, 2019 by daltomi <daltomi@disroot.org>

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
#include "fifo.h"


void close_fifo()
{
	close(fifo.fd);
	unlink(SERVER_FIFO);
}


static void make_fifo()
{
	umask(0);

	errno = 0;

	check_if(mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR) != -1, exit(EXIT_FAILURE),
		"Failed mkfifo %s, error=%s", SERVER_FIFO, strerror(errno));

	monitor_init();
}



void open_fifo()
{
	make_fifo();

	errno = 0;

	fifo.fd = open(SERVER_FIFO, O_RDONLY);

	check_if(fifo.fd != -1, goto error_open,
		"Failed open fifo: %s, error=%s.", SERVER_FIFO, strerror(errno));

	errno = 0;

	// dummy, discard EOF
	check_if(open(SERVER_FIFO, O_WRONLY) != -1, goto error_open,
		"Failed open dummy, error=%s.", strerror(errno));

	return;

error_open:
	fifo.close();
	exit(EXIT_FAILURE);
}



void read_fifo()
{
	memset(fifo.data, 0, LEN_DATA);

	errno = 0;

	fifo.len = read(fifo.fd, fifo.data, LEN_DATA - 1); // reserve '\0'

	if (fifo.len <= 0 || fifo.len > SSIZE_MAX) {
	    log_warning("Failed reading request; discarding,"
		      " error=%s", strerror(errno));

		fifo.len = 0;
	}

	fifo.data[fifo.len] = '\0';
}
