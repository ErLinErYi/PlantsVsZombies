/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "Zombies.h"
#include "Plants/Plants.h"

#include "Scenes/GameScene/GSInformationLayer.h"
#include "Scenes/GameScene/GSGameResultJudgement.h"
#include "Scenes/GameScene/GSGameEndLayer.h"
#include "Scenes/GameScene/GSData.h"

#include "Based/Coin.h"
#include "Based/UserData.h"
#include "Based/PlayMusic.h"

unsigned int Zombies::_zombiesNumbers = 0;
int Zombies::_zombiesNewDieNumbers = 0;
bool Zombies::_zombieIsWin = false;
GLProgram* Zombies::_normalGLProgram = nullptr;
GLProgram* Zombies::_highLightGLProgram = nullptr;
GSGameEndLayer* Zombies::_gameEndLayer = nullptr;
GSGameResultJudgement* Zombies::_gameResultJudgement = nullptr;

Zombies::Zombies() :
	_node(nullptr)
,   _attackHeadSoundEffectType(0)
,   _attackBodySoundEffectType(0)
,   _bodyAnimationId(1)
,   _bodyShieldAnimationId(1)
,   _headShieldAnimationId(1)
,	_isEat(false)
,	_isEatGarlic(false)
,	_isUseForGameType(false)
,	_isShow(false)
,	_isPreview(false)
,	_isHaveShield(false)
,   _isStrikeFly(false)
,   _redWarning(false)
,   _isShowLoseLimbsAnimation(true)
,   _isCanDelete{false,false}
,   _zombieEatPlantNumber(-1)
,   _zombieHowlNumbers(0)
,   _highLightIntensity(0.3f)
,   _highLightGLProgramState(nullptr)
,   _highLightFinished(false)
,   _headShieldType(ShieldType::none)
,   _bodyShieldType(ShieldType::none)
,	_openLevelData(OpenLevelData::getInstance())
,	_global(Global::getInstance())
,   _animationName{"Zombies_Stand","Zombies_Stand1","Zombies_Walk","Zombies_Walk2"}
{
	_random.seed(_device());
}

Zombies::~Zombies()
{
}

void Zombies::zombieInit(const string& animation_name)
{
	uniform_real_distribution<float>number(0.f, 0.45f);
	_zombiesAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find(animation_name)->second);
	_zombiesAnimation->setPosition(_position);
	_zombiesAnimation->setAnchorPoint(Vec2(0, 0));
	_zombiesAnimation->setTimeScale(0.6f + number(_random));
	_zombiesAnimation->setLocalZOrder(getZombieLocalZOrder(_position.y));
	_zombiesAnimation->setOpacity(0);   /* !!! 创建的僵尸自动隐身 */
	_node->addChild(_zombiesAnimation);

	setZombieGLProgram();
}

void Zombies::setZombieScale(const int& scale) const
{
	_zombiesAnimation->setScale(scale);
}

void Zombies::setZombieTimeScale(const float& timeScale) const
{
	_zombiesAnimation->setTimeScale(timeScale);
}

void Zombies::setZombieScale(const Size& scale) const
{
	_zombiesAnimation->setContentSize(scale);
}

void Zombies::setZombieAnimationInformation(const int tag, const bool isLoop) const
{
	_zombiesAnimation->setAnimation(0, _animationName[tag], isLoop);
}

void Zombies::setZombieAnimationName(const string& name, bool isLoop) const
{
	_zombiesAnimation->setAnimation(0, name, isLoop);
}

