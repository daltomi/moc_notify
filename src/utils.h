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

#include <stdarg.h>
#include <syslog.h>
#include <assert.h>

static void va_log(int option, char const*const format, ...) {
	assert(format != NULL);

	va_list vl;
	va_start(vl, format);
	vsyslog(option, format, vl);
	va_end(vl);
}

#define log_warning(...)				\
	va_log(LOG_WARNING, "Warning: " __VA_ARGS__)

#define log_error(...)					\
	va_log(LOG_ERR, "Error: " __VA_ARGS__)

#define log_notice(...)					\
	va_log(LOG_NOTICE,"Notice: "  __VA_ARGS__)


#define check_if(value, action, ...) {			\
	if (! (value)) {				\
		log_error(__VA_ARGS__);			\
		action;					\
	}						\
}
