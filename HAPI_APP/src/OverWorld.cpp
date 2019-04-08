#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"

OverWorld::OverWorld()
	: m_overWorldGUI(std::make_unique<OverWorldGUI>(*this)),
	m_battle()
{
	for(int i = 0; i < 20; ++i)
	{
		m_entities.push_back({});
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
}

void OverWorld::update(float deltaTime)
{
	if (OverWorldGUI::CURRENT_WINDOW == OverWorldWindow::eBattle)
	{
		m_battle.update(deltaTime);
	}
}
