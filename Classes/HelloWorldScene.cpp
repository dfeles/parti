#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define BIRDNUMB 500


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
    if ( !Layer::init() )
    {
        return false;
    }
    scheduleUpdate();
    director = Director::getInstance();
    
    visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();
    
    createParallaxBg();
    
    birdsField = Node::create();
    this->addChild(birdsField);
    birdsField->setAnchorPoint(Vec2(.5,.5));
    birdsField->setScale(2.0);
    
    auto label = Label::createWithSystemFont("hello", "Helvetica", 12);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    
    
    for(int i=0; i<BIRDNUMB ;i++){
        auto bird = Bird::create(i);
        if(i == 0) {
            bird->makeSpecial();
        }
        //auto birdSprite = Sprite::create("bird.png");
        birdsField->addChild(bird);
        bird->addedToParent();
        //birdSprites.insert(i, birdSprite);
        birds.pushBack(bird);

    }
    
    dot = cocos2d::DrawNode::create();
    addChild(dot);
    
    return true;
}

void HelloWorld::createParallaxBg() {
    
    
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("background.png");
    Texture2D::TexParams texParams = {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
    texture->setTexParameters(texParams);
    
    background = Sprite::createWithTexture(texture, (Rect){ 0, 0, visibleSize.width*2, visibleSize.height*2});
    this->addChild(background);
    
    
    auto blue = Sprite::create("blue.png");
    auto scaleX = visibleSize.width*2 / blue->getContentSize().width/2;
    auto scaleY = visibleSize.height*2 / blue->getContentSize().height/2;
    blue->setAnchorPoint(Vec2(0,0));
    blue->setScale(scaleX, scaleY);
    
    auto someBlend = BlendFunc();
    someBlend.src = GL_ONE;
    someBlend.dst = GL_ONE_MINUS_SRC_ALPHA;
    //blue->setBlendFunc(someBlend);
    //blue->setOpacityModifyRGB(true);
    blue->setOpacity(100);
    
    this->addChild(blue);
    /*
    auto middle_layer = Sprite::create("background.png");
    parallax->addChild(middle_layer, 1, Vec2(2.2f,1.0f), Vec2(0,-200) );
    
    auto top_layer = Sprite::create("background.png");
    parallax->addChild(top_layer, 2, Vec2(3.0f,2.5f), Vec2(200,800) );
    
    this->addChild(parallax);
     */
}

int i = 0;
void HelloWorld::update( float dt ) {
    float x = i/1.0f;
    float y = sin(i/20.0f)*10.0f;
    background->setTextureRect((Rect){ x, y, visibleSize.width*2, visibleSize.height*2});
    
    if(i%50 == 0) {
        //dot->drawSolidRect(Vec2(0,0), Vec2(visibleSize.width, visibleSize.height), Color4F(1.0f,0.0f,0.0f,0.01f));
    
    }
    dot->clear();
    i++;
    int n = 0;
    
    
    cocos2d::Vec2* points = new cocos2d::Vec2[BIRDNUMB];
    for(auto bird:birds) {
        n++;
        if(n == 1) continue;
        if((n+i)%10 == 0) {
            bird->update(birds);
        } else {
            bird->move();
        }
        points[n] = bird->position;
    }
}
