#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "Battle.h"
//#include "Map.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;


class BattleUI : public IHapiSpritesInputListener
{
	enum class BattleWindow
	{
		Battle = 0,
		Pause
	};
public:
	BattleUI();
	//bool sideBoundary = false;
	//bool floorBoundary = false;//if sides are on the left and right, what is on the top and bottom. I dont think a word exists for that. vertical, plane & floor arnt quite right.
	//VectorF pendingCameraMovement{ 0 };
	//std::pair<int, int> CameraPositionOffset;//camera offset that will be used by the map
	//float cameraZoom = 1.0f;//variable to multiply scale by

	void render();
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}

private:
	std::unique_ptr<Sprite> m_battleIcons;
	std::unique_ptr<Sprite> m_pauseButton;
	std::unique_ptr<Sprite> m_chickenButton;
	std::unique_ptr<Sprite> m_pauseMenuBackground;
	std::unique_ptr<Sprite> m_resumeButton;
	std::unique_ptr<Sprite> m_quitButton;

	BattleWindow m_currentWindow;

	Battle m_battle;
	/*Map m_map;*/
	DWORD animationStartTime;
	bool shipSelected;
	bool playAnimation;
	int animationOffset = 100;
};