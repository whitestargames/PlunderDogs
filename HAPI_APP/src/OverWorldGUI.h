#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>

#include "Global.h"
#include "GUIBase.h"

struct EntityProperties;
class OverWorld;

class OverWorldGUI : public GUIBase
{
public:
	OverWorldGUI(OverWorld& overWorld);

	void render() override final;

private:
	std::vector<EntityProperties*> m_selectedEntities;
	EntityProperties* m_currentlySelected;
	OverWorldWindow CURRENT_WINDOW;
	OverWorld& OVER_WORLD;

	std::unique_ptr<Sprite> m_battleMapBackground;
	std::unique_ptr<Sprite> m_enemyTerritoryHexSheet;
	std::unique_ptr<Sprite> m_prebattleUIBackground;
	std::unique_ptr<Sprite> m_playButton;
	std::unique_ptr<Sprite> m_backButton;

	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}

	void onLeftClick(const HAPI_TMouseData& mouseData) override final;
	void onRightClick(const HAPI_TMouseData& mouseData) override final;

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
};