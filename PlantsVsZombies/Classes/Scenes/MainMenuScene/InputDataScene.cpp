/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.17
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "InputDataScene.h"
#include "Based/GlobalVariable.h"

#define FontSize 20

InputDataMenu::InputDataMenu() :
	_caveFileDialog(nullptr),
	_inputCursor(nullptr),
	_inputDataDialog(nullptr),
	_editbox(nullptr),
	_textField(nullptr),
	_shieldDialogLayer(nullptr),
	_caveFileNumber(-1)
{
	_dataButton.clear();
}

InputDataMenu::~InputDataMenu()
{
}

void InputDataMenu::addLayer(Node* node)
{
	_dialogNode = node;
	_dialogNode->addChild(this, 1);
}

bool InputDataMenu::init()
{
	if (!Layer::init())return false;

	Dialog::createShieldLayer(this);

	this->createDialog();

	return true;
}

void InputDataMenu::deleteDialog()
{
	setMouseListenerEnable();
	_dialogNode->removeFromParent();
}

void InputDataMenu::createDialog()
{
	_caveFileDialog = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(_global->userInformation->getImagePath().find("InputDataDialog")->second));
	_caveFileDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_caveFileDialog->setScale(2.0f);
	_caveFileDialog->setName("CaveFileDialog");
	this->addChild(_caveFileDialog);

	auto question = Label::createWithTTF("选择你的存档", "resources/fonts/fzse_gbk.ttf", 30);
	question->setTextColor(Color4B::YELLOW);
	question->setPosition(Vec2(_caveFileDialog->getContentSize().width / 2, _caveFileDialog->getContentSize().height - 110));
	_caveFileDialog->addChild(question);

	auto SeedPacketSilhouette1 = Sprite::create(_global->userInformation->getImagePath().find("SeedPacketSilhouette1")->second);
	SeedPacketSilhouette1->setPosition(Vec2(245, 270));
	SeedPacketSilhouette1->setScale(2.0f);
	_caveFileDialog->addChild(SeedPacketSilhouette1);

	for (int i = 0; i < 8; i++)
	{
		this->createDataButton(_global->userInformation->getUserCaveFileName(i), Vec2(245, 357 - 25 * i), 3.7f, i);
	}

	/* if not read data, read it */
	if (_global->userInformation->getUserCaveFileNumber() == -1)
	{
		_caveFileNumber = UserDefault::getInstance()->getIntegerForKey("USERDATANUMBER");
		_dataButton.find(_caveFileNumber)->second->setColor(Color3B::YELLOW);
	}
	else
	{
		_caveFileNumber = _global->userInformation->getUserCaveFileNumber();
		_dataButton.find(_caveFileNumber)->second->setColor(Color3B::YELLOW);
	}

	/* 创建按钮 */
	this->createButtons(_caveFileDialog, _global->userInformation->getGameText().find("确定")->second, Vec2(110, 70), 1.0f, 3);
	this->createButtons(_caveFileDialog, _global->userInformation->getGameText().find("取消")->second, Vec2(245, 70), 1.0f, 4);
	this->createButtons(_caveFileDialog, _global->userInformation->getGameText().find("重命名")->second, Vec2(380, 70), 1.0f, 5);

	/* 创建监听 */
	this->createTouchtListener(_caveFileDialog);
}

