#include "BattleUI.h"
#include "Utilities/Utilities.h"

BattleUI::BattleUI()
	: m_battleIcons(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "battleIcons.png")),
	m_pauseButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "pauseButton.png", 2)),
	m_chickenButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "chickenButton.png")),
	m_pauseMenuBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "pauseMenuBackground.png")),
	m_resumeButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "resumeButton.png", 2)),
	m_quitButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "quitButton.png", 2))
{
	m_battleIcons->GetTransformComp().SetPosition({ 350, 800 });
	m_pauseButton->GetTransformComp().SetPosition({ 1450, 50 });
	m_chickenButton->GetTransformComp().SetPosition({ 1450, 750 });

	m_pauseMenuBackground->GetTransformComp().SetPosition({ 568, 152 });
	m_resumeButton->GetTransformComp().SetPosition({ 658, 307 });
	m_quitButton->GetTransformComp().SetPosition({ 658, 322 });
}

void BattleUI::render()
{
	SCREEN_SURFACE->Clear();

	m_battleIcons->Render(SCREEN_SURFACE);
	m_pauseButton->Render(SCREEN_SURFACE);
	m_chickenButton->Render(SCREEN_SURFACE);

	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(300, 800), HAPISPACE::Colour255::BLACK, "45/55", 50);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(400, 800), HAPISPACE::Colour255::BLACK, "3", 50);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(500, 800), HAPISPACE::Colour255::BLACK, "4", 50);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(600, 800), HAPISPACE::Colour255::BLACK, "5", 50);

	switch (m_currentWindow)
	{
	case BattleWindow::Battle:
	{
		break;//the battle should continue to render behind the pause menu so is before the switch case
	}
	case BattleWindow::Pause:
	{
		m_pauseMenuBackground->Render(SCREEN_SURFACE);
		m_resumeButton->Render(SCREEN_SURFACE);
		m_quitButton->Render(SCREEN_SURFACE);
		break;
	}
	}
}

void BattleUI::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		switch (m_currentWindow)
		{
		case BattleWindow::Battle:
		{
			if (m_pauseButton->GetSpritesheet()->GetFrameRect(0).Translated(
				m_pauseButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//if you press the pause button
			{
				m_currentWindow = BattleWindow::Pause;//enables the pause menu
			}
			break;
		}
		case BattleWindow::Pause:
		{
			if (m_resumeButton->GetSpritesheet()->GetFrameRect(0).Translated(
				m_resumeButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//if you press the resume button
			{
				m_currentWindow = BattleWindow::Battle;//disables the pause menu
			}
			break;
		}
		}
	}
}

void BattleUI::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	switch (m_currentWindow)
	{
	case BattleWindow::Battle:
	{
		if (m_pauseButton->GetSpritesheet()->GetFrameRect(0).Translated(m_pauseButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//checks if mouse is over button
		{
			m_pauseButton->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_pauseButton->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_pauseButton->SetFrameNumber(0);// sets it to the default sprite
		}
		break;
	}
	case BattleWindow::Pause:
	{
		if (m_resumeButton->GetSpritesheet()->GetFrameRect(0).Translated(m_resumeButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_resumeButton->SetFrameNumber(1);
		}
		else if (m_resumeButton->GetFrameNumber() != 0)
		{
			m_resumeButton->SetFrameNumber(0);
		}

		if (m_quitButton->GetSpritesheet()->GetFrameRect(0).Translated(m_quitButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_quitButton->SetFrameNumber(1);
		}
		else if (m_quitButton->GetFrameNumber() != 0)
		{
			m_quitButton->SetFrameNumber(0);
		}
		break;
	}
	}
}