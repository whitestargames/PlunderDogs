#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "Global.h"
#include "Textures.h"
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
	~BattleGUI();

	std::pair<int, int> getCameraPositionOffset() const;

	void render() const;
	void update(eDirection windDirection);
	void updateFactionToken(int factionName);
	void OnMouseLeftClick(const HAPI_TMouseData& mouseData);
	//void OnMouseScroll could be added
	void OnMouseMove(const HAPI_TMouseData& mouseData);
	

private:
	BattleWindow m_currentBattleWindow;
	std::pair<int, int> m_maxCameraOffset;
	std::unique_ptr<Sprite> m_battleIcons;
	std::unique_ptr<Sprite> m_pauseButton;
	std::unique_ptr<Sprite> m_chickenButton;
	std::unique_ptr<Sprite> m_pauseMenuBackground;
	std::unique_ptr<Sprite> m_resumeButton;
	std::unique_ptr<Sprite> m_quitButton;
	std::unique_ptr<Sprite> m_postBattleBackground;
	std::unique_ptr<Sprite> m_doneButton;
	std::unique_ptr<Sprite> m_activeFactionToken;
	std::unique_ptr<Sprite> m_CompassPointer;
	std::unique_ptr<Sprite> m_CompassBackGround;
	
	DWORD animationStartTime;
	bool shipSelected;
	bool playAnimation;
	int animationOffset = 100;
	bool victory = false;
	VectorF pendingCameraMovement{ 0 };
	std::pair<int, int> CameraPositionOffset;//camera offset that will be used by the map
	float cameraZoom = 1.0f;//variable to multiply scale by
	std::pair<int, int> m_cameraPositionOffset;
	void onReset();
};