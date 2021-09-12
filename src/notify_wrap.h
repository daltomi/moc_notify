/*
	Copyright © 2015,2021 by Daniel T. Borelli <danieltborelli@gmail.com>

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

#include <sys/syslog.h>
#include <libnotify/notify.h>

#include "utils.h"


void notify_wrap_show(gchar const *summary,
		    gchar const *body,
		    gchar const *icon);


int notify_wrap_init();

void notify_wrap_end();

gboolean notify_wrap_is_closed();
