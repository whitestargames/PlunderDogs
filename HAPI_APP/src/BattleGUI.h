#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "Global.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;


class BattleGUI
{
public:
	BattleGUI();
	VectorF pendingCameraMovement{ 0 };
	std::pair<int, int> CameraPositionOffset;//camera offset that will be used by the map
	float cameraZoom = 1.0f;//variable to multiply scale by

	void render();
	void OnMouseLeftClick(const HAPI_TMouseData& mouseData);
	void OnMouseRightClick(const HAPI_TMouseData& mouseData);
	//void OnMouseScroll could be added
	void OnMouseMove(const HAPI_TMouseData& mouseData);

private:
	std::unique_ptr<Sprite> m_battleIcons;
	std::unique_ptr<Sprite> m_pauseButton;
	std::unique_ptr<Sprite> m_chickenButton;
	std::unique_ptr<Sprite> m_pauseMenuBackground;
	std::unique_ptr<Sprite> m_resumeButton;
	std::unique_ptr<Sprite> m_quitButton;
	//postBattleScreen
	std::unique_ptr<Sprite> m_postBattleBackground;
	std::unique_ptr<Sprite> m_doneButton;

	BattleWindow m_currentBattleWindow;
	//Map m_map;
	DWORD animationStartTime;
	bool shipSelected;
	bool playAnimation;
	int animationOffset = 100;
	bool victory = false;
};