void Zombies::setZombieMove(float delta)
{
	_zombiesAnimation->setPositionX(_zombiesAnimation->getPositionX() - delta * _currentSpeed);

	if (getZombieIsEnterMap() && !getZombieIsShow())
	{
		setZombieIsShow(true);
		_zombiesAnimation->runAction(FadeIn::create(1.0f));
		_zombiesAnimation->getChildByName("shadow")->runAction(FadeIn::create(1.0f));
	}

	if (!_redWarning && getZombieWarning())
	{
		auto action = RepeatForever::create(
			Sequence::create(TintTo::create(0.3f, Color3B::RED),
				TintTo::create(0.3f, Color3B::WHITE), nullptr));
		action->setTag(10);
		_zombiesAnimation->runAction(action);
		_redWarning = true;
	}

	if (_redWarning && !getZombieWarning())
	{
		_zombiesAnimation->stopActionByTag(10);
		_redWarning = false;
	}
}

void Zombies::setZombieOpacity(GLubyte opacity)
{
	_zombiesAnimation->setOpacity(opacity);
	_zombiesAnimation->getChildByName("shadow")->setOpacity(opacity);
}

void Zombies::setZombiePosition(const Vec2& position)
{
	_position = position;
}

void Zombies::setZombieName(const string& name)
{
	_zombieName = name;
}

void Zombies::setZombieTag(const int& tag)
{
	_zombieTag = tag;
}

void Zombies::setZombieCurrentSpeed(const int currentSpeed)
{
	_currentSpeed = currentSpeed;
}

void Zombies::setZombieStop()
{
	_currentSpeed = 0;
}

void Zombies::setZombieIsShow(const bool isShow)
{
	_isShow = isShow;
}

void Zombies::setZombieIsEat(const bool isEat)
{
	_isEat = isEat;
}

void  Zombies::setZombieDeath(const bool death)
{
	if (death)
	{
		setZombieCurrentBloodVolume(0);
		setZombieCurrentBodyShieldVolume(0);
		setZombieCurrentHeadShieldVolume(0);
	}
}

void Zombies::setZombieVisible(const bool isVisible)
{
	_zombiesAnimation->setVisible(isVisible);
}

string Zombies::getZombieAniamtionName(ZombiesType zombiestype)
{
	string name;
	switch (zombiestype)
	{
	case ZombiesType::CommonDoorZombies:
	case ZombiesType::ConeDoorZombies:
	case ZombiesType::BucketDoorZombies: 
		     name = "Zombies_Door_Walk"; break;
	default: name = "Zombies_Walk"; break;
	}
	return name;
}

void Zombies::zombiesDeleteUpdate(list<Zombies*>::iterator& zombie)
{
	/* 删除死亡僵尸 */
	if (!(*zombie)->getZombieAnimation()->isVisible())
	{
		if (!(*zombie)->getIsCanDelete()[0])
		{
			(*zombie)->getIsCanDelete()[0] = true;
			++Global::getInstance()->userInformation->getKillZombiesNumbers();
			informationLayerInformation->updateZombiesDieNumbers(); /* 更新显示 */

			zombiesNumbersChange("--");  /* 僵尸总数更新 */
			rewardCoin((*zombie)->getZombieAnimation());

			if (++_zombiesNewDieNumbers >= 20 || getZombiesNumbers() <= 0)
			{
				_zombiesNewDieNumbers = 0;
				UserData::getInstance()->caveUserData("KILLALLZOMBIES", 
					Global::getInstance()->userInformation->getKillZombiesNumbers());/* 杀死僵尸数加一 */
			}

			auto zombies = zombie;
			(*zombies)->getZombieAnimation()->runAction(Sequence::create(DelayTime::create(5.0f),
				CallFunc::create([zombies]()
					{
						(*zombies)->getIsCanDelete()[1] = true;
					}), nullptr));
		}

		if ((*zombie)->getIsCanDelete()[1])
		{
			(*zombie)->getZombieAnimation()->pause();
			(*zombie)->getZombieAnimation()->stopAllActions();
			(*zombie)->getZombieAnimation()->removeFromParent();
			delete *zombie;
			*zombie = nullptr;
			ZombiesGroup.erase(zombie++);
		}
		else
		{
			++zombie;
		}
	}
	else
	{
		++zombie;
	}
}

