#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <string>
#include "Battle.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

class OverWorld : public IHapiSpritesInputListener
{
	enum class OverWorldWindow
	{
		Battle = 0,
		PreBattle,              
		LevelSelection
	};
public:
	OverWorld();

	void render();
	void update() {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE ikeyCode) override final {}

private:
	void positionEntity(const std::string& windowName, const std::string& windowSliderName, std::string windowObjectName, int objectNumber, std::vector<Entity> &entityVector);
	void positionEntity(const std::string& windowName, const std::string& windowSliderName, std::string windowObjectName, int objectNumber, std::vector<Entity*> &entityVector);
	float getWindowSliderValue(const std::string &windowName, const std::string &windowSliderName);
	HAPISPACE::RectangleI calculateObjectWindowPosition(int objectNumber);
	HAPISPACE::VectorI calculateObjectScrolledPosition(const std::string& windowName, const std::string& windowSliderName, int objectNumber, std::vector<Entity> &entityVector);
	HAPISPACE::VectorI calculateObjectScrolledPosition(const std::string& windowName, const std::string& windowSliderName, int objectNumber, std::vector<Entity*> &entityVector);
	bool entityContainsMouse(const std::string& windowName, std::string windowObjectName, HAPISPACE::VectorI windowTopLeft, HAPISPACE::VectorI mousePosition);

	std::unique_ptr<Sprite> m_battleMapBackground;
	std::unique_ptr<Sprite> m_enemyTerritoryHexSheet;
	std::unique_ptr<Sprite> m_prebattleUIBackground;
	std::unique_ptr<Sprite> m_playButton;
	std::unique_ptr<Sprite> m_backButton;
	std::vector<Entity> m_entityVector;
	std::vector<Entity*> m_selectedEntities;
	Entity* m_currentlySelected {nullptr};
	HAPISPACE::SliderLayout sliderLayout;
	HAPISPACE::VectorI m_fleetWindowTopLeft{HAPISPACE::VectorI(220, 510)};
	HAPISPACE::VectorI m_battleFleetWindowTopLeft{HAPISPACE::VectorI(220, 220)};
	OverWorldWindow m_currentWindow;
	Battle m_battle;

	const std::string FLEET_WINDOW = "fleetwindow";
	const std::string BATTLE_FLEET_WINDOW = "battleFleetWindow";
	const std::string FLEET_SLIDER = "fleetSlider";
	const std::string BATTLE_FLEET_SLIDER = "battleFleetSlider";
};