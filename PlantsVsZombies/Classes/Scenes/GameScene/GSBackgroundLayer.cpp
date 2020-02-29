#include "GSBackgroundLayer.h"

#include "Based/GameType.h"
#include "Zombies/CommonZombies.h"
#include "Based/LevelData.h"

GSBackgroundLayer::GSBackgroundLayer():
	_global(Global::getInstance()),
	gameType(nullptr)
{
}

GSBackgroundLayer::~GSBackgroundLayer()
{
	delete gameType;
}

bool GSBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	createBackGroundEffect();

	setBackgroundImagePosition();

	showGameType();

	return true;
}

void GSBackgroundLayer::setBackgroundImagePosition()
{
	auto background = _global->userInformation->getBackground();
	if (background != nullptr)
	{
		background->setAnchorPoint(Point(0, 0));
		background->setContentSize(Size(2930, 1081));
		background->setPosition(Vec2(-220, 0));
		this->addChild(background);
	}
}

void GSBackgroundLayer::showGameType()
{
	gameType = new GameType(this);
	gameType->createGameType();
}
