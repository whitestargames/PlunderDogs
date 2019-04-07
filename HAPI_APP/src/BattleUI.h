#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>

struct Tile;
class Battle;
class BattleUI : public IHapiSpritesInputListener
{
	struct InvalidPositionSprite
	{
		InvalidPositionSprite();

		void render() const;
		void setPosition(std::pair<int, int> screenPosition, float mapDrawScale);

		std::unique_ptr<Sprite> m_sprite;
		bool m_activate;
	};

public:
	BattleUI(Battle& battle);

	void render() const;

	void newPhase();

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

private:
	Battle& m_battle;
	const Tile* m_currentTileSelected;
	InvalidPositionSprite m_invalidPositionSprite;
	//This stores the position at which the mouse event "eLeftMouseButtonDown" last occured while giving an entity a move command, its used to calculate what direction the mouse moved during that input.
	std::pair<int, int> m_leftMouseDownPosition{ 0,0 };
	//This is used to determine if an entity is currently being given a move command, it gets set to true in the "handleOnLeftClickMovementPhase()" and false after "eLeftMouseButtonUp" is detected.
	bool m_isMovingEntity{ false };

	//Movement Phase
	void handleOnMouseMoveMovementPhase();
	void handleOnLeftClickMovementPhase();
	void handleOnRightClickMovementPhase();

	//Attack Phase
	void handleOnLeftClickAttackPhase();
};