void Zombies::rewardCoin(SkeletonAnimation* zombies)
{
	if (rand() % 100 < 5)
	{
		auto coin = new Coin(zombies->getParent());
		coin->setPosition(zombies->getPosition());
		coin->setCoinLocalZOrder(zombies->getLocalZOrder() + 100);
		coin->createCoin();

		CoinsGroup.push_back(coin);
	}
}

unsigned int Zombies::getZombiesNumbers()
{
	return _zombiesNumbers;
}

void Zombies::setZombiesNumbers(const unsigned int numbers)
{
	_zombiesNumbers = numbers;
}

bool Zombies::getZombieIsSurvive() const
{
	if (_currentBloodVolume > 0 && _zombiesAnimation->isVisible())
	{
		return true;
	}
	return false;
}

bool Zombies::getZombieIsVisible() const
{
	return _zombiesAnimation->isVisible() ? true : false;
}

string Zombies::getZombieName() const
{
	return _zombieName;
}

int Zombies::getZombieTag() const
{
	return _zombieTag;
}

int Zombies::getZombieEatPlantNumber() const
{
	return _zombieEatPlantNumber;
}

bool Zombies::getZombieIsPlayDieAnimation() const
{
	if (_bodyAnimationId != 10)
	{
		return false;
	}
	return true;
}

void Zombies::setZombieCurrentBodyShieldVolume(const float currentBodyShieldVolume)
{
	_currentBodyShieldVolume = currentBodyShieldVolume;
}

void Zombies::setZombieCurrentHeadShieldVolume(const float currentHeadShieldVolume)
{
	_currentHeadShieldVolume = currentHeadShieldVolume;
}

void Zombies::setZombieCurrentBloodVolume(const float currentBloodVolume)
{
	_currentBloodVolume = currentBloodVolume;
}

void Zombies::setZombieIsStrikeFly(const bool isStrikeFly)
{
	_isStrikeFly = isStrikeFly;
}

void Zombies::setZombieHurtBlink()
{
	if (!_highLightFinished)
	{
		_highLightFinished = true;
		auto action = Repeat::create(Sequence::create(
			CallFunc::create([this]()
				{
					_highLightIntensity -= 0.06f;
					_highLightGLProgramState->setUniformFloat("intensity", _highLightIntensity);
				}), DelayTime::create(0.02f), nullptr), 10);

		_zombiesAnimation->runAction(Sequence::create(
			CallFunc::create([this]()
				{
					_zombiesAnimation->setGLProgram(_highLightGLProgram);
					_highLightGLProgramState = _zombiesAnimation->getGLProgramState();
					_highLightGLProgramState->setUniformFloat("intensity", _highLightIntensity);
				}), DelayTime::create(0.15f),
			CallFunc::create([this]()
				{
					_zombiesAnimation->setGLProgram(_normalGLProgram);
					_highLightIntensity = 0.3f;
					_highLightFinished = false;
				}), nullptr));
	}
}

void Zombies::setZombieScale()
{
	_zombiesAnimation->setScale(_zombiesAnimation->getScale() + (getZombieLocalZOrder(_position.y) + 10) / 20 / 40.f);
}

SkeletonAnimation* Zombies::getZombieAnimation() const
{
	return _zombiesAnimation;
}

Vec2 Zombies::getZombiePosition() const
{
	return _zombiesAnimation->getPosition();
}

float Zombies::getZombiePositionX() const
{
	return _zombiesAnimation->getPositionX();
}

float Zombies::getZombiePositionY() const
{
	return _zombiesAnimation->getPositionY();
}

bool Zombies::getZombieIsEnterMap() const
{
	return _zombiesAnimation->getPositionX() < 1710 ? true : false;
}

float Zombies::getZombieCurrentBodyShieldVolume() const
{
	return _currentBodyShieldVolume;
}

