/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <err.h>
#include <string.h>

#include "util.h"

static xcb_connection_t *conn;

static void
usage(char *name) {
	fprintf(stderr, "usage: %s name wid\n", name);
	exit(1);
}

xcb_atom_t
get_atom() {
	xcb_intern_atom_cookie_t cookie;
	xcb_intern_atom_reply_t *reply;
	cookie = xcb_intern_atom(conn, 0, strlen("_WM_GROUP"), "_WM_GROUP");
	if ((reply = xcb_intern_atom_reply(conn, cookie, NULL))) {
		return reply->atom;
		free(reply);
	}
	return 0;
}

void 
set_group(char *grp, xcb_window_t wid) {
		xcb_atom_t atom = get_atom();
		xcb_void_cookie_t cookie;
		cookie = xcb_change_property(conn, XCB_PROP_MODE_REPLACE, wid, atom, XCB_ATOM_STRING, 8, strlen(grp), grp);
		xcb_generic_error_t *error;
		if ((error = xcb_request_check(conn, cookie))) {
			fprintf(stderr, "unable to set group\n");
			free(error);
		}
}

int
main(int argc, char *argv[]) {
	if (argc != 3) {
		usage(argv[0]);
	}
	init_xcb(&conn);
	set_group(argv[1], strtoul(argv[2], NULL, 16 ));
	kill_xcb(&conn);
  	return 0;
}
