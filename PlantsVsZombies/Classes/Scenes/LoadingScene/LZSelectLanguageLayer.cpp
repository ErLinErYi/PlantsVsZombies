/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.01.02
 *Email: 2117610943@qq.com
 */

#include "LZSelectLanguageLayer.h"
#include "LZLoadingScene.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZMouseEventControl.h"

SelectLanguage::SelectLanguage() :
	_selectLanguageDialog(nullptr),
	_scrollView(nullptr),
	_document(nullptr)
{
	auto name = UserDefault::getInstance()->getStringForKey("LANGUAGE");
	_selectLanguageName = name.empty() ? "中文" : name;
}

SelectLanguage::~SelectLanguage()
{
}

bool SelectLanguage::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	Dialog::createShieldLayer(this);

	createDialog();
	createMouseListener();

	return true;
}

void SelectLanguage::createDialog()
{
	_selectLanguageDialog = Sprite::createWithSpriteFrameName("LevelObjiectives.png");
	_selectLanguageDialog->setPosition(Director::getInstance()->getWinSize() / 2.f);
	_selectLanguageDialog->setScale(2.0f);
	this->addChild(_selectLanguageDialog);

	auto question = Text::create(_global->userInformation->getGameText().find("选择你界面显示的语言！")->second->text, GAME_FONT_NAME_1,
		_global->userInformation->getGameText().find("选择你界面显示的语言！")->second->fontsize);
	question->setTextColor(Color4B(0, 255, 255, 255));
	question->setPosition(Vec2(_selectLanguageDialog->getContentSize().width / 2, _selectLanguageDialog->getContentSize().height - 70));
	_selectLanguageDialog->addChild(question);

	auto prompt = Text::create(_global->userInformation->getGameText().find("重启后生效！")->second->text, GAME_FONT_NAME_1,
		_global->userInformation->getGameText().find("重启后生效！")->second->fontsize);
	prompt->setTextColor(Color4B::RED);
	prompt->setPosition(Vec2(_selectLanguageDialog->getContentSize().width / 2, _selectLanguageDialog->getContentSize().height - 95));
	_selectLanguageDialog->addChild(prompt);

	/* 创建按钮 */
	createButtons(_selectLanguageDialog, _global->userInformation->getGameText().find("确定")->second->text, Vec2(130, 10), 0.25f, 1);
	createButtons(_selectLanguageDialog, _global->userInformation->getGameText().find("取消")->second->text, Vec2(380, 10), 0.25f, 2);

	showLanguageOptionBox();

	showDialogAction(_selectLanguageDialog);

	/* 创建监听 */
	createTouchtListener(_selectLanguageDialog);
}

void SelectLanguage::showLanguageOptionBox()
{
	auto buttonGroup = RadioButtonGroup::create();
	buttonGroup->setAllowedNoSelection(false);
	buttonGroup->setPosition(_selectLanguageDialog->getContentSize() / 2);
	_selectLanguageDialog->addChild(buttonGroup);

	_scrollView = ui::ScrollView::create();
	_scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	_scrollView->setContentSize(Size(400.0f, 150));
	_scrollView->setPosition(Vec2(60,50));
	_scrollView->setBounceEnabled(true);
	_scrollView->setScrollBarPositionFromCorner(Vec2(10, 0));
	_scrollView->setScrollBarAutoHideTime(2.0f);
	_scrollView->setScrollBarWidth(7);
	_scrollView->setScrollBarColor(Color3B::WHITE);
	_selectLanguageDialog->addChild(_scrollView);

	_document.Parse<rapidjson::kParseDefaultFlags>(FileUtils::getInstance()->getStringFromFile(_global->userInformation->getTextPath().find("language")->second).c_str());
	if (_document.HasParseError()) 
	{
		auto question = Text::create(_global->userInformation->getGameText().find("语言加载失败！")->second->text, GAME_FONT_NAME_1,
			_global->userInformation->getGameText().find("语言加载失败！")->second->fontsize);
		question->setTextColor(Color4B::RED);
		question->setPosition(Vec2(_selectLanguageDialog->getContentSize().width / 2.f, _selectLanguageDialog->getContentSize().height / 2.f - 30));
		_selectLanguageDialog->addChild(question);
		return;
	}

	float precent = 0.f;
	if (_document.HasMember("language"))
	{
		for (unsigned int i = 0; i < _document["language"].Size(); ++i)
		{
			auto select = RadioButton::create("CheckBox2.png", "CheckBox.png", "CheckBox.png", "CheckBox.png", "CheckBox.png",cocos2d::ui::Widget::TextureResType::PLIST);
			select->setPosition(Vec2(140 + 100 * i, 80));
			select->setScale(0.4f);
			select->setName(_document["language"][i].GetString());
			select->addEventListener(CC_CALLBACK_2(SelectLanguage::onChangedRadioButtonSelect, this));
			buttonGroup->addRadioButton(select);
			_scrollView->addChild(select);

			if (_selectLanguageName == _document["language"][i].GetString())
			{
				buttonGroup->setSelectedButtonWithoutEvent(select);
				precent = i * 1.0f / 4 * 100;
			}

			auto text = Text::create(_document["language"][i].GetString(),
				_global->userInformation->getGameText().find("系统字体名称")->second->text,
				_global->userInformation->getGameText().find("系统字体名称")->second->fontsize);
			text->setColor(Color3B::BLACK);
			text->setPosition(Vec2(50, -40));
			select->addChild(text);
		}
	}
	_scrollView->setInnerContainerSize(Size(400 + (_document["language"].Size() - 2) * 99, 150));
	_scrollView->scrollToPercentHorizontal(precent, 1.f, true);
}

