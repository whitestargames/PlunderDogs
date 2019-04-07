#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"

OverWorldWindow OverWorld::CURRENT_WINDOW = OverWorldWindow::LevelSelection;

OverWorld::OverWorld()
	: m_overWorldGUI(std::make_unique<OverWorldGUI>()),
	m_battle()
{
	for(int i = 0; i < 20; ++i)
	{
		m_entityVector.push_back({});
	}
	/*for (int i = 0; i < 20; i++)
	{
		Entity newEntity(Utilities::getDataDirectory() + "thingy.xml", 5, i + 1, i + 2, i + 3);
		m_entityVector.push_back(newEntity);
	}*/
}

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
