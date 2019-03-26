#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

#pragma once
class BattleUI : public IHapiSpritesInputListener
{
	enum class BattleWindow
	{
		Battle = 0,
		Pause
	};
public:
	BattleUI();

	void render();
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

private:
	std::unique_ptr<Sprite> m_battleIcons;
	std::unique_ptr<Sprite> m_pauseButton;
	std::unique_ptr<Sprite> m_chickenButton;
	std::unique_ptr<Sprite> m_pauseMenuBackground;
	std::unique_ptr<Sprite> m_resumeButton;
	std::unique_ptr<Sprite> m_quitButton;

	BattleWindow m_currentWindow;
};