float Zombies::getZombieCurrentHeadShieldVolume() const
{
	return _currentHeadShieldVolume;
}

float Zombies::getZombieCurrentBloodVolume() const
{
	return _currentBloodVolume;
}

bool Zombies::getZombieIsEat() const
{
	return _isEat;
}

bool Zombies::getZombieIsStop() const
{
	return _currentSpeed == 0 ? true : false;
}

float Zombies::getZombieCurrentSpeed() const
{
	return _currentSpeed;
}

float Zombies::getZombieSpeed() const
{
	return _speed;
}

bool Zombies::getZombieIsShow() const
{
	return _isShow;
}

ZombiesType Zombies::getZombieType() const
{
	return _zombiesType;
}

int Zombies::getZombieHeadAttackSoundEffect() const
{
	return _attackHeadSoundEffectType;
}

int Zombies::getZombieBodyAttackSoundEffect() const
{
	return _attackBodySoundEffectType;
}

bool* Zombies::getIsCanDelete()
{
	return _isCanDelete;
}

bool Zombies::getZombieIsHaveShield() const
{
	return _isHaveShield;
}

bool Zombies::getZombieIsStrikeFly() const
{
	return _isStrikeFly;
}

void Zombies::playZombiesDieAnimation()
{
	uniform_real_distribution<float>number(0.f, 0.4f);
	auto ashes = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Zombies_Ash")->second);
	ashes->setPosition(_zombiesAnimation->getPosition() + Vec2(0, -15));
	ashes->setLocalZOrder(_zombiesAnimation->getLocalZOrder());
	ashes->setScale(1.5f);
	ashes->setTimeScale(0.8f + number(_random));
	ashes->setAnimation(0, "animation", false);
	_node->addChild(ashes);

	ashes->setEventListener([ashes](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
				ashes->runAction(Sequence::create(FadeOut::create(1.0f), CallFunc::create([ashes]()
					{
						ashes->setVisible(false);
					}), nullptr));
			}
		});

	ashes->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([ashes]()
		{
			ashes->removeFromParent();
		}), nullptr));

	setZombieAttributeForGameType(ashes);
}

void Zombies::setZombieAttributeForGameType()
{
	auto data = _openLevelData->readLevelData(_openLevelData->getLevelNumber());
	/* 判断是否是小僵尸 */
	if (data->getZombiesIsSmall())
		setSmallZombieAttribute();

	/* 判断是否是巨人僵尸 */
	if (data->getZombiesIsBig())
		setBigZombieAttribute();

	/* 判断僵尸是否隐身 */
	if (data->getZombiesVisible())
		setOpacityZombieAttribute();
}

void Zombies::setZombieIsShowLoseLimbsAnimation(const bool isShow)
{
	_isShowLoseLimbsAnimation = isShow;
}

void Zombies::setZombieEatPlantNumber(const int& number)
{
	_zombieEatPlantNumber = number;
}

float Zombies::getZombieLocalZOrder(const int& positiionY) const
{
	const int pos[] = { 682,544,406,268,130 };
	for (int i = 0; i < 5; i++){
		if (pos[i] == positiionY){
			return (i + 1) * 20 - 10;
		}
	}
	return 0;
}

void Zombies::zombiesNumbersChange(const string& name)
{
	if (name == "++")
	{
		++_zombiesNumbers;
	}
	else
	{
		--_zombiesNumbers;
	}
}

void Zombies::judgeZombieWin(list<Zombies*>::iterator zombie)
{
	if (!_gameResultJudgement)
	{
		_gameResultJudgement = new GSGameResultJudgement();
	}

	auto zombieWin = _gameResultJudgement->judgeUserIsLose(zombie);
	if (zombieWin != GameTypes::None && !_zombieIsWin)
	{
		_zombieIsWin = true;
		_gameEndLayer = GSGameEndLayer::create();
		_gameEndLayer->breakThrough(zombieWin);
		Director::getInstance()->getRunningScene()->addChild(_gameEndLayer, 10);
	}
}

