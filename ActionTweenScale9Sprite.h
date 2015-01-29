//
//  ActionTweenScale9Sprite.h
//  BitHacker
//
//  Created by Dan Silk on 29/01/2015.
//
//

#ifndef __BitHacker__ActionTweenScale9Sprite__
#define __BitHacker__ActionTweenScale9Sprite__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ActionTweenScale9Sprite : public cocos2d::ui::Scale9Sprite, public cocos2d::ActionTweenDelegate
{
public:
    
    static ActionTweenScale9Sprite* create(const std::string& file);
    
    void updateTweenAction(float value, const std::string& key);
};

#endif /* defined(__BitHacker__ActionTweenScale9Sprite__) */
