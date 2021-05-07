#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct NumArray {
  int size;
  double values[1]; /* variable part */
} NumArray;
static int newarray(lua_State *L) {
  int n = luaL_checkint(L, 1);
  size_t nbytes = sizeof(NumArray) + (n - 1) * sizeof(double);
  NumArray *a = (NumArray *)lua_newuserdata(L, nbytes);
  a->size = n;
  return 1; /* new userdatum is already on the stack */
}

static int setarray(lua_State *L) {
  NumArray *a = (NumArray *)lua_touserdata(L, 1);
  int index = luaL_checkint(L, 2);
  double value = luaL_checknumber(L, 3);

  luaL_argcheck(L, a != NULL, 1, "`array' expected");

  luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");

  a->values[index - 1] = value;
  return 0;
}

static int getarray(lua_State *L) {
  NumArray *a = (NumArray *)lua_touserdata(L, 1);
  int index = luaL_checkint(L, 2);

  luaL_argcheck(L, a != NULL, 1, "`array' expected");

  luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");

  lua_pushnumber(L, a->values[index - 1]);
  return 1;
}
static int getsize(lua_State *L) {
  NumArray *a = (NumArray *)lua_touserdata(L, 1);
  luaL_argcheck(L, a != NULL, 1, "`array' expected");
  lua_pushnumber(L, a->size);
  return 1;
}

// second :Our current implementation has a major security hole
static int newarray_s(lua_State *L) {
  int n = luaL_checkint(L, 1);
  size_t nbytes = sizeof(NumArray) + (n - 1) * sizeof(double);
  NumArray *a = (NumArray *)lua_newuserdata(L, nbytes);

  luaL_getmetatable(L, "LuaBook.array");
  lua_setmetatable(L, -2);

  a->size = n;
  return 1; /* new userdatum is already on the stack */
}

static NumArray *checkarray(lua_State *L) {
  void *ud = luaL_checkudata(L, 1, "LuaBook.array");
  luaL_argcheck(L, ud != NULL, 1, "`array' expected");
  return (NumArray *)ud;
}

static int getsize_s(lua_State *L) {
  NumArray *a = checkarray(L);
  lua_pushnumber(L, a->size);
  return 1;
}

static double *getelem(lua_State *L) {
  NumArray *a = checkarray(L);
  int index = luaL_checkint(L, 2);

  luaL_argcheck(L, 1 <= index && index <= a->size, 2, "index out of range");

  /* return element address */
  return &a->values[index - 1];
}

static int setarray_s(lua_State *L) {
  double newvalue = luaL_checknumber(L, 3);
  *getelem(L) = newvalue;
  return 0;
}

static int getarray_s(lua_State *L) {
  lua_pushnumber(L, *getelem(L));
  return 1;
}

static const struct luaL_Reg arraylib[] = {
    {"new", newarray},     {"set", setarray},     {"get", getarray},
    {"size", getsize},     {"new_s", newarray_s}, {"set_s", setarray_s},
    {"get_s", getarray_s}, {"size_s", getsize_s}, {NULL, NULL}};

extern int luaopen_array(lua_State *L) {
  luaL_newmetatable(L, "LuaBook.array");
#ifdef LUA_LOW_VER
  luaL_openlib(L, "array", arraylib, 0);
#else
  luaL_newlib(L, arraylib);
#endif
  return 1;
}