void Zombies::zombiesWinOrLoseInit()
{
	_zombieIsWin = false;
	if (_gameResultJudgement)
	{
		delete _gameResultJudgement;
		_gameResultJudgement = nullptr;
	}
}

void Zombies::createZombieShadow()
{
	/* 创建僵尸影子 */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setScale(2.0f);
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, 10));
	shadow->setOpacity(0);
	_zombiesAnimation->addChild(shadow, -1);
}

void Zombies::setZombiePrimaryInjure()
{
	if (_bodyAnimationId == 1) /* 僵尸开始掉胳膊 */
	{
		_zombiesAnimation->setAttachment("tt_outerarm_upper", "tt_outerarm_upper2");
		_zombiesAnimation->setAttachment("tt_outerarm_lower", "tt_innerleg_foot3");
		_zombiesAnimation->setAttachment("tt_outerarmhand", "tt_innerleg_foot3");
		_bodyAnimationId = 2;

		zombieLoseArmAnimation("ZombieArm");
	}
}

void Zombies::setZombieSecondaryInjure()
{
	if (_bodyAnimationId == 2)
	{
		_zombiesAnimation->setAnimation(1, "Zombies_Die", false);
		_zombiesAnimation->setAttachment("tt_head", "tt_innerleg_foot3");
		_zombiesAnimation->setAttachment("tt_jaw", "tt_innerleg_foot3");
		_bodyAnimationId = 10;

		zombieLoseHeadAnimation("ZombieHead");

		zombieFadeOutAnimation();
	}
}

void Zombies::setZombieBodyShieldPrimaryInjure(const string& oldName, const string& newName)
{
	if (_bodyShieldAnimationId == 1)
	{
		_zombiesAnimation->setAttachment(oldName, newName);
		_bodyShieldAnimationId = 2;
	}
}

void Zombies::setZombieBodyShieldSecondaryInjure(const string& oldName, const string& newName)
{
	if (_bodyShieldAnimationId == 2)
	{
		_zombiesAnimation->setAttachment(oldName, newName);
		_bodyShieldAnimationId = 3;
	}
}

void Zombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
{
	// 子类中实现
}

void Zombies::setZombieHeadShieldPrimaryInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 1)
	{
		_zombiesAnimation->setAttachment(oldName, newName);
		_headShieldAnimationId = 2;
	}
}

void Zombies::setZombieHeadShieldSecondaryInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 2)
	{
		_zombiesAnimation->setAttachment(oldName, newName);
		_headShieldAnimationId = 3;
	}
}

void Zombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	// 子类中实现
}

void Zombies::zombieLoseArmAnimation(const std::string& name)
{
	if (_isShowLoseLimbsAnimation)/* 掉胳膊 */
	{
		PlayMusic::playMusic("limbs_pop");
		
		auto arm = Sprite::createWithSpriteFrameName(name +".png");
		arm->setPosition(_zombiesAnimation->getPosition() + Vec2(-10, 100));
		arm->setLocalZOrder(_zombiesAnimation->getLocalZOrder() + 1);
		arm->runAction(Sequence::create(Spawn::create(
			Sequence::create(JumpBy::create(0.5f, Vec2(-80 + rand() % 160, -100), rand() % 80 + 66, 1), 
				JumpBy::create(0.2f, Vec2(-20 + rand() % 40, 0), rand() % 10 + 10, 1), nullptr),
			RotateBy::create(0.5f, rand() % 2 == 0 ? 90 : -90), nullptr),
			DelayTime::create(2.0f), FadeOut::create(0.5f), CallFunc::create([arm]() {arm->removeFromParent(); }), nullptr));
		_node->addChild(arm);

		showZombieShadow(arm, 100);
		setZombieAttributeForGameType(arm);
	}
}

