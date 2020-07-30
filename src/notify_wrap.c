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
#include "notify_wrap.h"

static NotifyNotification *notify_obj = NULL;


void notify_wrap_show(gchar const *summary,
		    gchar const *body,
		    gchar const *icon)
{
	check_if(0 != notify_wrap_init(), return, "notify_init failed.");

	if (notify_obj == NULL) {
		notify_obj = notify_notification_new(NULL, NULL, NULL);

		check_if(NULL != notify_obj, goto error_notification_new,
			"notify_notification_new failed. Possibly your notify"
			" daemon is not running.");
	}

	GVariant *string = g_variant_new_string(icon);

	notify_notification_set_hint(notify_obj, "image_path", string);
	notify_notification_update(notify_obj, summary, body, icon);

	GError *err = NULL;

	gboolean is_ok = notify_notification_show(notify_obj, &err);

	check_if(FALSE != is_ok, goto error_notification_show,
		"notify_notification_show. It is possible that your notification "
		"daemon is not running or is suspended.");
	return;

error_notification_show:
	g_error_free(err);

error_notification_new:
	notify_wrap_end();

}



int notify_wrap_init()
{
	if (!(notify_is_initted() || notify_init("moc_notify")))
		return 0;

	return 1;
}



void notify_wrap_end()
{
	g_object_unref(G_OBJECT(notify_obj));
	notify_obj = NULL;
	notify_uninit();
}



gboolean notify_wrap_is_closed()
{
	g_assert(notify_obj != NULL);
	return -1 != notify_notification_get_closed_reason(notify_obj);
}
