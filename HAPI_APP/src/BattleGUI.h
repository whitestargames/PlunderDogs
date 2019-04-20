#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

enum BattleWindow
{
	eCombat = 0,
	ePause,
	ePostBattle
};

class BattleGUI
{
public:
	BattleGUI(std::pair<int, int> maxCameraOffset);

	std::pair<int, int> getCameraPositionOffset() const;

	void render() const;
	void update(int WindDirection);
	void OnMouseLeftClick(const HAPI_TMouseData& mouseData);
	//void OnMouseScroll could be added
	void OnMouseMove(const HAPI_TMouseData& mouseData);
	

private:
	std::unique_ptr<Sprite> m_battleIcons;
	std::unique_ptr<Sprite> m_pauseButton;
	std::unique_ptr<Sprite> m_chickenButton;
	std::unique_ptr<Sprite> m_pauseMenuBackground;
	std::unique_ptr<Sprite> m_resumeButton;
	std::unique_ptr<Sprite> m_quitButton;
	std::unique_ptr<Sprite> m_Compass;
	//postBattleScreen
	std::unique_ptr<Sprite> m_postBattleBackground;
	std::unique_ptr<Sprite> m_doneButton;


	BattleWindow m_currentBattleWindow;
	std::pair<int, int> m_maxCameraOffset;
	//Map m_map;
	DWORD animationStartTime;
	bool shipSelected;
	bool playAnimation;
	int animationOffset = 100;
	bool victory = false;
	VectorF pendingCameraMovement{ 0 };
	std::pair<int, int> CameraPositionOffset;//camera offset that will be used by the map
	float cameraZoom = 1.0f;//variable to multiply scale by
	std::pair<int, int> m_cameraPositionOffset;
};