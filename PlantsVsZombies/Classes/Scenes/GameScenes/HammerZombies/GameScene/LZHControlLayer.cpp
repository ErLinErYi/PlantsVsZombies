/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHControlLayer.h"
#include "LZHGameEndLayer.h"
#include "LZHButtonLayer.h"
#include "LZHAnimationLayer.h"
#include "LZHInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Zombies/LZZombies.h"
#include "Plants/EmissionPlants/Bullet/LZBullet.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"
#include "Based/LZMouseEventControl.h"

HControlLayer::HControlLayer() :
	_hammer(nullptr),
	_attack(100),
	_frequencyZombiesNumbers(10),
	_maxFrequencyNumbers(3),
	_zombiesTypeNumbers(1),
	_currentLevelZombiesSpeed(2.f),
	_isShowHammerButton(false),
	_isHammerCheat(false),
	_levelFinished(false),
	_hammerNumbers(0)
{
	srand(time(nullptr));
	onSetMostLevel(1);
	onSetCurrentLevel(1);
	createHammerAnimation();
	createMouseListener();
	calculateZombiesData();
}

HControlLayer::~HControlLayer()
{
}

bool HControlLayer::init()
{
	if (!Layer::init())return false;

	initData();
	createSchedule();
	
	return true;
}

void HControlLayer::initData()
{
	srand(time(nullptr));
	gameMapInformation = new GameMapInformation();
	_zombiesAppearControl = new ZombiesAppearControl();
}

void HControlLayer::createSchedule()
{
	schedule([&](float) {
		createZombies();
		controlRefurbishMusicAndText();
		}, 0.1f, "mainUpdate");

	schedule([&](float) {
		zombiesComeTiming();
		judgeZombiesWin();
		judgeLevelIsFinished();
		}, 1.0f, "zombiesComing");

	schedule([&](float) {
		if (_hammerNumbers >= 40)
		{
			_isHammerCheat = true;
		}
		else
		{
			_hammerNumbers = 0;
		}
		}, 5.0f, "hummerNumbers");

	runAction(Sequence::create(DelayTime::create(1.f), 
		CallFunc::create([=]()
			{
				MouseEventControl::goodsRecovery(goodsLayerInformation, _hammer);
			}), nullptr));
}

void HControlLayer::createHammerAnimation()
{
	_hammer = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Hammer")->second);
	_hammer->setAnimation(0, "Hammer_Normal", false);
	_hammer->setSkin("skin" + to_string(HButtonLayer::selectedHammer));
	_hammer->setScale(1.5f);
	_hammer->setPosition(Vec2(-100,-100));
	_hammer->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_hammer->setGlobalZOrder(2);
	_hammer->update(0);
	this->addChild(_hammer);
}

void HControlLayer::updateHammerInformation()
{
	_hammer->setSkin("skin" + to_string(HButtonLayer::selectedHammer));
	_attack = hammerInformation[HButtonLayer::selectedHammer].lowAttack + min(hammerInformation[HButtonLayer::selectedHammer].HigtAttack,
		hammerInformation[HButtonLayer::selectedHammer].lowAttack + onGetCurrentLevel() * 15);
}

