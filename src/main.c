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

#include <signal.h>

#include "version.h"
#include "fifo.h"
#include "notify_wrap.h"


volatile sig_atomic_t signalcaught = 0;


struct fifo_file fifo = {
	.close = close_fifo,
	.open  = open_fifo,
	.read  = read_fifo
};



static void signal_h(int sig)
{
	signalcaught = sig;

	fifo.close();
	notify_wrap_end();
	exit(EXIT_SUCCESS);
}


static void add_new_signal(int sig, void (*function)(int))
{
	struct sigaction action = {.sa_handler = function, .sa_flags = 0};

	sigemptyset(&action.sa_mask);

	if (sigaction(sig, &action, 0) == -1) {
		log_error("Fatal sigaction.");
		exit(EXIT_FAILURE);
	}
}


static void print_help()
{
	char const* const help = "\n\nmoc_notify %s\n\n"
	"Usage: \n"
	"   -v   - Show the version.\n"
	"   -f   - Show the name of the fifo file, internaly.\n"
	"   -h   - Show help.\n\n"
	"How to kill cleanly:\n\n"
	"   killall moc_notify\n"
	"   -or-\n"
	"   kill `pidof moc_notify`\n\n";

	printf(help, VERSION);
}



_Noreturn void options(int argc, char* argv [])
{
	int opt = 0;

	while ((opt = getopt(argc, argv, "vfh")) != -1) {
		switch (opt) {
		case 'v':
			printf("moc_notify %s\n", VERSION);
			break;
		case 'f':
			printf("%s", SERVER_FIFO);
			break;
		case 'h':
			print_help();
			break;
		}
	}

	exit(EXIT_SUCCESS);
}



static char const *const getenv_delim()
{
	char const *const delim = getenv("MOC_NOTIFY_DELIM");

	if (delim && strlen(delim) == 1) {
		log_notice("Using delimiter: %c", *delim);
		return delim;
	}

	return "@"; // By default is '@'
}



static void main_loop()
{
	while (1) {

		fifo.read();

		if (fifo.len == 0)
			continue;

		char const *const delim = getenv_delim();
		char const *title = strtok(fifo.data, delim);
		char const *body  = strtok(NULL, delim);
		char const *icon  = strtok(NULL, delim);

		if (title && body && icon) {
			notify_wrap_show(title, body, icon);
		} else {
			char const *const ctitle = title ? "(ok)" : "(nil)" ;
			char const *const cbody  = body  ? "(ok)" : "(nil)" ;
			char const *const cicon  = icon  ? "(ok)" : "(nil)" ;

			log_warning("The format from FIFO not match: "
				    "title=%s, body=%s, icon=%s", ctitle, cbody, cicon);
		}
	}
}



int main(int argc, char* argv[])
{
	if (argc == 2)
		options(argc, argv);

	add_new_signal(SIGTERM, signal_h);
	add_new_signal(SIGHUP, signal_h);
	add_new_signal(SIGINT, signal_h);
	fifo.open();
	main_loop();
	return EXIT_SUCCESS;
}