void Zombies::zombieLoseHeadAnimation(const std::string& name)
{
	if (_isShowLoseLimbsAnimation)/* 僵尸掉头 */
	{
		PlayMusic::playMusic("limbs_pop");
		
		auto head = Sprite::createWithSpriteFrameName(name + ".png");
		head->setPosition(_zombiesAnimation->getPosition() + Vec2(-40, 150));
		head->setLocalZOrder(_zombiesAnimation->getLocalZOrder() + 1);
		head->setScale(1.5f);
		head->runAction(Sequence::create(Spawn::create(
			Sequence::create(JumpBy::create(0.5f, Vec2(-150 + rand() % 300, -120), rand() % 100 + 66, 1), 
				JumpBy::create(0.2f, Vec2(-20 + rand() % 40, 0), rand() % 10 + 10, 1), nullptr),
			RotateBy::create(0.5f, -180 + rand() % 360), nullptr),
			DelayTime::create(2.0f), FadeOut::create(0.5f), CallFunc::create([head]() {head->removeFromParent(); }), nullptr));
		_node->addChild(head);

		showZombieShadow(head, 150);
		setZombieAttributeForGameType(head);
	}
}

void Zombies::zombieLoseShieldAnimation(const std::string& name)
{
	/* 僵尸掉护盾 */
	PlayMusic::playMusic("limbs_pop");
	
	auto cone = Sprite::createWithSpriteFrameName(name + ".png");
	cone->setPosition(_zombiesAnimation->getPosition() + Vec2(-40, 200));
	cone->setLocalZOrder(_zombiesAnimation->getLocalZOrder() + 1);
	cone->setScale(1.5f);
	cone->runAction(Sequence::create(Spawn::create(
		Sequence::create(JumpBy::create(0.5f, Vec2(-150 + rand() % 300, -170), rand() % 100 + 66, 1), 
			JumpBy::create(0.2f, Vec2(-20 + rand() % 40, 0), rand() % 10 + 10, 1), nullptr),
		RotateBy::create(0.5f, -180 + rand() % 360), nullptr),
		DelayTime::create(2.0f), FadeOut::create(0.5f), CallFunc::create([cone]() {cone->removeFromParent(); }), nullptr));
	_node->addChild(cone);

	showZombieShadow(cone, 200);
	setZombieAttributeForGameType(cone);
}

void Zombies::zombieFadeOutAnimation()
{
	uniform_real_distribution<float>number(0.f, 0.45f);
	_zombiesAnimation->setTimeScale(0.6f + number(_random));
	_zombiesAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "filldown"))
			{
				_currentSpeed = 0; /* 停止运动 */
				PlayMusic::playMusic(rand() % 2 ? "zombie_falling_1" : "zombie_falling_2");
			}
			if (!strcmp(event->data->name, "die"))
			{
				_zombiesAnimation->getChildByName("shadow")->runAction(FadeOut::create(1.0f));
				_zombiesAnimation->runAction(Sequence::create(FadeOut::create(1.0f),
					CallFunc::create([this]()
						{
							_zombiesAnimation->setVisible(false);
						}), nullptr));
			}
		});
}

void Zombies::playZombieSoundEffect()
{
	const string music[6] = { "groan","groan2","groan3","groan4","groan5","groan6" };
	playZombieSoundEffect(music[rand() % 6]);
}

void Zombies::playZombieSoundEffect(const string& name)
{
	uniform_int_distribution<unsigned>number(0, 10000); 
	if (number(_random) < 5 && _zombieHowlNumbers< 3)
	{
		PlayMusic::playMusic(name);
		++_zombieHowlNumbers;
	}
}

ShieldType Zombies::getZombieBodyShieldType() const
{
	return _bodyShieldType;
}

ShieldType Zombies::getZombieHeadShieldType() const
{
	return _headShieldType;
}

