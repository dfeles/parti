#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define BIRDNUMB 1500


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    scheduleUpdate();
    director = Director::getInstance();
    
    visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithSystemFont("hello", "Helvetica", 12);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    for(int i=0; i<BIRDNUMB ;i++){
        auto bird = Bird::create();
        //this->addChild(bird);
        birds.pushBack(bird);

    }
    
    dot = cocos2d::DrawNode::create();
    addChild(dot);
    
    return true;
}

int i = 0;
void HelloWorld::update( float dt ) {
    if(i%50 == 0) {
        //dot->drawSolidRect(Vec2(0,0), Vec2(visibleSize.width, visibleSize.height), Color4F(1.0f,0.0f,0.0f,0.01f));
    
    }
    //dot->clear();
    i++;
    int n = 0;
    
    
    cocos2d::Vec2* points = new cocos2d::Vec2[BIRDNUMB];
    for(auto bird:birds) {
        if((n+i)%8 == 0) {
            bird->update(birds);
        } else {
            bird->move();
        }
        points[n] = bird->position;
        n++;
    }
    dot->drawPoints(points, BIRDNUMB, 4, Color4F(1.0f,1.0f,1.0f,0.1f));
    
    
}
