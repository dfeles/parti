#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleBird.hpp"
#include "Footer.hpp"
#include "Magic.hpp"


class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    cocos2d::Size visibleSize;
    cocos2d::Point mousePosition;
    void createBird(int x = -1, int y = -1);
    cocos2d::Vec2 direction = cocos2d::Vec2(0,0);
    cocos2d::Vec2 directionModulated = cocos2d::Vec2(0,0);
    cocos2d::LayerGradient* backgroundLayer;
    
    Footer* footer;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    cocos2d::Director* director;
    
    cocos2d::DrawNode* dot;
    
    cocos2d::Node* birdsField;
    cocos2d::Vector<SimpleBird*> birds;
    cocos2d::Vector<cocos2d::Sprite*> birdSprites;
    virtual void update(float delta);
    void createParallaxBg();
    
    cocos2d::ParallaxNode* parallax;
    cocos2d::Sprite* background;
    
};

#endif // __HELLOWORLD_SCENE_H__
