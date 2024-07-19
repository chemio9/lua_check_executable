#define __USE_POSIX2
#define _GNU_SOURCE /* See feature_test_macros(7) */
#include "config.h"
#include <alloca.h>
#include <luajit-2.1/lauxlib.h>
#include <luajit-2.1/lua.h>
#include <luajit-2.1/luajit.h>
#include <luajit-2.1/lualib.h>
#include <paths.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool search_check(const char *file);
static int check(lua_State *L) {
  const char *path = luaL_checkstring(L, 1);

  bool result = search_check(path);
  lua_pushboolean(L, result ? 1 : 0);
  return 1;
}

static const luaL_Reg mylib[] = {
    {"check", check}, {NULL, NULL} /* sentinel */
};

int luaopen_mylib(lua_State *L) {
  luaL_openlib(L, "mylib", mylib, 0);
  return 1;
}

bool search_check(const char *file) {
  size_t pathlen;
  size_t alloclen = 0;
  char *path = getenv("PATH");
  if (path == NULL) {
    pathlen = confstr(_CS_PATH, (char *)NULL, 0);
    alloclen = pathlen + 1;
  } else
    pathlen = strlen(path);

  size_t len = strlen(file) + 1;
  alloclen += pathlen + len + 1;

  char *name;
  name = (char *)alloca(alloclen);

  if (path == NULL) {
    /* There is no `PATH' in the environment.
       The default search path is the current directory
       followed by the path `confstr' returns for `_CS_PATH'.  */
    path = name + pathlen + len + 1;
    path[0] = ':';
    (void)confstr(_CS_PATH, path + 1, pathlen);
  }

  /* Copy the file name at the top.  */
  name = (char *)memcpy(name + pathlen + 1, file, len);
  /* And add the slash.  */
  *--name = '/';

  void *script_argv_malloc = NULL;
  char *p = path;
  do {
    char *startp;

    path = p;
    p = strchrnul(path, ':');

    if (p == path)
      /* Two adjacent colons, or a colon at the beginning or the end
         of `PATH' means to search the current directory.  */
      startp = name + 1;
    else
      startp = (char *)memcpy(name - (p - path), path, p - path);

    /* Try to check this name. */

    if (access(startp, F_OK | X_OK) == 0) {
      return true;
      /* You can execute this file. */
    }
  } while (*p++ != '\0');

  free(script_argv_malloc);
  return false;
}
