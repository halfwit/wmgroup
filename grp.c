/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <err.h>
#include <string.h>

#include "util.h"

static xcb_connection_t *conn;

static void
usage(char *name) {
	fprintf(stderr, "usage: %s name wid\n", name);
	exit(1);
}

static void 
set_group(char *grp, xcb_window_t wid) {
	xcb_intern_atom_cookie_t cookie;
	xcb_intern_atom_reply_t *reply;
	cookie = xcb_intern_atom(conn, 0, 10, "GROUP_NAME");
	reply = xcb_intern_atom_reply(conn, cookie, NULL);
	xcb_change_property(conn, XCB_PROP_MODE_REPLACE, wid, reply->atom, XCB_ATOM_STRING, 8, strlen(grp), grp);
	free(reply);
}

int
main(int argc, char *argv[]) {
	if (argc != 3) {
		usage(argv[0]);
	}
	init_xcb(&conn);
	set_group(argv[1], strtoul(argv[2], NULL, 16 ));
	xcb_flush(conn);
	kill_xcb(&conn);
  	return 0;
}
