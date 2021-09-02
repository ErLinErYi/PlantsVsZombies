/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Email: 2117610943@qq.com
 */

#include "Based/LZDialog.h"
#include "Based/LZPlayMusic.h"

Dialog::Dialog():
	_shieldListener(nullptr),
	_mouseListener(nullptr),
	_dialog(nullptr),
	_phasePosition(Vec2::ZERO),
	_global(Global::getInstance())
{
}

Dialog::~Dialog()
{
}

bool Dialog::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	createDialog();
	
	return true;
}

void Dialog::createDialog()
{
	_dialog = Sprite::createWithSpriteFrameName("LevelObjiectives.png");
	_dialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_dialog->setScale(1.5f);
	this->addChild(_dialog);

	/* 创建触摸监听 */
	createTouchtListener(_dialog);
}

void Dialog::createButtons(string name, float position, int id)
{
	auto button = Button::create("ButtonNew.png", "ButtonNew2.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	auto label = Label::createWithTTF(_global->userInformation->getGameText().find(name)->second->text,
		GAME_FONT_NAME_1, _global->userInformation->getGameText().find(name)->second->fontsize);
	label->enableShadow(Color4B(0, 0, 0, 200));//设置阴影
	label->setScale(3.5f);
	button->setTitleLabel(label);
	button->setPosition(Vec2(position, 10));
	button->setScale(0.25f);
	_dialog->addChild(button);

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::ENDED:
				PlayMusic::playMusic("gravebutton");
				if (id == 0)
				{
					selectButtonCallBack(true);
				}
				else
				{
					selectButtonCallBack(false);
					deleteDialog();
				}
				break;
			}
		});
}

void Dialog::getData(const std::function<void(bool flag)>& pSelector)
{
	selectButtonCallBack = pSelector;
}

void Dialog::createText()
{
	auto str = _global->userInformation->getGameText().find("确认操作")->second;
	auto information = Text::create(str->text, GAME_FONT_NAME_1, str->fontsize);
	information->setColor(Color3B::RED);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(_dialog->getContentSize().width - 90, 70));
	information->setPosition(Vec2(_dialog->getContentSize().width / 2.0f, _dialog->getContentSize().height / 2.0f + 80));
	_dialog->addChild(information);

	auto information1 = Text::create(_strText, GAME_FONT_NAME_1, _global->userInformation->getGameText().find("确认说明")->second->fontsize);
	information1->setColor(Color3B::RED);
	information1->setPosition(Vec2(_dialog->getContentSize().width / 2.0f, _dialog->getContentSize().height / 2.0f));
	_dialog->addChild(information1);
}

void Dialog::setString(string str)
{
	_strText = str;

	createText();
}

void Dialog::setMouseListener(EventListenerMouse* listener)
{
	_mouseListener = listener;
}

void Dialog::setMouseListenerEnable(bool isEnable)
{
	if (_mouseListener)
	{
		_mouseListener->setEnabled(isEnable);
	}
}

EventListenerTouchOneByOne* Dialog::createTouchtListener(Sprite* sprite)
{
	static Size windowSize = Director::getInstance()->getWinSize();
	static Vec2 phasePosition = Vec2(Vec2::ZERO);
	static Vec2 lrDistance,udDistance;
	const Size dialogSize(sprite->getBoundingBox().size / 2);

	/* 创建触摸监听 */
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *t, Event *e) {
		if (sprite->getBoundingBox().containsPoint(t->getLocation()))
		{
			phasePosition = t->getLocation() - sprite->getPosition();
			lrDistance.x = t->getLocation().x - sprite->getBoundingBox().getMinX();
			lrDistance.y = sprite->getBoundingBox().getMaxX() - t->getLocation().x;
			udDistance.x = t->getLocation().y - sprite->getBoundingBox().getMinY();
			udDistance.y = sprite->getBoundingBox().getMaxY() - t->getLocation().y;
			return true;
		}
		else return false;
	};
	listener->onTouchMoved = [=](Touch *t, Event *e) {
		sprite->setPosition(Vec2(
			t->getLocation().x - lrDistance.x < 0 ? dialogSize.width :
			t->getLocation().x + lrDistance.y > windowSize.width ? windowSize.width - dialogSize.width : (t->getLocation() - phasePosition).x,
			t->getLocation().y - udDistance.x < 0 ? dialogSize.height :
			t->getLocation().y + udDistance.y > windowSize.height ? windowSize.height - dialogSize.height : (t->getLocation() - phasePosition).y));
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sprite);

	return listener;
}

void Dialog::showDialogAction(Sprite* sprite)
{
	if (sprite)
	{
		sprite->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 3000));
		sprite->runAction(EaseBounceOut::create(MoveTo::create(0.5f, Director::getInstance()->getWinSize() / 2.0f)));
	}
}

void Dialog::showDeleteDialogAction(Sprite* sprite)
{
	if (sprite)
	{
		sprite->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.3f, 0.f),
			CallFunc::create([=]() {
				sprite->getParent()->runAction(FadeOut::create(0.3f));
				}), nullptr),
			CallFunc::create([=]()
				{
					sprite->getParent()->removeFromParent();
				}), nullptr));
	}
}

Label* Dialog::label(const std::string &name, const float& fontsize, const Vec2 &vec2, const float& space, const Color3B& color, const float& scale)
{
	auto label = Label::createWithTTF(name, GAME_FONT_NAME_1, fontsize);
	label->setScaleX(scale);
	label->setColor(color);
	label->setAdditionalKerning(space);//设置列间距
	label->enableShadow(Color4B(100, 20, 100, 255));//设置阴影
	label->setPosition(vec2);
	return label;
}

void Dialog::createLabel(Sprite* sprite, MenuItemImage* MenuItem, const std::string &name, const Vec2 &vec2, float& fontsize, const float& space, const Color3B& color)
{
	auto label = Label::createWithTTF(name, GAME_FONT_NAME_1, fontsize);
	label->setPosition(vec2);
	label->setColor(color);
	label->enableShadow(Color4B(100, 25, 100, 255));//设置阴影
	label->setAdditionalKerning(space);//设置列间距
	MenuItem->addChild(label);

	auto Buttons = Menu::create(MenuItem, NULL);
	Buttons->setPosition(Vec2(0, 0));
	sprite->addChild(Buttons);
}

void Dialog::createShieldLayer(Node* node)
{
	// set shieldLayer
	_shieldListener = EventListenerTouchOneByOne::create();
	_shieldListener->onTouchBegan = [](Touch* touch, Event* event)-> bool { return true; };
	_shieldListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_shieldListener, node);
}

void Dialog::deleteDialog()
{
	this->removeFromParent();
}
