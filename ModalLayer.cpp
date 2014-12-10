//
//  ModalLayer.cpp
//  XmasCracker
//
//  Created by Dan on 3/12/2014.
//
//

#include "ModalLayer.h"

USING_NS_CC;

bool ModalLayer::init()
{
    if (!cocos2d::Layer::init())
    {
        return false;
    }
    
    return true;
}

void ModalLayer::onEnter()
{
    Layer::onEnter();
    
    // Register Touch Event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ModalLayer::onTouchBegan, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool ModalLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    cocos2d::log("You touched %f, %f", touch->getLocationInView().x, touch->getLocationInView().y);
    
    return true;
}