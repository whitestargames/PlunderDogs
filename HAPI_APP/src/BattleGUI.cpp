#include "BattleGUI.h"
#include "Utilities/Utilities.h"
#include "Utilities/MapParser.h"

BattleGUI::BattleGUI(std::pair<int, int> maxCameraOffset)
	:
	m_battleIcons(HAPI_Sprites.MakeSprite(Textures::m_battleIcons)),
	m_pauseButton(HAPI_Sprites.MakeSprite(Textures::m_pauseButton)),
	m_chickenButton(HAPI_Sprites.MakeSprite(Textures::m_chickenButton)),
	m_pauseMenuBackground(HAPI_Sprites.MakeSprite(Textures::m_pauseMenuBackground)),
	m_resumeButton(HAPI_Sprites.MakeSprite(Textures::m_resumeButton)),
	m_quitButton(HAPI_Sprites.MakeSprite(Textures::m_quitButton)),
	m_postBattleBackground(HAPI_Sprites.MakeSprite(Textures::m_postBattleBackground)),
	m_doneButton(HAPI_Sprites.MakeSprite(Textures::m_doneButton)),
	m_activeFactionToken(HAPI_Sprites.MakeSprite(Textures::m_activeFactionToken)),
	m_CompassPointer(HAPI_Sprites.MakeSprite(Textures::m_CompassPointer)),
	m_CompassBackGround(HAPI_Sprites.MakeSprite(Textures::m_CompassBackGround)),

	m_currentBattleWindow(BattleWindow::eCombat),
	m_maxCameraOffset(maxCameraOffset)
{	
	//battle
	m_battleIcons->GetTransformComp().SetPosition({ 350, 800 });
	m_pauseButton->GetTransformComp().SetPosition({ 1450, 50 });
	m_chickenButton->GetTransformComp().SetPosition({ 1450, 750 });
	m_CompassBackGround->GetTransformComp().SetOriginToCentreOfFrame();
	m_CompassBackGround->GetTransformComp().SetPosition({ 80, 80 });
	m_CompassPointer->GetTransformComp().SetOrigin({ 21.5f,60 });
	m_CompassPointer->GetTransformComp().SetPosition({ 80, 80 });
	m_activeFactionToken->GetTransformComp().SetOriginToCentreOfFrame();
	m_activeFactionToken->GetTransformComp().SetPosition({ 1350,50 });// position just temp can be adjusted as needed
	
	
	//pauseMenu
	m_resumeButton->GetTransformComp().SetPosition({ 658, 297 });
	m_quitButton->GetTransformComp().SetPosition({ 658, 427 });
	//postBattle
	m_postBattleBackground->GetTransformComp().SetPosition({ 200, 100 });
	m_doneButton->GetTransformComp().SetPosition({ 660, 710 });
}

std::pair<int, int> BattleGUI::getCameraPositionOffset() const
{
	return m_cameraPositionOffset;
}

void BattleGUI::render() const
{
	
	m_battleIcons->Render(SCREEN_SURFACE);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(440, (815 + animationOffset)), HAPISPACE::Colour255::BLACK, "45/55", 44);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(700, (815 + animationOffset)), HAPISPACE::Colour255::BLACK, "4", 44);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(900, (815 + animationOffset)), HAPISPACE::Colour255::BLACK, "3/4", 44);
	SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1135, (815 + animationOffset)), HAPISPACE::Colour255::BLACK, "5", 44);
		
	
	m_pauseButton->Render(SCREEN_SURFACE);
	m_chickenButton->Render(SCREEN_SURFACE);
	m_CompassBackGround->Render(SCREEN_SURFACE);
	m_CompassPointer->Render(SCREEN_SURFACE);
	m_activeFactionToken->Render(SCREEN_SURFACE);

	switch (m_currentBattleWindow)
	{
	case BattleWindow::ePause:
	{
		m_pauseMenuBackground->Render(SCREEN_SURFACE);
		m_resumeButton->Render(SCREEN_SURFACE);
		m_quitButton->Render(SCREEN_SURFACE);
		break;
	}
	case BattleWindow::ePostBattle:
	{
		m_postBattleBackground->Render(SCREEN_SURFACE);
		m_doneButton->Render(SCREEN_SURFACE);
		if (victory)
		{
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(595, 115), HAPISPACE::Colour255::GREEN, "VICTORY", 90);
		}
		else
		{
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(615, 115), HAPISPACE::Colour255::RED, "DEFEAT", 90);
		}
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(800, 330), HAPISPACE::Colour255::BLACK, "KILLS: " "50G", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(800, 415), HAPISPACE::Colour255::BLACK, "WIN BONUS: " "15G", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(800, 500), HAPISPACE::Colour255::BLACK, "HEALTH BONUS: " "35G", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(800, 585), HAPISPACE::Colour255::BLACK, "TOTAL: " "21G", 50);
		break;
	}
	}
}

