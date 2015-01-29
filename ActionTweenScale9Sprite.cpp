//
//  ActionTweenScale9Sprite.cpp
//  BitHacker
//
//  Created by Dan Silk on 29/01/2015.
//
//

#include "ActionTweenScale9Sprite.h"

ActionTweenScale9Sprite* ActionTweenScale9Sprite::create(const std::string& file)
{
    ActionTweenScale9Sprite* pReturn = new (std::nothrow) ActionTweenScale9Sprite();
    if ( pReturn && pReturn->initWithFile(file) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

void ActionTweenScale9Sprite::updateTweenAction(float value, const std::string& key)
{
    if(key.compare("height") == 0)
        this->setContentSize(cocos2d::Size(this->getContentSize().width, value));
}