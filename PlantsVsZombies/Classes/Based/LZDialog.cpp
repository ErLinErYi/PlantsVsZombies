#include "Based/LZDialog.h"

Dialog::Dialog():
	_shieldListener(nullptr),
	_mouseListener(nullptr),
	_phasePosition(Vec2::ZERO),
	_global(Global::getInstance())
{
}

Dialog::~Dialog()
{
}

void Dialog::setMouseListener(EventListenerMouse* listener)
{
	_mouseListener = listener;
}

void Dialog::setMouseListenerEnable(bool isEnable)
{
	_mouseListener->setEnabled(isEnable);
}

EventListenerTouchOneByOne* Dialog::createTouchtListener(Sprite* sprite)
{
	static Size windowSize = Director::getInstance()->getWinSize();
	static Vec2 phasePosition = Vec2(Vec2::ZERO);
	static Vec2 lrDistance,udDistance;
	Size dialogSize(sprite->getBoundingBox().size / 2);

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

Label* Dialog::label(const std::string &name, const float& fontsize, Vec2 &vec2, const float& space, const Color3B& color, const float& scale)
{
	auto label = Label::createWithTTF(name, GAME_FONT_NAME_1, fontsize);
	label->setScaleX(scale);
	label->setColor(color);
	label->setAdditionalKerning(space);//设置列间距
	label->enableShadow(Color4B(100, 20, 100, 255));//设置阴影
	label->setPosition(vec2);
	return label;
}

void Dialog::createLabel(Sprite* sprite, MenuItemImage* MenuItem, const std::string &name, Vec2 &vec2, float& fontsize, const float& space, const Color3B& color)
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
