#include "GSBackgroundLayer.h"
#include "GSData.h"

#include "Based/GameType.h"
#include "Zombies/CommonZombies.h"
#include "Based/LevelData.h"

GSBackgroundLayer::GSBackgroundLayer():
	_global(Global::getInstance()),
	gameType(nullptr)
{
}

void GSBackgroundLayer::backgroundRunAction()
{
	Vec2 begin[4] = { Vec2(2,2),Vec2(-2,2),Vec2(2,-2),Vec2(-2,-2) };
	auto move = MoveBy::create(0.1f, begin[rand() % 4]);
	backgroundLayerInformation->runAction(Sequence::create(move, move->reverse(), move, move->reverse(), nullptr));
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
#if VIRTUAL3D
	auto bg = Sprite::create("resources/timg.png");
	bg->setContentSize(Size(3325, 1080));
	bg->setAnchorPoint(Point(0, 0));
	bg->setPosition(Vec2(-455, 0));
	this->addChild(bg);
#else
	auto background = _global->userInformation->getBackground();
	if (background != nullptr)
	{
		background->setAnchorPoint(Point(0, 0));
		background->setContentSize(Size(2930, 1081));
		background->setPosition(Vec2(-220, 0));
		this->addChild(background);
    }
#endif
}

void GSBackgroundLayer::showGameType()
{
	gameType = new GameType(this);
	gameType->createGameType();
}
