#include "BattleUI.h"
#include "Utilities/Utilities.h"

BattleUI::BattleUI()
	: m_mouseX(0),
	m_mouseY(0),
	m_trigger(false),
	m_mouseCursor(HAPI_Sprites.LoadSprite(Utilities::getDataDirectory() + "mouseCrossHair.xml"))
{
	m_mouseCursor->GetColliderComp().EnablePixelPerfectCollisions(true);
	HAPI_Sprites.SetShowCursor(false);
}

void BattleUI::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	m_mouseX = mouseData.x;
	m_mouseY = mouseData.y;

	HandleCollision(m_mouseCursor, m_mouseCursor);
	std::cout << m_mouseCursor->GetTransformComp().GetPosition().x << "\n";
	std::cout << m_mouseCursor->GetTransformComp().GetPosition().y << "\n";
}

void BattleUI::HandleCollision(std::unique_ptr<Sprite> & sprite, std::unique_ptr<Sprite> & collideWith)
{
	//sprite is the mouse cursor sprite
	//collideWith is the tile
	CollisionInfo info;
	if (sprite->CheckCollision(*collideWith.get(), &info) && m_trigger) //TODO: This line is the evil one 
	{
		m_tilePos = std::pair<float, float>(collideWith->GetTransformComp().GetPosition().x, collideWith->GetTransformComp().GetPosition().y);
		m_trigger = false;
	}
}

void BattleUI::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		m_mouseX = mouseData.x;
		m_mouseY = mouseData.y;
		m_trigger = true;
	}
}

void BattleUI::Update()
{
	m_mouseCursor->GetTransformComp().SetPosition({ (float)m_mouseX - 5,(float)m_mouseY - 5 });//this is the mouse cursor
}

void BattleUI::render()
{
	m_mouseCursor->Render(SCREEN_SURFACE);
}
