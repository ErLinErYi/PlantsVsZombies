#include "Based/LZLevelData.h"
#include "Based/LZUserWinRequirement.h"
#include "Based/LZGameType.h"
#include "Based/LZDialog.h"

#include "Scenes/GameScenes/Adventure/WorldScene/LZModernWorld.h"

UserWinRequirement::UserWinRequirement(Node* node):
	_node(node),
	_listener(nullptr),
	_shieldListener(nullptr),
	_levelObjiectives(nullptr),
	_global(Global::getInstance())
{
}

UserWinRequirement::~UserWinRequirement()
{
}

void UserWinRequirement::createShieldLayer()
{
	// set shieldLayer
	_shieldListener = EventListenerTouchOneByOne::create();
	_shieldListener->onTouchBegan = [](Touch* touch, Event* event)-> bool { return true; };
	_shieldListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_shieldListener, _node);
}

void UserWinRequirement::createDialogBox(GameTypes finishedid)
{
	createShieldLayer();

	_levelObjiectives = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("LevelObjiectives.png");
	_levelObjiectives->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	_levelObjiectives->setScale(2.0f);
	_node->addChild(_levelObjiectives);

	auto LevelObjiectivesText = Text::create();
	LevelObjiectivesText->setString(_global->userInformation->getGameText().find("通关要求！")->second->text);
	LevelObjiectivesText->setFontName(GAME_FONT_NAME_1);
	LevelObjiectivesText->setFontSize(_global->userInformation->getGameText().find("通关要求！")->second->fontsize);
	LevelObjiectivesText->setScale(0.5f);
	LevelObjiectivesText->setColor(Color3B(0, 255, 255));
	LevelObjiectivesText->setPosition(Vec2(_levelObjiectives->getContentSize().width / 2, 245));
	_levelObjiectives->addChild(LevelObjiectivesText);

	/* 显示要求 */
	showRequirement(finishedid);

	_listener = Dialog::createTouchtListener(_levelObjiectives);
}

void UserWinRequirement::setParent(Node* node)
{
	_node = node;
}

void UserWinRequirement::setButtonVisible(bool visible)
{
	_continuesGame->setVisible(visible); 
}

void UserWinRequirement::setDialogOpacity(const int opacity)
{
	_levelObjiectives->setOpacity(opacity);
}

void UserWinRequirement::setListenerEnable(bool enable)
{
	_listener->setEnabled(enable);
}

void UserWinRequirement::setShowDialogAction()
{
	if (_levelObjiectives)
	{
		_levelObjiectives->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 3000));
		_levelObjiectives->runAction(EaseBounceOut::create(MoveTo::create(0.5f, Director::getInstance()->getWinSize() / 2.0f)));
	}
}

void UserWinRequirement::setDelectDialogAction()
{
	if (_levelObjiectives)
	{
		setListenerEnable(false);
		_levelObjiectives->runAction(MoveBy::create(0.2f, Vec2(0, -1000)));
	}
}

Sprite* UserWinRequirement::getDialog() const
{
	return _levelObjiectives;
}

void UserWinRequirement::showRequirement(GameTypes finishedid)
{
	auto leveldata = OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber());
	for (unsigned int i = 0; i < leveldata->getGameType().size(); i++)
	{
		switch (static_cast<GameTypes>(leveldata->getGameType().at(i)))
		{
		case GameTypes::CreateWall:
			showText(_global->userInformation->getGameText().find("建立你的防线，阻止僵尸的进攻！")->second->text, i);
			break;
		case GameTypes::AtLeastSunNumbers:
		{
			auto buff = StringUtils::format(_global->userInformation->getGameText().find("至少产生 %d 的阳光！")->second->text.c_str(), leveldata->getAtLeastSunNumbers());
			finishedid == GameTypes::AtLeastSunNumbers ? showText(buff, i, Color3B::RED) : showText(buff, i);
		}
			break;
		case GameTypes::FlowerPosition:
			finishedid == GameTypes::FlowerPosition ? showText(_global->userInformation->getGameText().find("僵尸踩坏了你的花坛！")->second->text, i, Color3B::RED) :
				showText(_global->userInformation->getGameText().find("不要让僵尸踩坏你的花坛！")->second->text, i);
			break;
		case GameTypes::CarNumbers:
		{
			auto buff = StringUtils::format(_global->userInformation->getGameText().find("僵尸进攻结束后至少存留 %d 辆小车！")->second->text.c_str(), leveldata->getCarNumbers());
			finishedid == GameTypes::CarNumbers ? showText(buff, i, Color3B::RED) : showText(buff, i);
		}
			break;
		case GameTypes::UserPlantsNumbers:
		{
			auto buff = StringUtils::format(_global->userInformation->getGameText().find("最多使用 %d 株植物来建立你的防线！")->second->text.c_str(), leveldata->getUsePlantsNumbers());
			showText(buff, i);
		}
			break;
		case GameTypes::ZombiesInvisible:
			showText(_global->userInformation->getGameText().find("阻止隐形的僵尸的进攻！")->second->text, i);
			break;
		case GameTypes::SmallZombies:
			showText(_global->userInformation->getGameText().find("小僵尸大麻烦！")->second->text, i);
			break;
		case GameTypes::BigZombies:
			showText(_global->userInformation->getGameText().find("抵御巨人僵尸的进攻！")->second->text, i);
			break;
		case GameTypes::NoPlants:
			showText(_global->userInformation->getGameText().find("请把植物种植在合适的地方！")->second->text, i);
			break;
		default:
			break;
		}
	}
}

void UserWinRequirement::showText(const string& text, const int& ID, Color3B color)
{
	auto requiretext = Label::createWithTTF(text, GAME_FONT_NAME_1, _global->userInformation->getGameText().find("小僵尸大麻烦！")->second->fontsize);
	requiretext->setColor(Color3B::BLACK);
	requiretext->setScale(0.5f);
	requiretext->setLineBreakWithoutSpace(true);
	requiretext->setWidth(790);
	requiretext->setPosition(_textPosition[OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber())->getGameType().size() - 1][ID]);
	requiretext->setAnchorPoint(Vec2(0, 1));
	requiretext->setColor(color);
	_levelObjiectives->addChild(requiretext);

	auto LevelObjiectives2 = Sprite::createWithSpriteFrameName("LevelObjiectives2.png");
	LevelObjiectives2->setPosition(_textPosition[OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber())->getGameType().size() - 1][ID] + Vec2(-40, +6));
	LevelObjiectives2->setAnchorPoint(Vec2(0, 1));
	LevelObjiectives2->setScale(0.5f);
	_levelObjiectives->addChild(LevelObjiectives2);
}