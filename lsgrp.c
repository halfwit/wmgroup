/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <err.h>
#include <string.h>

#include "util.h"

static xcb_connection_t *conn;
static xcb_screen_t *scrn;
static xcb_atom_t atom;

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <group>\n", name);
	exit(1);
}

xcb_atom_t
atom_get(char *grp)
{
  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, 1, strlen(grp), grp);
  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
  xcb_atom_t atom = reply->atom;
  free(reply);
  return atom;
}

  void
group_list(char *grp)
{
  xcb_window_t *wc;
  int wn  = get_windows(conn, scrn->root, &wc); 
  for (int i = 0; i < wn; i++) {
    xcb_get_property_cookie_t prop_c = xcb_get_property(conn, 0, wc[i], atom, XCB_ATOM_STRING, 0L, 32L);
    xcb_get_property_reply_t *prop_r = xcb_get_property_reply(conn, prop_c, NULL);
    if(strcmp(grp, (char *) xcb_get_property_value(prop_r)) == 0)
      printf("0x%08x\n", wc[i]);
    free(prop_r);
  }
}

int
main(int argc, char **argv)
{ 
  init_xcb(&conn);
  get_screen(conn, &scrn);

  if (argc != 2)
    usage(argv[0]);
  
  atom = atom_get("GROUP_NAME");

  if (atom == XCB_ATOM_NONE) {
    return 0;
  }

  group_list(argv[1]);
  
  kill_xcb(&conn);

  return 0;
}
