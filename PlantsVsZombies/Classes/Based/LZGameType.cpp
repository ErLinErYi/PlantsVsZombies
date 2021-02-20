#include "Based/LZGameType.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSDefine.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"

SunNumberRequriement::SunNumberRequriement():
	allSunNumbersText(nullptr)
,	atLeastSunNumbers(0)
,	allSunNumbers(0)
,	isHaveSunNumberRequriement(false)
,	isUpdateImage(false)
{
}

PlantsRequriement::PlantsRequriement() :
	palntsNumbersText(nullptr)
,	userPlantsNumbers(-1)
,	surPlusPlantsNumbers(-1)
,	isHavePlantsRequriement(false)
,	isUpdateImage(false)
{
}

GameType::GameType(Node* node):
	_node(node)
,	_global(Global::getInstance())
,	_openlevelData(OpenLevelData::getInstance())
,	_sunNumberRequriement(new SunNumberRequriement)
,	_plantsRequriement(new PlantsRequriement)
{
}

GameType::~GameType()
{
	if(_sunNumberRequriement) delete _sunNumberRequriement;
	if( _plantsRequriement)   delete _plantsRequriement;
}

void GameType::createGameType()
{
	auto _levelData = _openlevelData->readLevelData(_openlevelData->getLevelNumber());
	for (unsigned int i = 0; i < _levelData->getGameType().size(); ++i)
	{
		switch (static_cast<GameTypes>(_levelData->getGameType().at(i)))
		{
		case GameTypes::AtLeastSunNumbers:
		{
			auto ShowNumber = Sprite::createWithSpriteFrameName("ShowNumber.png");
			ShowNumber->setPosition(Vec2(690, 1025));
			ShowNumber->setScaleX(1.2f);
			_node->addChild(ShowNumber);

			auto LackSun = Sprite::createWithSpriteFrameName("LackSun.png");
			LackSun->setPosition(Vec2(590, 1025));
			LackSun->setName("LackSun");
			_node->addChild(LackSun);

			_sunNumberRequriement->isHaveSunNumberRequriement = true;
			showNumbers(1);
		}
			break;
		case GameTypes::UserPlantsNumbers:
		{
			auto ShowNumber = Sprite::createWithSpriteFrameName("ShowNumber.png");
			ShowNumber->setPosition(Vec2(735, 1025));
			_node->addChild(ShowNumber);

			auto PlantsNumbers = Sprite::createWithSpriteFrameName("MorePlants.png");
			PlantsNumbers->setPosition(Vec2(640, 1025));
			PlantsNumbers->setName("MorePlants");
			_node->addChild(PlantsNumbers);

			_plantsRequriement->isHavePlantsRequriement = true;
			showNumbers(2);
		}
		    break;
		case GameTypes::FlowerPosition:
		{
			auto Flowers = Sprite::createWithSpriteFrameName("Flowers.png");
			Flowers->setPosition(Vec2(_levelData->getFlowerPosition(), 600));
			Flowers->setScaleY(1.2f);
			backgroundLayerInformation->addChild(Flowers);
			auto Flowers1 = Sprite::createWithSpriteFrameName("Flowers.png");
			Flowers1->setPosition(Vec2(_levelData->getFlowerPosition(), 300));
			Flowers1->setScaleY(1.2f);
			backgroundLayerInformation->addChild(Flowers1);
		}
			break;
		case GameTypes::NoPlants:
		{
			for (auto sp : _levelData->getNoPlantsPosition())
			{
				auto impurity = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Impurity")->second);
				impurity->setPosition(Vec2(570 + 122 * sp.x + 60, 110 + 138 * (sp.y + 1) - 90));
				impurity->setAnimation(0, "animation", true);
				impurity->update(0);
				backgroundLayerInformation->addChild(impurity);
			}
		}
			break;
		default:
			break;
		}
	}
}

void GameType::updateRequirementNumbers(const std::string name)
{
	if (!strcmp(name.c_str(), "植物数量增加"))
	{
		/* 如果有植物数量要求，则自加一 */
		if (_plantsRequriement->isHavePlantsRequriement)
		{
			_plantsRequriement->surPlusPlantsNumbers++;
			updateNumbers(2);
		}
	}
	else if(!strcmp(name.c_str(), "植物数量减少"))
	{
		/* 如果有植物数量要求，则自减一 */
		if (_plantsRequriement->isHavePlantsRequriement)
		{
			_plantsRequriement->surPlusPlantsNumbers--;
			updateNumbers(2);
		}
	}
	else if (!strcmp(name.c_str(), "阳光数量增加"))
	{
		if (_sunNumberRequriement->isHaveSunNumberRequriement)
		{
			_sunNumberRequriement->allSunNumbers += 50;
			updateNumbers(1);
		}
	}
	else if(!strcmp(name.c_str(), "阳光数量减少"))
	{
		if (_sunNumberRequriement->isHaveSunNumberRequriement)
		{
			_sunNumberRequriement->allSunNumbers -= 50;
			updateNumbers(1);
		}
	}
}

