#pragma once

#include <string>
#include "Battle.h"
#include "OverWorldGUI.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

enum class OverWorldWindow
{
	Battle = 0,
	PreBattle,
	LevelSelection
};

class OverWorld : public IHapiSpritesInputListener
{
public:
	OverWorld();

	void render();
	void update(float deltaTime);
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	
	static OverWorldWindow CURRENT_WINDOW;

private:
	std::unique_ptr<GUIBase> m_overWorldGUI;
	Battle m_battle;
};