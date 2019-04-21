#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "Global.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;
#include "Textures.h"
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
	void update(eDirection windDirection);
	void updateFactionToken(int tempfaction);
	void OnMouseLeftClick(const HAPI_TMouseData& mouseData);
	//void OnMouseScroll could be added
	void OnMouseMove(const HAPI_TMouseData& mouseData);
	

private:
	BattleWindow m_currentBattleWindow;
	Textures texture;
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