void InputDataMenu::CreateInputDialog()
{
	_inputDataDialog = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(_global->userInformation->getImagePath().find("QuitDialog")->second));
	_inputDataDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_inputDataDialog->setScale(3.0f);
	_inputDataDialog->setName("InputDataDialog");
	_shieldDialogLayer->addChild(_inputDataDialog);

	auto editbox = Sprite::create(_global->userInformation->getImagePath().find("editbox")->second);
	editbox->setPosition(Vec2(155, 140));
	editbox->setScale(0.7f);
	_inputDataDialog->addChild(editbox);

	_inputCursor = Sprite::create(_global->userInformation->getImagePath().find("InputCursor")->second);
	_inputCursor->setScale(0.7f);
	_inputCursor->setPosition(Vec2(155, 140));
	_inputCursor->setVisible(false);
	_inputDataDialog->addChild(_inputCursor);
	_inputCursor->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.25f), FadeIn::create(0.25f), nullptr)));

	_textField = TextField::create("点击此处进行输入","resources/fonts/GameFont.ttf", FontSize);
	_inputDataDialog->addChild(_textField);
	_textField->setPosition(Vec2(155, 137));
	_textField->setMaxLengthEnabled(true);
	_textField->setMaxLength(10);
	_textField->addEventListener([=](Ref* ref, TextField::EventType type)
		{
			switch (type)
			{
			case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
				_textField->setPlaceHolder(" ");
				_inputCursor->setVisible(true);
				break;
			case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
				_textField->setPlaceHolder("点击此处进行输入");
				_inputCursor->setVisible(false);
				break;
			case cocos2d::ui::TextField::EventType::INSERT_TEXT:
			{
				/* 获取新增加的字符 */
				_newInputstring = _textField->getString().substr(_inputString.size());

				/* 更新字符串 */
				_inputString = _textField->getString();

				/* 插入字符动画 */
				auto textLabel = onTextFieldInsertText();
				
				_inputCursor->setPositionX(_inputCursor->getPositionX() + textLabel->getContentSize().width / 2);
			}
				
				break;
			case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
			{
				/* 获取删除的字符 */
				_newInputstring = _inputString.substr(_textField->getString().size());

				/* 更新字符串 */
				_inputString = _textField->getString();

				/* 删除动画 */
				auto textLabel = onTextFieldDeleteBackward();

				_inputCursor->setPositionX(_inputCursor->getPositionX() - textLabel->getContentSize().width / 2);
			}
				break;
			default:
				break;
			}
		});

	/* 创建按钮 */
	this->createButtons(_inputDataDialog, _global->userInformation->getGameText().find("确定")->second, Vec2(90, 65),1.0f, 1);
	this->createButtons(_inputDataDialog, _global->userInformation->getGameText().find("取消")->second, Vec2(225, 65),1.0f, 2);
	
	/* 创建监听 */
	this->createTouchtListener(_inputDataDialog);
}


void InputDataMenu::createButtons(Sprite* sprite, const std::string &Label, Vec2 &vec2,const float& scale, const int& ID)
{
	auto button = Button::create(_global->userInformation->getImagePath().find("button")->second, _global->userInformation->getImagePath().find("button_down")->second);
	button->setPosition(vec2);
	button->setScaleX(scale);
	button->setTitleLabel(label(Label, 20, Vec2(0, 0), 0, Color3B::GREEN, 1.0f/scale));
	sprite->addChild(button);

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
	{
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
			break;
		case Widget::TouchEventType::ENDED:
			switch (ID)
			{
			case 1: /* 确定 */
				caveData();
				updateButtonText();
				this->removeChildByName("_shieldDialogLayer");
				_inputString.clear();
				break;
			case 2: /* 取消 */
				this->removeChildByName("_shieldDialogLayer");
				_inputString.clear();
				break;
			case 3:
				UserDefault::getInstance()->setIntegerForKey("USERDATANUMBER", _caveFileNumber); /* 记录所选存档 */
				_global->userInformation->setUserCaveFileNumber(_caveFileNumber);

				/* 读取所选存档的名字并更新 */
				if (UserDefault::getInstance()->getStringForKey(_global->userInformation->getUserCaveFileNameKey(_global->userInformation->getUserCaveFileNumber()).c_str()) != "")
				{
					_global->userInformation->setUserName(UserDefault::getInstance()->getStringForKey(_global->userInformation->getUserCaveFileNameKey(_global->userInformation->getUserCaveFileNumber()).c_str()));
				}
				else
				{
					_global->userInformation->setUserName(_global->userInformation->getUserCaveFileName(_caveFileNumber));
				}
				_global->userInformation->setIsUpdate(true);

				deleteDialog();
				break;
			case 4:
				deleteDialog();
				break;
			case 5:
				createShieldLayer();
				break;
			}
		}
	});
}

void InputDataMenu::createDataButton(const std::string& Labels, Vec2& vec2, const float& scale, const int& ID)
{
	auto button = Button::create(_global->userInformation->getImagePath().find("CaveFileButton")->second, _global->userInformation->getImagePath().find("CaveFileButtonDown")->second);
	button->setPosition(vec2);
	button->setScale9Enabled(true);
	button->setScaleX(scale);
	
	setButtonText(button, Labels, scale);
	_caveFileDialog->addChild(button);

	_dataButton.insert(pair<int, Button*>(ID, button));

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case Widget::TouchEventType::ENDED:
				_caveFileNumber = ID;
				switch (ID)
				{
				case 0: setButtonColor(button); break;
				case 1: setButtonColor(button); break;
				case 2: setButtonColor(button); break;
				case 3: setButtonColor(button); break;
				case 4: setButtonColor(button); break;
				case 5: setButtonColor(button); break;
				case 6: setButtonColor(button); break;
				case 7: setButtonColor(button); break;
				}
			}
		});
}

