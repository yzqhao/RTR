
#ifndef _GAME_AITANK_H_
#define _GAME_AITANK_H_

#include "Tank.h"

namespace RTR
{
	// 实现了简单AI的tank
	class AITank : public Tank
	{
	public:
		virtual ~AITank(){}

		virtual void onHited();
		// 开火
		virtual void fire();
		// 更新tank
		virtual void update();

	protected:
		// 更新AI
		void updateAI();

	protected:
		friend class GameManager;
		friend class PlayerController;
		AITank(const std::string &name, const std::string &meshName, GameManager *gameMgr);
	};
}

#endif