void HControlLayer::createMouseListener()
{
	/* 创建鼠标监听 */
	auto listener = EventListenerMouse::create();

	/* 鼠标移动 */
	listener->onMouseMove = [&](Event* event)
	{
		/* 获取鼠标位置 */
		_cur = ((EventMouse*)event)->getLocationInView();
		_hammer->setPosition(_cur - Vec2(20, 60));

		changeCursor();
		showSelectedButtonHoverEffect();
	};

	/* 鼠标按下 */
	listener->onMouseDown = [=](Event* event)
	{
		if (((EventMouse*)event)->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
		{
			++_hammerNumbers;
			_cur = ((EventMouse*)event)->getLocationInView();
			_hammer->setAnimation(0, "Hammer_Attack", false);
			judgeHammerZombies(_cur);

			if (_hammer->isVisible())
			{
				PlayMusic::playMusic("swing");
			}
		}
	};

	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HControlLayer::judgeHammerZombies(const Vec2& position)
{
	for (auto zombie : ZombiesGroup)
	{
		if (_isHammerCheat)
		{
			zombie->setZombieDieRewardCoinPrecent(0);
		}

		if (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && _hammer->isVisible() && !_isHammerCheat)
		{
			const auto& box = zombie->getZombieAnimation()->getBoundingBox();
			if (_hammer->getBoundingBox().intersectsRect(Rect(box.origin.x + 60, box.origin.y + 50, box.size.width, box.size.height - 50)))
			{
				PlayMusic::playMusic("bonk");
				Bullet::selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(), zombie->getZombieHeadAttackSoundEffect());

				zombie->setZombieHurtBlink();
				attackZombies(zombie);
				showPowImage(zombie, position);
				break;
			}
		}
	}
}

void HControlLayer::changeCursor()
{
	if (_cur.x > GRASS_POSITION_LEFT - 100 && _cur.y > GRASS_POSITION_BOTTOM &&
		_cur.x < GRASS_POSITION_RIGHT + 100 && _cur.y < GRASS_POSITION_TOP + 150)
	{
		_director->getOpenGLView()->setCursorVisible(false);
		_hammer->setVisible(true);
		hideHammerButton();
	}
	else
	{
		_director->getOpenGLView()->setCursorVisible(true);
		_hammer->setVisible(false);
		showHammerButton();
	}
}

void HControlLayer::showSelectedButtonHoverEffect()
{
	for (auto button : HButtonLayer::hammerButton)
	{
		if (button->isEnabled() && HButtonLayer::selectedHammer != button->getTag())
		{
			button->getChildByName("SeedPacketFlash")->
				setVisible(button->getBoundingBox().containsPoint(_cur));
		}

		if (button->getBoundingBox().containsPoint(_cur))
		{
			dynamic_cast<HInformationLayer*>(informationLayerInformation)->showHammerInformation(button);
		}
	}
}

void HControlLayer::attackZombies(Zombies* zombie)
{
	if (zombie->getZombieCurrentBodyShieldVolume() < _attack)     /* 如果当前身体护盾血量小于爆炸伤害 */
	{
		if (zombie->getZombieCurrentBodyShieldVolume() +
			zombie->getZombieCurrentHeadShieldVolume() < _attack) /* 如果当前身体护盾加头部护盾血量小于爆炸伤害 */
		{
			if (zombie->getZombieCurrentBodyShieldVolume() +
				zombie->getZombieCurrentHeadShieldVolume() +
				zombie->getZombieCurrentBloodVolume() <= _attack) /* 如果僵尸所有血量小于爆炸伤害（僵尸死亡） */
			{
				zombie->setZombieDeath(true);
			}
			else
			{
				/* 计算僵尸本体血量 */
				zombie->setZombieCurrentBloodVolume(
					zombie->getZombieCurrentBodyShieldVolume() +
					zombie->getZombieCurrentHeadShieldVolume() +
					zombie->getZombieCurrentBloodVolume() - _attack);
				zombie->setZombieCurrentHeadShieldVolume(0);
				zombie->setZombieCurrentBodyShieldVolume(0);
			}
		}
		else
		{
			/* 计算僵尸护盾剩于血量 */
			zombie->setZombieCurrentHeadShieldVolume(
				zombie->getZombieCurrentBodyShieldVolume() +
				zombie->getZombieCurrentHeadShieldVolume() - _attack);
			zombie->setZombieCurrentBodyShieldVolume(0);
		}
	}
	else
	{
		zombie->setZombieCurrentBodyShieldVolume(
			zombie->getZombieCurrentBodyShieldVolume() - _attack);
	}
}

void HControlLayer::createZombies()
{
	/* 刷新僵尸 */
	if (_zombiesAppearControl->getLastFrequencyZombiesWasDeath())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(false);
		_zombiesAppearControl->setTimeClear(); /* 距离上一波刷新时间清零 */
		if (static_cast<unsigned int>(_zombiesAppearControl->getZombiesAppearFrequency()) < _maxFrequencyNumbers)
		{
			unsigned int zombiesNumbers = _frequencyZombiesNumbers + _zombiesAppearControl->getZombiesAppearFrequency();
			zombiesNumbers = zombiesNumbers + rand() % (zombiesNumbers / 3);
			for (unsigned int i = 0; i < zombiesNumbers; ++i)
			{
				dynamic_cast<HAnimationLayer*>(animationLayerInformation)->createZombies(
					_currentLevelZombiesSpeed, _zombiesAppearControl->getZombiesAppearFrequency() * 0.01f, _zombiesTypeNumbers);
			}
			/* 进攻波数自增一 */
			_zombiesAppearControl->setZombiesAppearFrequency();

			/* 进度条更新 */
			informationLayerInformation->updateProgressBar(_zombiesAppearControl->getZombiesAppearFrequency(), _maxFrequencyNumbers);
		}
		ZombiesGroup.sort(Zombies::compare);
	}
	informationLayerInformation->updateProgressBarHead();

	/* 控制僵尸的刷新 */
	if (controlRefurbishZombies())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(true);
		_zombiesAppearControl->setIsBegin(false);
	}
}

