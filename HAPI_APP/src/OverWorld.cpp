#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"

OverWorldWindow OverWorld::CURRENT_WINDOW = OverWorldWindow::LevelSelection;

constexpr int WINDOW_OBJECTWIDTH = 75;
constexpr int WINDOW_OBJECTHEIGHT = 150;
constexpr int WINDOW_WIDTH = 830;
constexpr int WINDOW_HEIGHT = 200;

OverWorld::OverWorld()
	: m_overWorldGUI(std::make_unique<OverWorldGUI>()),
	m_battle()
{}

void OverWorld::render()
{
	m_overWorldGUI->render();

	if (CURRENT_WINDOW == OverWorldWindow::Battle)
	{
		m_battle.render();
	}
}

void OverWorld::update(float deltaTime)
{
	switch (CURRENT_WINDOW)
	{
		case OverWorldWindow::Battle:
		{
			m_battle.update(deltaTime);
			break;
		}
	}
}

void OverWorld::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		m_overWorldGUI->onLeftClick(mouseData);
	}
}

void OverWorld::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	m_overWorldGUI->onMouseMove(mouseData);
}
