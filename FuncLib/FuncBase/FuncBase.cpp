#include "FuncBase.h"
#include "RStockFunc.h"

QMap<const char*,lua_CFunction>* ExportAllFuncs()
{
	QMap<const char*,lua_CFunction>* pFuncs = new QMap<const char*,lua_CFunction>;
	pFuncs->insert("RHigh",&my_lua_high);
	pFuncs->insert("RLow",&my_lua_low);
	pFuncs->insert("ROpen",&my_lua_open);
	pFuncs->insert("RClose",&my_lua_close);
	pFuncs->insert("RVolume",&my_lua_volume);
	pFuncs->insert("RLastClose",&my_lua_lastclose);
	pFuncs->insert("RLtAg",&my_lua_ltag);

	return pFuncs;
}

int my_lua_high( lua_State* _L )
{
	lua_getglobal(_L,"_calc");
	RCalcInfo* pCalc = reinterpret_cast<RCalcInfo*>(lua_touserdata(_L,-1));
	lua_pop(_L,1);
	if(pCalc&&pCalc->mapData)
	{
		lua_newtable(_L);
//		lua_pushnumber(_L,-1);
//		lua_rawseti(_L,-2,0);


		QMap<time_t,RStockData*>::iterator iter = pCalc->mapData->begin();
		if(iter != pCalc->mapData->end())
		{
			int _i = 1;
			float fLast = 0;
			while(iter!=pCalc->mapData->end())
			{
				if((*iter)!=NULL)
				{
					fLast = (*iter)->fClose;
					lua_pushnumber(_L,(*iter)->fHigh);
				}
				else
				{
					lua_pushnumber(_L,fLast);
				}
				lua_rawseti(_L,-2,_i);
				++iter;
				++_i;
			}
		}
		lua_getglobal(_L,"Array");
		lua_setmetatable(_L,-2);

		return 1;
	}
	return 0;
}

int my_lua_low( lua_State* _L )
{
	lua_getglobal(_L,"_calc");
	RCalcInfo* pCalc = reinterpret_cast<RCalcInfo*>(lua_touserdata(_L,-1));
	lua_pop(_L,1);
	if(pCalc&&pCalc->mapData)
	{
		lua_newtable(_L);
//		lua_pushnumber(_L,-1);
//		lua_rawseti(_L,-2,0);


		QMap<time_t,RStockData*>::iterator iter = pCalc->mapData->begin();
		if(iter != pCalc->mapData->end())
		{
			int _i = 1;
			float fLast = 0;
			while(iter!=pCalc->mapData->end())
			{
				if((*iter)!=NULL)
				{
					fLast = (*iter)->fClose;
					lua_pushnumber(_L,(*iter)->fLow);
				}
				else
				{
					lua_pushnumber(_L,fLast);
				}
				lua_rawseti(_L,-2,_i);
				++iter;
				++_i;
			}
		}
		lua_getglobal(_L,"Array");
		lua_setmetatable(_L,-2);

		return 1;
	}
	return 0;
}

int my_lua_open( lua_State* _L )
{
	lua_getglobal(_L,"_calc");
	RCalcInfo* pCalc = reinterpret_cast<RCalcInfo*>(lua_touserdata(_L,-1));
	lua_pop(_L,1);
	if(pCalc&&pCalc->mapData)
	{
		lua_newtable(_L);
//		lua_pushnumber(_L,-1);
//		lua_rawseti(_L,-2,0);

		QMap<time_t,RStockData*>::iterator iter = pCalc->mapData->begin();
		if(iter != pCalc->mapData->end())
		{
			int _i = 1;
			float fLast = 0;
			while(iter!=pCalc->mapData->end())
			{
				if((*iter)!=NULL)
				{
					fLast = (*iter)->fClose;
					lua_pushnumber(_L,(*iter)->fOpen);
				}
				else
				{
					lua_pushnumber(_L,fLast);
				}
				lua_rawseti(_L,-2,_i);
				++iter;
				++_i;
			}
		}
		lua_getglobal(_L,"Array");
		lua_setmetatable(_L,-2);

		return 1;
	}
	return 0;
}

int my_lua_close( lua_State* _L )
{
	lua_getglobal(_L,"_calc");
	RCalcInfo* pCalc = reinterpret_cast<RCalcInfo*>(lua_touserdata(_L,-1));
	lua_pop(_L,1);
	if(pCalc&&pCalc->mapData)
	{
		lua_newtable(_L);
//		lua_pushnumber(_L,-1);
//		lua_rawseti(_L,-2,0);

		QMap<time_t,RStockData*>::iterator iter = pCalc->mapData->begin();
		if(iter != pCalc->mapData->end())
		{
			int _i = 1;
			float fLast = 0;
			while(iter!=pCalc->mapData->end())
			{
				if((*iter)!=NULL)
				{
					fLast = (*iter)->fClose;
					lua_pushnumber(_L,(*iter)->fClose);
				}
				else
				{
					lua_pushnumber(_L,fLast);
				}
				lua_rawseti(_L,-2,_i);
				++iter;
				++_i;
			}
		}
		lua_getglobal(_L,"Array");
		lua_setmetatable(_L,-2);
		return 1;
	}
	return 0;
}

int my_lua_volume( lua_State* _L )
{
	lua_getglobal(_L,"_calc");
	RCalcInfo* pCalc = reinterpret_cast<RCalcInfo*>(lua_touserdata(_L,-1));
	lua_pop(_L,1);
	if(pCalc&&pCalc->mapData)
	{
		lua_newtable(_L);

		QMap<time_t,RStockData*>::iterator iter = pCalc->mapData->begin();
		if(iter != pCalc->mapData->end())
		{
			int _i = 1;
			while(iter!=pCalc->mapData->end())
			{
				if((*iter)!=NULL && (*iter)->fVolume>0.0)
				{
					lua_pushnumber(_L,(*iter)->fVolume);
				}
				else
				{
					lua_pushnumber(_L,0);
				}
				lua_rawseti(_L,-2,_i);
				++iter;
				++_i;
			}
		}
		lua_getglobal(_L,"Array");
		lua_setmetatable(_L,-2);
		return 1;
	}
	return 0;
}

int my_lua_lastclose( lua_State* _L )
{
	lua_getglobal(_L,"_calc");
	RCalcInfo* pCalc = reinterpret_cast<RCalcInfo*>(lua_touserdata(_L,-1));
	lua_pop(_L,1);
	if(pCalc&&pCalc->pItem)
	{
		lua_pushnumber(_L,pCalc->pItem->getLastClose());
		return 1;
	}
	return 0;
}

int my_lua_ltag( lua_State* _L )
{
	lua_getglobal(_L,"_calc");
	RCalcInfo* pCalc = reinterpret_cast<RCalcInfo*>(lua_touserdata(_L,-1));
	lua_pop(_L,1);
	if(pCalc&&pCalc->pItem)
	{
		lua_pushnumber(_L,pCalc->pItem->getLtag());
		return 1;
	}
	return 0;
}