bool HControlLayer::controlRefurbishZombies()
{
	if ((Zombies::getZombiesNumbers() <= 4 &&
		_zombiesAppearControl->getZombiesAppearFrequency() > 3)                    /* 如果活着的僵尸数小于规定，刷新下一波 */

		|| (Zombies::getZombiesNumbers() <= 0 &&                                   /* 如果没有存活僵尸则立即刷新僵尸 */
			_zombiesAppearControl->getZombiesAppearFrequency() > 0)

		|| (_zombiesAppearControl->getTime() >= 5 &&
			_zombiesAppearControl->getZombiesAppearFrequency() == 0)               /* 第一波刷新控制 */

		|| (_zombiesAppearControl->getTime() >= 10 &&
			_zombiesAppearControl->getZombiesAppearFrequency() > 0)                /* 如果大于10秒刷新下一波 */
		)
	{
		return true;
	}
	return false;
}

void HControlLayer::controlRefurbishMusicAndText()
{
	/* 控制大波僵尸来袭的文字与音乐 */
	if (_zombiesAppearControl->getTime() >= 5 && _zombiesAppearControl->getZombiesAppearFrequency() == 0)
	{
		PlayMusic::playMusic("awooga");
	}

	/* 最后一波僵尸的文字显示与旗子更新 */
	if (_zombiesAppearControl->getZombiesAppearFrequency() == _maxFrequencyNumbers && !_zombiesAppearControl->getIsShowWords())
	{
		if (informationLayerInformation->updateProgressBarFlag())
		{
			_zombiesAppearControl->setIsShowWords(true);
		}
	}
}

void HControlLayer::judgeLevelIsFinished()
{
	/* 关卡结束 */
	if (!_levelFinished && ZombiesGroup.size() <= 0 && static_cast<unsigned int>(_zombiesAppearControl->getZombiesAppearFrequency()) >= _maxFrequencyNumbers)
	{
		_levelFinished = true;
		_hammer->setVisible(false);
		UserData::getInstance()->caveUserData(const_cast<char*>("HAMMERZOMBIES_LEVEL_NUMBER"), static_cast<int>(onGetCurrentLevel() + 1));
		UserData::getInstance()->caveUserData(const_cast<char*>("MOST_HAMMERZOMBIES_LEVEL_NUMBER"), static_cast<int>(max(onGetCurrentLevel() + 1, onGetMostLevel())));

		auto gameEndShieldLayer = HGameEndLayer::create();
		_director->getRunningScene()->addChild(gameEndShieldLayer, 10, "gameEndShieldLayer");
		gameEndShieldLayer->successfullEntry();
	}
}

