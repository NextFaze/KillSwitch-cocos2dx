//
//  ModalLayer.h
//  XmasCracker
//
//  Created by Dan on 3/12/2014.
//
//

#ifndef __XmasCracker__ModalLayer__
#define __XmasCracker__ModalLayer__

#include "cocos2d.h"

class ModalLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(ModalLayer);
    
private:
    virtual void onEnter();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif /* defined(__XmasCracker__ModalLayer__) */
