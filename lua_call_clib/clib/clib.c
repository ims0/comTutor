#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/* 所有注册给Lua的C函数具有
 * "typedef int (*lua_CFunction) (lua_State *L);"的原型。
 */
static int l_sin(lua_State *L)
{ 
	// 如果给定虚拟栈中索引处的元素可以转换为数字，则返回转换后的数字，否则报错。
	double d = luaL_checknumber(L, 1);
	lua_pushnumber(L, sin(d)); /* push result */

	/* 这里可以看出，C可以返回给Lua多个结果，
	 * 通过多次调用lua_push*()，之后return返回结果的数量。
	 */
	return 1; /* number of results */
}

int func_return_table(lua_State *L)
{
	lua_newtable(L);//创建一个表格，放在栈顶
	lua_pushstring(L, "mydata");//压入key
	lua_pushnumber(L,66);//压入value
	lua_settable(L,-3);//弹出key,value，并设置到table里面去

	lua_pushstring(L, "subdata");//压入key
	lua_newtable(L);//压入value,也是一个table
	lua_pushstring(L, "mydata");//压入subtable的key
	lua_pushnumber(L,53);//value
	lua_settable(L,-3);//弹出key,value,并设置到subtable

	lua_settable(L,-3);//这时候父table的位置还是-3,弹出key,value(subtable),并设置到table里去
	lua_pushstring(L, "mydata2");//同上
	lua_pushnumber(L,77);
	lua_settable(L,-3);
	return 1;//堆栈里现在就一个table.其他都被弹掉了。
}
int table_item(lua_State *L, int num)
{
	lua_newtable(L);
	for( int i=1 ; i < num; i++ )
	{
		lua_pushnumber(L, i);
		lua_pushnumber(L,10*i);
		lua_settable(L,-3);
	}
	return 0;
}
int numindex_subtable(lua_State *L)
{
    /* get the input. */  
    //int num = lua_tonumber(L, 1);
	// 如果给定虚拟栈中索引处的元素可以转换为数字，则返回转换后的数字，否则报错。
    int num = (int)luaL_checknumber(L, 1);
    int subNum = (int)luaL_checknumber(L, 2);
	//lua_gettop函数返回栈顶的索引值。因为在Lua中栈是从1开始编号的，因此该函数获得的值就是参数的个数
	int argNum =lua_gettop(L);
	printf("major:%d, sub:%d, argN:%d\n", num, subNum, argNum );

	lua_newtable(L);
	for( int i=1 ; i<num ; i++ )
	{
		lua_pushnumber(L, i);
		table_item(L, subNum);
		lua_settable(L,-3);
	}
	return 1;
}


static int get_tb(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	size_t tb_len = luaL_len(L, 1);
	int pos = luaL_checknumber(L, 2);
	printf("table item num:%zu,arg2: pos = %d\n",tb_len, pos);

	printf("the number of elements in the stack = %d\n",  lua_gettop(L));
	if (pos >= 1 && pos <= tb_len)
	{
		//lua_geti(L, 1, pos);
		//lua_rawgeti(L, 1, pos);
		lua_pushnumber(L, pos);
		lua_pushnumber(L, pos+1);
		lua_gettable(L, 1);
		//The lua_gettop function returns the number of elements in the stack
		int count = lua_gettop(L);
		printf("the number of elements in the stack = %d\n", count);
	}
	else
	{
		lua_pushnil(L);
	}
	return 1;  //这是是返回个数
}
/* 需要一个"luaL_Reg"类型的结构体，其中每一个元素对应一个提供给Lua的函数。
 * 每一个元素中包含此函数在Lua中的名字，以及该函数在C库中的函数指针。
 * 最后一个元素为“哨兵元素”（两个"NULL"），用于告诉Lua没有其他的函数需要注册。
 */
static const struct luaL_Reg mylib[] = {
	{"mysin", l_sin},
	{"func_return_table", func_return_table},
	{"numindex_subtable", numindex_subtable},
	{"get_tb", get_tb},
	{NULL, NULL}
};
/* 此函数为C库中的“特殊函数”。
 * 通过调用它注册所有C库中的函数，并将它们存储在适当的位置。
 * 此函数的命名规则应遵循：
 * 1、使用"luaopen_"作为前缀。
 * 2、前缀之后的名字将作为"require"的参数。
 */
extern int luaopen_mylib(lua_State* L)
{
	/* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
	 * 创建一个新的"table"，并将"l"中所列出的函数注册为"table"的域。
	 */

#ifdef  LUA_LOW_VER
	luaL_openlib(L, "mylib", mylib, 0);
#else
	luaL_newlib(L, mylib);
#endif

	return 1;
}