void HControlLayer::judgeZombiesWin()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_levelFinished && zombie->getZombieIsSurvive() && zombie->getZombiePositionX() < GRASS_POSITION_LEFT - 60)
		{
			_levelFinished = true;
			_hammer->setVisible(false);
			auto gameEndShieldLayer = HGameEndLayer::create();
			_director->getRunningScene()->addChild(gameEndShieldLayer, 10, "gameEndShieldLayer");
			gameEndShieldLayer->breakThrough();
		}
	}
}

void HControlLayer::showPowImage(Zombies* zombie, const Vec2& position)
{
	auto pow = Sprite::createWithSpriteFrameName("Pow.png");
	pow->setPosition(position - Vec2(60, 40));
	pow->setOpacity(0);
	pow->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + 1);
	pow->runAction(Sequence::create(FadeIn::create(0.1f), DelayTime::create(0.3f), FadeOut::create(0.2f),
		CallFunc::create([pow]()
			{
				pow->removeFromParent();
			}), nullptr));
	animationLayerInformation->addChild(pow);
}

void HControlLayer::calculateZombiesData()
{
	onSetCurrentLevel(max(UserData::getInstance()->openIntUserData(const_cast<char*>("HAMMERZOMBIES_LEVEL_NUMBER")), 1));
	onSetMostLevel(max(static_cast<int>(onGetCurrentLevel()), UserData::getInstance()->openIntUserData(const_cast<char*>("MOST_HAMMERZOMBIES_LEVEL_NUMBER"))));
	_maxFrequencyNumbers += min(static_cast<int>(onGetCurrentLevel() / 20), 3);
	_zombiesTypeNumbers = min(static_cast<int>(_zombiesTypeNumbers + onGetCurrentLevel() / 3), ZOMBIESNUMBERS);
	_currentLevelZombiesSpeed += onGetCurrentLevel() * 0.02f;
	_attack = hammerInformation[HButtonLayer::selectedHammer].lowAttack + min(hammerInformation[HButtonLayer::selectedHammer].HigtAttack,
		hammerInformation[HButtonLayer::selectedHammer].lowAttack + onGetCurrentLevel() * 15);
}

void HControlLayer::showHammerButton()
{
	if (!_isShowHammerButton)
	{
		for (auto button : HButtonLayer::hammerButton)
		{
			button->stopAllActions();
			button->runAction(Sequence::create(MoveTo::create(0.1f, Vec2(90, 840 - 100 * button->getTag())), nullptr));
		}
		_isShowHammerButton = true;
	}
}

void HControlLayer::hideHammerButton()
{
	if (_isShowHammerButton)
	{
		for (auto button : HButtonLayer::hammerButton)
		{
			button->stopAllActions();
			button->runAction(Sequence::create(MoveTo::create(0.1f, Vec2(-90, 840 - 100 * button->getTag())), nullptr));
		}
		dynamic_cast<HInformationLayer*>(informationLayerInformation)->deleteHammerInformation();
		_isShowHammerButton = false;
	}
}

void HControlLayer::onSetMostLevel(int level)
{
	_mostLevelNumber = level;

	_encryptMKey = rand();
	_mostLevelNumber ^= _encryptMKey;
}

void HControlLayer::onSetCurrentLevel(int level)
{
	_currentLevelNumber = level;

	_encryptCKey = rand();
	_currentLevelNumber ^= _encryptCKey;
}

unsigned int HControlLayer::onGetMostLevel()
{
	return _mostLevelNumber ^ _encryptMKey;
}

unsigned int HControlLayer::onGetCurrentLevel()
{
	return _currentLevelNumber ^ _encryptCKey;
}