void SelectLanguage::reloadLanguageText(const char* language)
{
	/* 读取语言 */
	if (_document.HasMember(language))
	{
		for (unsigned int j = 0; j < _document[language].Size(); ++j)
		{
			LanguageTextAttribute* lta = new LanguageTextAttribute;
			lta->text = _document[language][j][1].GetString();
			lta->fontsize = _document[language][j][2].GetFloat();
			lta->scale = _document[language][j][3].GetFloat();
			lta->position.x = _document[language][j][4].GetFloat();
			lta->position.y = _document[language][j][5].GetFloat();

			_global->userInformation->getGameText().insert(pair<string, LanguageTextAttribute*>(_document[language][j][0].GetString(), lta));
		}
	}
}

void SelectLanguage::setNewLanguage()
{
	if (UserDefault::getInstance()->getStringForKey("LANGUAGE") != _selectLanguageName)
	{
		UserDefault::getInstance()->setStringForKey("LANGUAGE", _selectLanguageName);
		_global->userInformation->gameTextInit();
		reloadLanguageText(_selectLanguageName.c_str());
	}
	LoadingScene::loadingFinished = false;
	showDeleteDialogAction(_selectLanguageDialog);
}

void SelectLanguage::createButtons(Sprite* sprite, const std::string& Label, Vec2& vec2, const float& scale, const int& ID)
{
	auto button = Button::create("ButtonNew.png", "ButtonNew2.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	button->setPosition(vec2);
	button->setScale(scale);
	button->setTitleFontSize(70);
	button->setTitleFontName(GAME_FONT_NAME_1);
	button->setTitleText(Label);
	button->setTitleColor(Color3B::BLACK);
	sprite->addChild(button);

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap");
				break;
			case Widget::TouchEventType::ENDED:
				switch (ID)
				{
				case 1: /* 确定 */
					setNewLanguage();
					break;
				case 2: /* 取消 */
					showDeleteDialogAction(_selectLanguageDialog);
					break;
				}
			}
		});
}

void SelectLanguage::onChangedRadioButtonSelect(RadioButton* radioButton, RadioButton::EventType type)
{
	PlayMusic::playMusic("bleep");
	switch (type)
	{
	case RadioButton::EventType::SELECTED:
		_selectLanguageName = radioButton->getName();
		break;
	}
}

void SelectLanguage::createMouseListener()
{
	/* 鼠标滑动监听 */
	auto mouse = EventListenerMouse::create();
	mouse->onMouseScroll = [&](Event* event)
	{
		const auto mouseEvent = static_cast<EventMouse*>(event);
		const float movex = mouseEvent->getScrollY() * 40;

		MouseEventControl::mouseScrollControlListener(_scrollView, movex, ui::ScrollView::Direction::HORIZONTAL);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _scrollView);
}
