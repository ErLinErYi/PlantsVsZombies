/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.3.8
 *Email: 2117610943@qq.com
 */

#include "LZAGSPauseLayer.h"
#include "LZAGSPauseQuitLayer.h"
#include "LZAGameScene.h"
#include "Based/LZPlayMusic.h"

GSPauseLayer::GSPauseLayer():
   _levelObjiectives(nullptr)
, _global(Global::getInstance())
{
}

GSPauseLayer::~GSPauseLayer()
{
}

Layer* GSPauseLayer::addLayer()
{
    return GSPauseLayer::create();
}

bool GSPauseLayer::init()
{
    if(!LayerColor::initWithColor(Color4B(0,0,0,180))) return false;

    createShieldLayer(this);
    createDialog();
    showAniamtions();
    showButton();
    return true;
}

void GSPauseLayer::createDialog()
{
    _levelObjiectives = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("LevelObjiectives.png");
    _levelObjiectives->setPosition(Director::getInstance()->getWinSize() / 2.0f);
    _levelObjiectives->setGlobalZOrder(10);
    _levelObjiectives->setScale(2.0f);
    this->addChild(_levelObjiectives);

    auto LevelObjiectivesText = Text::create();
    LevelObjiectivesText->setString(_global->getInstance()->userInformation->getGameText().find("休息一会吧！")->second->text);
    LevelObjiectivesText->setFontName(GAME_FONT_NAME_1);
    LevelObjiectivesText->setFontSize(_global->getInstance()->userInformation->getGameText().find("休息一会吧！")->second->fontsize);
    LevelObjiectivesText->setScale(0.5f);
    LevelObjiectivesText->setColor(Color3B(0, 255, 255));
    LevelObjiectivesText->setPosition(Vec2(_levelObjiectives->getContentSize().width / 2, 245));
    LevelObjiectivesText->setGlobalZOrder(10);
    _levelObjiectives->addChild(LevelObjiectivesText);

    Dialog::createTouchtListener(_levelObjiectives);
}

void GSPauseLayer::showAniamtions()
{
    auto pauseAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("powerup")->second);
    pauseAnimation->setAnimation(0, "animation", true);
    pauseAnimation->setPosition(Vec2(_levelObjiectives->getContentSize().width / 2.f, 50));
    pauseAnimation->setGlobalZOrder(10);
    pauseAnimation->setScale(0.6f);
    _levelObjiectives->addChild(pauseAnimation);
}

void GSPauseLayer::showButton()
{
    auto button = Button::create("Continue1.png", "Continue.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
    button->setTitleText(Global::getInstance()->userInformation->getGameText().find("确定")->second->text);
    button->setTitleFontName(GAME_FONT_NAME_1);
    button->setTitleFontSize(Global::getInstance()->userInformation->getGameText().find("确定")->second->fontsize);
    button->setTitleColor(Color3B::YELLOW);
    button->setPosition(Vec2(_levelObjiectives->getContentSize().width / 2.0f, 10));
    button->setGlobalZOrder(10);
    button->setScale(0.5f);
    button->runAction(FadeIn::create(0.5f));
    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case ui::Widget::TouchEventType::BEGAN:
                PlayMusic::playMusic("gravebutton");
                break;
            case ui::Widget::TouchEventType::ENDED:
                GSPauseQuitLayer::resumeLayer();
                this->removeFromParent();
                break;
            }
        });
    _levelObjiectives->addChild(button);
}
