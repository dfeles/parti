#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Bird.hpp"


class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    cocos2d::Size visibleSize;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    cocos2d::Director* director;
    
    cocos2d::DrawNode* dot;
    
    cocos2d::Node* birdsField;
    cocos2d::Vector<Bird*> birds;
    cocos2d::Vector<cocos2d::Sprite*> birdSprites;
    virtual void update(float delta);
    void createParallaxBg();
    
    cocos2d::ParallaxNode* parallax;
    cocos2d::Sprite* background;
    
};

#endif // __HELLOWORLD_SCENE_H__
