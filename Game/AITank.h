
#ifndef _GAME_AITANK_H_
#define _GAME_AITANK_H_

#include "Tank.h"

namespace RTR
{
	// ʵ���˼�AI��tank
	class AITank : public Tank
	{
	public:
		virtual ~AITank(){}

		virtual void onHited();
		// ����
		virtual void fire();
		// ����tank
		virtual void update();

	protected:
		// ����AI
		void updateAI();

	protected:
		friend class GameManager;
		friend class PlayerController;
		AITank(const std::string &name, const std::string &meshName, GameManager *gameMgr);
	};
}

#endif