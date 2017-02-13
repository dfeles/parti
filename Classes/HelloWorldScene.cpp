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
void HelloWorld::createBird(int x, int y) {
    auto bird = SimpleBird::create((int) birds.size(), x, y);
    birdsField->addChild(bird);
    bird->addedToParent();
    birds.pushBack(bird);
}

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
    birdsField->setScale(5.0);
    
    auto label = Label::createWithSystemFont("hello", "Helvetica", 12);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    
    
    for(int i=0; i<BIRDNUMB; i++){
        createBird();        

    }
    
    dot = cocos2d::DrawNode::create();
    addChild(dot);
    
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = [this](cocos2d::Event* event){
        // Cast Event to EventMouse for position details like above
        try {
            EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
            createBird(mouseEvent->getCursorX(), mouseEvent->getCursorY());
        }
        catch(std::bad_cast& e){
            return;
        }
    };
    
    listener->onMouseMove = [this](cocos2d::Event* event){
        // Cast Event to EventMouse for position details like above
        try {
            EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
            mousePosition = Point(mouseEvent->getCursorX(),mouseEvent->getCursorY());
            
            
        }
        catch (std::bad_cast& e){
            // Not sure what kind of event you passed us cocos, but it was the wrong one
            return;
        }
    };
    
    auto keyB = EventListenerKeyboard::create();
    keyB->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event){
        
        switch(keyCode){
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case EventKeyboard::KeyCode::KEY_A:
                direction -= Vec2(1,0);
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case EventKeyboard::KeyCode::KEY_D:
                direction -= Vec2(-1,0);
                break;
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
            case EventKeyboard::KeyCode::KEY_W:
                direction -= Vec2(0,-1);
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            case EventKeyboard::KeyCode::KEY_S:
                direction -= Vec2(0,1);
                break;
            default:
                break;
        }
    };
    keyB->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event){
        switch(keyCode){
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case EventKeyboard::KeyCode::KEY_A:
                direction += Vec2(1,0);
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case EventKeyboard::KeyCode::KEY_D:
                direction += Vec2(-1,0);
                break;
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
            case EventKeyboard::KeyCode::KEY_W:
                direction += Vec2(0,-1);
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            case EventKeyboard::KeyCode::KEY_S:
                direction += Vec2(0,1);
                break;
            default:
                break;
        }
    };

    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(keyB, 2);
    
    return true;
}

void HelloWorld::createParallaxBg() {
    
    /*
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("background.png");
    Texture2D::TexParams texParams = {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
    texture->setTexParameters(texParams);
    
    background = Sprite::createWithTexture(texture, (Rect){ 0, 0, visibleSize.width*2, visibleSize.height*2});
     this->addChild(background);
    */
    
    
    auto startColor = Color4B(255,220,200,255);
    auto endColor = Color4B(254,126,77,255);
    
    backgroundLayer = LayerGradient::create(startColor, endColor, Vec2(1,1));
    
    backgroundLayer->setContentSize(visibleSize);
    backgroundLayer->setPosition(0,0);
    this->addChild(backgroundLayer);
    
    auto blue = Sprite::create("blue.png");
    auto scaleX = visibleSize.width*2 / blue->getContentSize().width/2;
    auto scaleY = visibleSize.height*2 / blue->getContentSize().height/2;
    blue->setAnchorPoint(Vec2(0,0));
    blue->setScale(scaleX, scaleY);
    
    
    
    footer = Footer::create();
    footer->setAnchorPoint(Vec2(0,0));
    this->addChild(footer);
    
    auto fog = Sprite::create("fog.png");
    fog->setAnchorPoint(Vec2(0,0));
    //this->addChild(fog);
}

int i = 0;
void HelloWorld::update( float dt ) {
    
    auto ratio = 2000 / birds.size();
    //birdsField->setScale(SimpleBird::modulate(birdsField->getScale(), scale, 100));
    
    auto m = (sin(i/100.0f) + 1)/2.0f*255.0f;
    auto startColor = Color4B(255,220,255-m,255);
    auto endColor = Color4B(254,126,m,255);
    auto modStartColor = Magic::modulateColor(Color4B(backgroundLayer->getStartColor()), startColor, 10.0f);
    auto modEndColor = Magic::modulateColor(Color4B(backgroundLayer->getEndColor()), endColor, 10.0f);
    backgroundLayer->setStartColor(Color3B(modStartColor));
    backgroundLayer->setStartOpacity(modStartColor.a);
    backgroundLayer->setEndColor(Color3B(modEndColor));
    backgroundLayer->setEndOpacity(modEndColor.a);
    
    directionModulated = Magic::modulateVec(directionModulated, direction*5, 200.0);
    
    //backgroundLayer->setVector(Vec2::forAngle(i/100.0f));
    
    //Point scrollDecrement = Point(-(visibleSize.width/2 - mousePosition.x)/visibleSize.width*100,  -mousePosition.y/visibleSize.height*10);
    
    auto posi = footer->getPosition() + directionModulated;
    posi = Vec2(posi.x, fmin(posi.y, 0));
    
    footer->setPosition(posi);
    footer->updatePosition();
    
    
    //background->setTextureRect((Rect){ footer->getPosition().x, footer->getPosition().y, visibleSize.width*2, visibleSize.height*2});
    
    if(i%100 == 0) {
        //dot->drawSolidRect(Vec2(0,0), Vec2(visibleSize.width, visibleSize.height), Color4F(1.0f,0.0f,0.0f,0.01f));
    
    }
    dot->clear();
    i++;
    int n = 0;
    
    
    //cocos2d::Vec2* points = new cocos2d::Vec2[BIRDNUMB];
    
    for(auto bird:birds) {
        n++;
        //if(n == 1) continue;
        
        bird->update(mousePosition, direction);
    }
}
