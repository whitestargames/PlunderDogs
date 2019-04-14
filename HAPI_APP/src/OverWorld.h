#pragma once

#include <string>
#include "Global.h"
#include "Battle.h"
#include "OverWorldGUI.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

class OverWorld : public IHapiSpritesInputListener
{
public:
	OverWorld();

	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}

	void render();
	void update(float deltaTime);
	void startBattle();

private:
	std::vector<EntityProperties> m_entities;
	std::vector<EntityProperties*> m_selectedEntities;
	OverWorldGUI m_GUI;
	std::unique_ptr<Battle> m_battle;

	//Temporary - will change functionality in next few days
	//14/04/19
	bool m_startBattle;
};