void Zombies::showZombieShadow(Node* node, const int posy)
{
	/* 创建僵尸掉落肢体护盾影子 */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setScale(posy == 100 ? 1.0f : 1.5f);
	shadow->setOpacity(180);
	shadow->setLocalZOrder(node->getLocalZOrder());
	_node->addChild(shadow, -1);

	auto positiony = node->getPosition().y - posy;
	node->runAction(RepeatForever::create(Sequence::create(
		CallFunc::create([=]()
			{
				shadow->setPosition(Vec2(node->getPosition().x, positiony));
			}), DelayTime::create(0.01f), nullptr)));

	shadow->runAction(Sequence::create(
		DelayTime::create(2.7f), FadeOut::create(0.5f),DelayTime::create(1.0f),
		CallFunc::create([shadow]()
			{
				shadow->removeFromParent();
			}), nullptr));
	setZombieAttributeForGameType(shadow);
}

void Zombies::setSmallZombieAttribute()
{
	if (!_isUseForGameType)
	{
		/* 血量减半 */
		_bloodVolume /= 2.0f;
		_headShieldVolume /= 2.0f;
		_bodyShieldVolume /= 2.0f;
		_currentBloodVolume /= 2.0f;
		_currentHeadShieldVolume /= 2.0f;
		_currentBodyShieldVolume /= 2.0f;
		
		/* 速度增加 */
		_speed = 40;
		_currentSpeed = 40;

		/* 身体变小，动作变快 */
		_zombiesAnimation->setScale(0.5f);
		_zombiesAnimation->setTimeScale(_zombiesAnimation->getTimeScale() + 0.4f);

		_isUseForGameType = true;
	}
}

void Zombies::setBigZombieAttribute()
{
	if (!_isUseForGameType)
	{
		/* 血量增大 */
		_bloodVolume *= 2.0f;
		_headShieldVolume *= 2.0f;
		_bodyShieldVolume *= 2.0f;
		_currentBloodVolume *= 2.0f;
		_currentHeadShieldVolume *= 2.0f;
		_currentBodyShieldVolume *= 2.0f;
		
		/* 速度减少 */
		_speed = 20;
		_currentSpeed = 20;

		/* 身体变大，动作变慢 */
		_zombiesAnimation->setScale(1.3f);
		_zombiesAnimation->setTimeScale(_zombiesAnimation->getTimeScale() - 0.3f);

		_isUseForGameType = true;
	}
}

void Zombies::setZombieAttributeForGameType(Node* sprite)
{
	auto data = _openLevelData->readLevelData(_openLevelData->getLevelNumber());

	data->getZombiesIsSmall() ? sprite->setScale(0.7f) : data->getZombiesIsBig() ? sprite->setScale(1.8f) : nullptr;
}

void Zombies::setOpacityZombieAttribute()
{
	_zombiesAnimation->setOpacity(0);
	_isShow = true;
	_zombiesAnimation->getChildByName("shadow")->setOpacity(0);
}

void Zombies::setZombieGLProgram()
{
	if (!_normalGLProgram || !_highLightGLProgram)
	{
		_normalGLProgram = _zombiesAnimation->getGLProgram();
		_highLightGLProgram = getHighLight();
	}
}

bool Zombies::getZombieWarning()
{
	return _zombiesAnimation->getPositionX() < 570 + 122 * 2;
}

GLProgram* Zombies::getHighLight()
{
	auto program = GLProgramCache::getInstance()->getGLProgram("MyHighLightShader");
	if (!program)
	{
		program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,
			FileUtils::getInstance()->getStringFromFile("resources/Text/Bloom.fsh").c_str());
		program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
		program->link();
		program->updateUniforms();

		GLProgramCache::getInstance()->addGLProgram(program, "MyHighLightShader");
	}
	return program;
}

string& Zombies::createZombieName()
{
	char str[36]{""};
	snprintf(str, 36, "zombieName_%d", _zombieTag);
	_zombieName = str;
	return _zombieName;
}