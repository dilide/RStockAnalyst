#include "StdAfx.h"
#include "RLuaEx.h"

void RLuaEx::LuaPushArray( lua_State* _L,const char* _t,QVector<float>& _v )
{
	_v.clear();
	luaL_dostring(_L,"return Array.create()");
	if(lua_type(_L,-1)!=LUA_TTABLE)
		return;

	int table_index = lua_gettop(_L);

	for(int i = 0;i<_v.size();++i)
	{
		lua_pushnumber(_L,_v[i]);
		lua_rawseti(_L,table_index,i+1);
	}
	lua_setglobal(_L,_t);
}

void RLuaEx::LuaPopArray( lua_State* _L,const char* _t,QVector<float>& _v )
{
	_v.clear();
	lua_getglobal(_L,_t);
	if(lua_type(_L,-1)!=LUA_TTABLE)
	{
		lua_pop(_L,1);
		return;
	}

	int table_index = lua_gettop(_L);

	lua_pushnil(_L);  /* ��һ�� key */

	while (lua_next(_L, table_index) != 0) {
		_v.push_back(lua_tonumber(_L, -1));
		/* ��һ�� 'key' �������� -2 ���� �� 'value' �������� -1 ���� */
		/* �Ƴ� 'value' ������ 'key' ����һ�ε��� */
		lua_pop(_L, 1);
	}
	lua_pop(_L,1);
}


void RLuaEx::LuaRetArray( lua_State* _L,QVector<float>& _v )
{
	_v.clear();
	if(lua_type(_L,-1)!=LUA_TTABLE)
		return;
	int table_index = lua_gettop(_L);

	lua_pushnil(_L);  /* ��һ�� key */
	while (lua_next(_L, table_index) != 0) {
		_v.push_back(lua_tonumber(_L, -1));
		/* ��һ�� 'key' �������� -2 ���� �� 'value' �������� -1 ���� */
		/* �Ƴ� 'value' ������ 'key' ����һ�ε��� */
		lua_pop(_L, 1);
	}
	lua_pop(_L,1);
}