void GameType::updateRequirementNumbers()
{
	/* 如果有植物数量要求 */
	if (_plantsRequriement->isHavePlantsRequriement)
	{
		updateNumbers(2);
	}

	/* 如果有阳光数量要求 */
	if (_sunNumberRequriement->isHaveSunNumberRequriement)
	{
		updateNumbers(1);
	}
}

void GameType::showNumbers(const int& ID)
{
	switch(ID)
	{
	case 1:
	{
		_sunNumberRequriement->allSunNumbersText = Text::create();
		_sunNumberRequriement->allSunNumbersText->setFontSize(30);
		_sunNumberRequriement->allSunNumbersText->setFontName(GAME_FONT_NAME_2);
		_sunNumberRequriement->allSunNumbersText->setPosition(Vec2(700, 1025));
		_sunNumberRequriement->allSunNumbersText->setTextAreaSize(Size(150, 25));
		_sunNumberRequriement->allSunNumbersText->setOverflow(Overflow::SHRINK);
		_sunNumberRequriement->allSunNumbersText->setTextHorizontalAlignment(TextHAlignment::CENTER);
		_sunNumberRequriement->allSunNumbersText->setTextVerticalAlignment(TextVAlignment::CENTER);
		_sunNumberRequriement->allSunNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
		_sunNumberRequriement->allSunNumbersText->setColor(Color3B::YELLOW);
		_sunNumberRequriement->atLeastSunNumbers = _openlevelData->readLevelData(_openlevelData->getLevelNumber())->getAtLeastSunNumbers();
		updateNumbers(ID);
		_node->addChild(_sunNumberRequriement->allSunNumbersText);
	}
		break;
	case 2:
	{
		_plantsRequriement->palntsNumbersText= Text::create();
		_plantsRequriement->palntsNumbersText->setFontSize(30);
		_plantsRequriement->palntsNumbersText->setFontName(GAME_FONT_NAME_2);
		_plantsRequriement->palntsNumbersText->setPosition(Vec2(737, 1025));
		_plantsRequriement->palntsNumbersText->setTextAreaSize(Size(150, 25));
		_plantsRequriement->palntsNumbersText->setOverflow(Overflow::SHRINK);
		_plantsRequriement->palntsNumbersText->setTextHorizontalAlignment(TextHAlignment::CENTER);
		_plantsRequriement->palntsNumbersText->setTextVerticalAlignment(TextVAlignment::CENTER);
		_plantsRequriement->palntsNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
		_plantsRequriement->palntsNumbersText->setColor(Color3B::YELLOW);
		_plantsRequriement->surPlusPlantsNumbers = _openlevelData->readLevelData(_openlevelData->getLevelNumber())->getUsePlantsNumbers();
		_plantsRequriement->userPlantsNumbers = _openlevelData->readLevelData(_openlevelData->getLevelNumber())->getUsePlantsNumbers();
		updateNumbers(ID);
		_node->addChild(_plantsRequriement->palntsNumbersText);
	}
		break;
	default:
		break;
	}
}

void GameType::updateNumbers(const int& ID)
{
	switch (ID)
	{
	case 1:
	{
		_sunNumberRequriement->allSunNumbersText->setString(
			to_string(_sunNumberRequriement->atLeastSunNumbers) + "/" + to_string(_sunNumberRequriement->allSunNumbers));
		_sunNumberRequriement->allSunNumbersText->setTextAreaSize(Size(160, 25));

		if (!_sunNumberRequriement->isUpdateImage && _sunNumberRequriement->allSunNumbers >=
			_openlevelData->readLevelData(_openlevelData->getLevelNumber())->getAtLeastSunNumbers())
		{
			auto sprite = static_cast<Sprite*>(_node->getChildByName("LackSun"));
			sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("MoreSun.png"));
			
			_sunNumberRequriement->isUpdateImage = true;
		}
	}
		break;
	case 2:
	{
		_plantsRequriement->palntsNumbersText->setString(
			to_string(_plantsRequriement->userPlantsNumbers) + " / " + to_string(_plantsRequriement->surPlusPlantsNumbers));
		_plantsRequriement->palntsNumbersText->setTextAreaSize(Size(150, 25));

		if (!_plantsRequriement->isUpdateImage && _plantsRequriement->surPlusPlantsNumbers <= 0)
		{
			auto sprite = static_cast<Sprite*>(_node->getChildByName("MorePlants"));
			sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("LackPlants.png"));
			
			_sunNumberRequriement->isUpdateImage = true;
		}
	}
		break;
	default:
		break;
	}	
}

void GameType::waringPlantsNull()
{
	auto action = TintTo::create(0.1f, Color3B::RED);
	auto action1 = TintTo::create(0.1f, Color3B::YELLOW);
	_plantsRequriement->palntsNumbersText->runAction(Repeat::create(Sequence::create(action, action1, CallFunc::create([=]()
		{
			_plantsRequriement->palntsNumbersText->setColor(Color3B::YELLOW);
		}), nullptr), 5));
}

SunNumberRequriement* GameType::getSunNumberRequriement() const
{
	return _sunNumberRequriement;
}

PlantsRequriement* GameType::getPlantsRequriement() const
{
	return _plantsRequriement;
}
