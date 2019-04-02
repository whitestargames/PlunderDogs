#include "BattleUI.h"
#include "Utilities/Utilities.h"
#include "Utilities/MapParser.h"

BattleUI::BattleUI()
	: m_battleIcons(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "battleIcons.png")),
	m_pauseButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "pauseButton.png", 2)),
	m_chickenButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "chickenButton.png")),
	m_pauseMenuBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "pauseMenuBackground.png")),
	m_resumeButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "resumeButton.png", 2)),
	m_quitButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "quitButton.png", 2))//,
	//m_map(MapParser::parseMap(Utilities::getDataDirectory() + "Level1.tmx"))
{
	m_battleIcons->GetTransformComp().SetPosition({ 350, 800 });
	m_pauseButton->GetTransformComp().SetPosition({ 1450, 50 });
	m_chickenButton->GetTransformComp().SetPosition({ 1450, 750 });

	//m_pauseMenuBackground->GetTransformComp().SetPosition({ 568, 152 });
	m_resumeButton->GetTransformComp().SetPosition({ 658, 297 });
	m_quitButton->GetTransformComp().SetPosition({ 658, 427 });
}

void BattleUI::render()
{
	SCREEN_SURFACE->Clear();

	m_battle.render();

	if (shipSelected)
	{
		if (playAnimation)
		{
			animationOffset = 100 - (HAPI_Sprites.GetTime() - animationStartTime);
			if (animationOffset<1)
			{
				playAnimation = false;
				animationOffset = 0;
			}

			m_battleIcons->GetTransformComp().SetPosition({ 350, (800 + static_cast<float>(animationOffset)) });
		}
		m_battleIcons->Render(SCREEN_SURFACE);

		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(440, (815 + animationOffset)), HAPISPACE::Colour255::BLACK, "45/55", 44);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(700, (815 + animationOffset)), HAPISPACE::Colour255::BLACK, "4", 44);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(900, (815 + animationOffset)), HAPISPACE::Colour255::BLACK, "3/4", 44);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1135, (815 + animationOffset)), HAPISPACE::Colour255::BLACK, "5", 44);
		
	}
	m_pauseButton->Render(SCREEN_SURFACE);
	m_chickenButton->Render(SCREEN_SURFACE);

	switch (m_currentWindow)
	{
	case BattleWindow::Battle:
	{
		//camera pan
		if (!pendingCameraMovement.IsZero())
		{
			CameraPositionOffset.first += pendingCameraMovement.x;//translates the camera position
			CameraPositionOffset.second += pendingCameraMovement.y;

			if (CameraPositionOffset.first < -500)//checks for if its reached any of the 4 boundries, need to change it to a width variable
			{
				CameraPositionOffset.first = -500;
			}
			else if (CameraPositionOffset.first > 500)
			{
				CameraPositionOffset.first = 500;
			}
			else
			{
				CameraPositionOffset.first += pendingCameraMovement.x;
			}
			if (CameraPositionOffset.second < -400)
			{
				CameraPositionOffset.second = -400;
			}
			else if (CameraPositionOffset.second > 400)
			{
				CameraPositionOffset.second = 400;
			}
			else
			{
				CameraPositionOffset.second += pendingCameraMovement.y;
			}

			m_battle.setMapDrawOffset(CameraPositionOffset);
		}
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

			if (m_chickenButton->GetSpritesheet()->GetFrameRect(0).Translated(
				m_chickenButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//if you press the pause button
			{
				animationStartTime = HAPI_Sprites.GetTime();
				shipSelected = true;
				playAnimation = true;
			}
			else
			{
				shipSelected = false;
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


		//moves the sprites when the mouse is on the edge of the screen
		//only checks when mouse moves. if mouse doesnt move, it knows its still in the same spot and will keep scrolling without checking
		pendingCameraMovement = VectorF{ 0,0 };

		if (mouseData.x < 100)
		{
			pendingCameraMovement += VectorF{ -1,0 };
		}
		else if (mouseData.x > 1500)
		{
			pendingCameraMovement += VectorF{ 1,0 };
		}

		if (mouseData.y < 100)
		{
			pendingCameraMovement += VectorF{ 0,-1 };
		}
		else if (mouseData.y > 800)
		{
			pendingCameraMovement += VectorF{ 0,1 };
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