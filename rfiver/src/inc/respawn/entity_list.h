#pragma once

namespace respawn
{
	class c_entity_list
	{
	public:
		virtual int unknowFun1() = 0;
		virtual int unknowFun2() = 0;
		virtual int unknowFun3() = 0;
		virtual int unknowFun4() = 0;
		virtual c_entity* GetClientEntity(int entNum) = 0;
		virtual void* GetClientEntityFromHandle(void* hEnt) = 0;
		virtual int NumberOfEntities(bool bIncludeNonNetworkable) = 0;
		virtual int unknowFun5() = 0;
		virtual int GetHighestEntityIndex();
	};
}
