#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>

#include "Global.h"
#include <string>

struct Player;
struct EntityProperties;
class OverWorld;
class Battle;
class OverWorldGUI 
{
private:
	EntityProperties* m_currentlySelected;
	bool m_enitiesAdded;
	//m_enemyTerritoryHexSheet   might need name for replacing
	std::unique_ptr<Sprite> m_battleMapBackground;
	std::unique_ptr<Sprite> m_selectMapButtons1;
	std::unique_ptr<Sprite> m_selectMapButtons2;
	std::unique_ptr<Sprite> m_selectMapButtons3;
	std::unique_ptr<Sprite> m_prebattleUIBackground;
	std::unique_ptr<Sprite> m_playButton;
	std::unique_ptr<Sprite> m_backButton;



	void positionEntity(const std::string& windowName, const std::string& windowSliderName, const std::string& windowObjectName, int objectNumber, size_t vectorSize);
	float getWindowSliderValue(const std::string &windowName, const std::string &windowSliderName) const;
	HAPISPACE::RectangleI calculateObjectWindowPosition(int objectNumber) const;
	HAPISPACE::RectangleI windowScreenRect(const std::string& windowName) const;
	HAPISPACE::VectorI calculateObjectScrolledPosition(const std::string& windowName, const std::string& windowSliderName, int objectNumber, size_t vectorSize);
	bool entityContainsMouse(const std::string& windowName, const std::string& windowObjectName, HAPISPACE::VectorI windowTopLeft, HAPISPACE::VectorI mousePosition) const;
	bool windowObjectExists(const std::string& windowName, const std::string& windowObjectName) const;

	HAPISPACE::SliderLayout sliderLayout;
	HAPISPACE::VectorI m_fleetWindowTopLeft{ HAPISPACE::VectorI(220, 510) };
	HAPISPACE::VectorI m_battleFleetWindowTopLeft{ HAPISPACE::VectorI(220, 220) };

	const std::string fleetWindowSliderSkinName;
	const std::string fleetWindowSkinName;
	const std::string ENTITY = "entity";
	const std::string FLEET_WINDOW = "fleetwindow";
	const std::string BATTLE_FLEET_WINDOW = "battleFleetWindow";
	const std::string FLEET_SLIDER = "fleetSlider";
	const std::string BATTLE_FLEET_SLIDER = "battleFleetSlider";

public:
	static OverWorldWindow CURRENT_WINDOW;
	OverWorldGUI();

	void onLeftClick(const HAPI_TMouseData& mouseData, Player& currentSelectedPlayer, bool& selectNextPlayer);
	void onRightClick(const HAPI_TMouseData& mouseData, Player& currentSelectedPlayer);
	void onMouseMove(const HAPI_TMouseData& mouseData, Player& currentSelectedPlayer);

	void reset(std::vector<EntityProperties>& playerEntities);
	void clear();

	void render(std::unique_ptr<Battle>& battle);
};