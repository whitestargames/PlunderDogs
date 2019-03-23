#pragma once
#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
using namespace HAPISPACE;
using namespace HAPI_UI_SPACE;

class BattleUI :public IHapiSpritesInputListener
{
private:
	int m_mouseX;
	int	m_mouseY;
	bool m_trigger;

public:
	BattleUI();

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void HandleCollision(std::unique_ptr<Sprite>& sprite, std::unique_ptr<Sprite>& collideWith);
	void Update();
	void render();
	
	std::pair<float,float> m_tilePos;// this is to get center of sprite
	std::unique_ptr<Sprite> m_mouseCursor;
};