void InputDataMenu::createShieldLayer()
{
	_global->userInformation->setUserCaveFileNumber(_caveFileNumber);
	
	_shieldDialogLayer = LayerColor::create(Color4B(0,0,0,180));
	Dialog::createShieldLayer(_shieldDialogLayer);
	this->addChild(_shieldDialogLayer, 1, "_shieldDialogLayer");

	CreateInputDialog();
}

void InputDataMenu::setButtonColor(Button* button)
{
	for (auto& sp : _dataButton)
	{
		sp.second->setColor(Color3B::WHITE);
	}
	button->setColor(Color3B::YELLOW);
}

void InputDataMenu::setButtonText(Button* button, const string& text, const float& scale)
{
	auto label = Label::createWithTTF(text, "resources/fonts/GameFont.ttf", 17);
	label->setColor(Color3B::BLACK);
	label->setScaleX(1.0f / scale);
	button->setTitleText("");
	button->setTitleLabel(label);
}

void InputDataMenu::caveData()
{
	if (strlen(_textField->getString().c_str()))
	{
		_global->userInformation->setUserName(_textField->getString());
		UserDefault::getInstance()->setStringForKey(_global->userInformation->getUserCaveFileNameKey(_caveFileNumber).c_str(), _global->userInformation->getUserName());
		_global->userInformation->setIsUpdate(true);
		_global->userInformation->setUserCaveFileName(_caveFileNumber, _global->userInformation->getUserName());
	}
}

void InputDataMenu::updateButtonText()
{
	int i = 0;
	for (auto& sp : _dataButton)
	{
		setButtonText(sp.second, _global->userInformation->getUserCaveFileName(i++), 3.7f);
	}
}

Label* InputDataMenu::onTextFieldInsertText()
{
	auto textLabel = Label::createWithTTF(_newInputstring, "resources/fonts/GameFont.ttf", FontSize);
	textLabel->setColor(Color3B(0, 255, 255));
	textLabel->setName("label");
	_inputDataDialog->addChild(textLabel);

	auto endPos = _inputCursor->getPosition() - Vec2(0, 5);
	auto inputTextSize = textLabel->getContentSize();
	Vec2 beginPos(endPos.x + textLabel->getContentSize().width / 2, 270);

	float duration = 0.5f;
	textLabel->setPosition(beginPos);
	textLabel->setScale(8);

	auto seq = Sequence::create(Spawn::create(MoveTo::create(duration, endPos),ScaleTo::create(duration, 1),FadeOut::create(duration),nullptr),
		CallFunc::create([=]()
			{
				_inputDataDialog->removeChildByName("label");
			}), nullptr);
	textLabel->runAction(seq);

	return textLabel;
}

Label* InputDataMenu::onTextFieldDeleteBackward()
{
	// create a delete text sprite and do some action
	auto label = Label::createWithTTF(_newInputstring, "resources/fonts/GameFont.ttf", FontSize);
	label->setName("label");
	label->setColor(Color3B::YELLOW);
	_inputDataDialog->addChild(label);

	// move the sprite to fly out
	auto beginPos = _inputCursor->getPosition();
	auto textfieldSize = _inputCursor->getContentSize();
	auto labelSize = label->getContentSize();
	beginPos.x += (textfieldSize.width - labelSize.width) / 2.0f;

	auto winSize = Director::getInstance()->getWinSize();
	Vec2 endPos(-winSize.width / 4.0f, winSize.height / 2.0f * (0.25 + (float)rand() / (2.0f * RAND_MAX)));

	float duration = 1;
	float rotateDuration = 0.35f;
	int repeatTime = 5;
	label->setPosition(beginPos);

	auto seq = Sequence::create(Spawn::create(MoveTo::create(duration, endPos),Repeat::create(RotateBy::create(rotateDuration, (rand() % 2) ? 360 : -360),repeatTime),FadeOut::create(duration),nullptr),
		CallFunc::create([=]() 
			{
				_inputDataDialog->removeChildByName("label");
			}),
		nullptr);
	label->runAction(seq);

	return label;
}
