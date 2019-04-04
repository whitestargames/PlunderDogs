#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>

constexpr float DRAW_OFFSET_X{ 12 };
constexpr float DRAW_OFFSET_Y{ 28 };
struct Tile;
class Map;
class Battle;
class BattleUI : public IHapiSpritesInputListener
{
	class MovementPath
	{
	public:
		MovementPath();

		void render() const;
		void generatePath(Map& map, const Tile& currentTileSelected);

	private:
		std::vector<std::pair<std::unique_ptr<Sprite>, bool>> m_movementPath;
		std::unique_ptr<Sprite> m_mouseCursor;

		void clearPath();
	};

public:
	BattleUI(Battle& battle);

	void render();

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

private:
	Battle& m_battle;
	Tile* m_currentTileSelected;
	MovementPath m_movementPath;
};