void BattleGUI::update(eDirection windDirection)
{
	//m_CompassPointer->GetTransformComp().SetRotation(static_cast<float>(windDirection) * 0.333333 * 3.14159);
	m_CompassPointer->GetTransformComp().SetRotation(DEGREES_TO_RADIANS(static_cast<int>(windDirection) *45 % 360));

	if (shipSelected)
	{
		if (playAnimation)
		{
			animationOffset = 100 - (HAPI_Sprites.GetTime() - animationStartTime);
			if (animationOffset < 1)
			{
				playAnimation = false;
				animationOffset = 0;
			}

			m_battleIcons->GetTransformComp().SetPosition({ 350, (800 + static_cast<float>(animationOffset)) });
		}
	}

	switch (m_currentBattleWindow)
	{
	case BattleWindow::eCombat:
	{
		//camera pan
		if (!pendingCameraMovement.IsZero())
		{
			CameraPositionOffset.first += pendingCameraMovement.x;//translates the camera position
			CameraPositionOffset.second += pendingCameraMovement.y;

			if (CameraPositionOffset.first < -150)//checks for if its reached any of the 4 boundries, need to change it to a width variable
			{
				CameraPositionOffset.first = -150;
			}
			else if (CameraPositionOffset.first > m_maxCameraOffset.first)
			{
				CameraPositionOffset.first = m_maxCameraOffset.first;
			}
			else
			{
				CameraPositionOffset.first += pendingCameraMovement.x;
			}

			if (CameraPositionOffset.second < -200)
			{
				CameraPositionOffset.second = -200;
			}
			else if (CameraPositionOffset.second > m_maxCameraOffset.second)
			{
				CameraPositionOffset.second = m_maxCameraOffset.second;
			}
			else
			{
				CameraPositionOffset.second += pendingCameraMovement.y;
			}

			m_cameraPositionOffset = CameraPositionOffset;
			//m_battle.setMapDrawOffset(CameraPositionOffset);//TODO: CREATE A FUNCTION FOR THIS IN BATTLE THAT'S CALLED INSTEAD
		}
		break;//the battle should continue to render behind the pause menu so is before the switch case
	}
	}
}

void BattleGUI::updateFactionToken(int factionName)
{
	m_activeFactionToken->SetFrameNumber(static_cast<int>(factionName));
}

void BattleGUI::OnMouseLeftClick(const HAPI_TMouseData& mouseData)
{
	switch (m_currentBattleWindow)
	{
	case BattleWindow::eCombat:
	{
		if (m_pauseButton->GetSpritesheet()->GetFrameRect(0).Translated(
			m_pauseButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//if you press the pause button
		{
			m_currentBattleWindow = BattleWindow::ePause;//enables the pause menu
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
	case BattleWindow::ePause:
	{
		if (m_resumeButton->GetSpritesheet()->GetFrameRect(0).Translated(
			m_resumeButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//if you press the resume button
		{
			m_currentBattleWindow = BattleWindow::eCombat;//disables the pause menu
		}
		else if (m_quitButton->GetSpritesheet()->GetFrameRect(0).Translated(
			m_quitButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//if you press the resume button
		{
			m_currentBattleWindow = BattleWindow::ePostBattle;//disables the pause menu
		}
		break;
	}
	case BattleWindow::ePostBattle:
	{
		if (m_doneButton->GetSpritesheet()->GetFrameRect(0).Translated(
			m_doneButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//if you press the pause button
		{
			//switch to overworld
		}
		break;
	}
	}
}

void BattleGUI::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	switch (m_currentBattleWindow)
	{
	case BattleWindow::eCombat:
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
	case BattleWindow::ePause:
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
	case BattleWindow::ePostBattle:
	{
		if (m_doneButton->GetSpritesheet()->GetFrameRect(0).Translated(m_doneButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_doneButton->SetFrameNumber(1);
		}
		else if (m_doneButton->GetFrameNumber() != 0)
		{
			m_doneButton->SetFrameNumber(0);
		}
		break;
	}
	}
}