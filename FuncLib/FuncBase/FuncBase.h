/************************************************************************/
/* �ļ����ƣ�FuncBase.h
/* ����ʱ�䣺2013-02-26 03:54
/*
/* ��    ����������չ�⣬ʵ���˻����ĺ�������
/************************************************************************/
#include <QtCore>
#include "StockInfoItem.h"

extern "C"{_declspec(dllexport) QMap<const char*,lua_CFunction>* ExportAllFuncs();}

int my_lua_high(lua_State* _L);
int my_lua_low(lua_State* _L);
int my_lua_open(lua_State* _L);
int my_lua_close(lua_State* _L);
int my_lua_volume(lua_State* _L);
int my_lua_lastclose(lua_State* _L);
int my_lua_ltag(lua_State* _L);