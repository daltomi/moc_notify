/*
	Copyright (C) 2017,2019 by daltomi <daltomi@disroot.org>

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

/*
 * Note: 'errno' reset only once because all failed syscall terminate.
 */

#include "monitor.h"

#define MONITOR_EVENTS (IN_MOVE_SELF | IN_DELETE_SELF | IN_ATTRIB)

#define MONITOR_BUF_LEN ((sizeof(struct inotify_event) + NAME_MAX + 1))

static volatile int monitor_id = -1;

extern volatile sig_atomic_t signalcaught;


static void monitor_error_and_exit()
{
	raise(SIGINT);
}



static void *monitor_read_events_thread(void *UNUSED)
{
	assert(monitor_id != -1);

	char buffer[MONITOR_BUF_LEN] __attribute__((aligned(8))) = {0};

	errno = 0;

	/* block */
	ssize_t const nread = read(monitor_id, buffer, MONITOR_BUF_LEN);

	check_if(nread != -1, return (void*)0,
		"Failed to read inotify event: %s.", strerror(errno));

	if (signalcaught == 0)
		log_error("The file was deleted or moved: %s", SERVER_FIFO);

	monitor_error_and_exit();

	return (void*)0;
}



static void monitor_init_read_events()
{
	pthread_t monitor_thread_id;

	errno = 0;

	int ret = pthread_create(&monitor_thread_id, NULL,
				 &monitor_read_events_thread, NULL);

	check_if(ret == 0, monitor_error_and_exit(),
		"Failed pthread_create: %s.", strerror(errno));

	ret = pthread_detach(monitor_thread_id);

	check_if(ret == 0, monitor_error_and_exit(),
		"Failed pthread_detach: %s.", strerror(errno));
}



void monitor_init()
{
	errno = 0;

	if (monitor_id == -1) {
		monitor_id = inotify_init();

		check_if(monitor_id != -1, monitor_error_and_exit(),
			"Failed inotify_init: %s.", strerror(errno));
	}

	int const watch_id = inotify_add_watch(monitor_id, SERVER_FIFO, MONITOR_EVENTS);

	check_if(watch_id != -1, monitor_error_and_exit(),
		"Failed inotify_add_watch: %s", strerror(errno));

	monitor_init_read_events();
}
