//
//  SimpleBird.cpp
//  MyGame
//
//  Created by Daniel Feles on 1/15/17.
//
//

#include "SimpleBird.hpp"

USING_NS_CC;


#define MAX_SPEED 0.5
#define MAX_FORCE 1

#define SEPARATION 100.2
#define ALIGNMENT 3.1
#define COHERENT 25.0

#define DESIRED_SEPARATION 70.0f;
#define NEIGHBOR_DISTANCE 50.0f;

// on "init" you need to initialize your instance
SimpleBird::SimpleBird() {}

SimpleBird::~SimpleBird() {}

SimpleBird* SimpleBird::create(int index, int x, int y) {
    SimpleBird* mBird = new SimpleBird();
    
    mBird->initWithFile("birdAnim/birdAnim1.png");
    if(x!= -1 && y!= -1) {
        mBird->position = Vec2(x,y);
    }
    mBird->initOptions(index);
    mBird->createAnimation();
    return mBird;
}

void SimpleBird::createAnimation() {
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(5);
    auto width = getContentSize().width;
    for(int i = 0; i < 5; i++)
    {
        std::stringstream name;
        name << "birdAnim/birdAnim" << i+1 << ".png";
        animFrames.pushBack(SpriteFrame::create(name.str(), Rect(0,0,width,width)));
    }
    
    auto delay = DelayTime::create(random(0.2f, 0.4f));
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);
    auto seq1 = Sequence::create(animate, delay, nullptr);
    runAction(RepeatForever::create(seq1));
}

void SimpleBird::makeSpecial() {
    setScale(.5, .5);
}

void SimpleBird::initOptions(int _index) {
    index = _index;
    setAnchorPoint(Vec2(0.5,0.5));
    
    // Leaving the code temporarily this way so that this example runs in JS
    float angle = std::rand();
    vector = Vec2();
    
    r = 5.0;
    maxspeed = MAX_SPEED;
    maxforce = MAX_FORCE;
    
    fastNoise.SetNoiseType(FastNoise::GradientFractal);
    fastNoise.SetFrequency(0.005f);
    
    rndSeed = random(0.2f, 1.0f);
    
    desiredseparation = DESIRED_SEPARATION;
    //desiredseparation *= rndSeed * 5;
    neighbordist = NEIGHBOR_DISTANCE;
    //neighbordist *= rndSeed;
}

void SimpleBird::rearrage() {
    
    scaling = getParent()->getScale();
    
    visibleSize = Director::getInstance()->getVisibleSize() / scaling;
    origin = Director::getInstance()->getVisibleOrigin() / scaling;
    centerPosition = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
}

void SimpleBird::addedToParent() {
    rearrage();
    if(position == Vec2(-1,-1)) {
        position = centerPosition + Vec2(random(-30, 30), random(-30, 30));
    } else {
        position =  position / scaling;
    }
    setPosition(position);
    
    
    drawNode = DrawNode::create();
    
    getParent()->addChild(drawNode);
}

float angleFor(Vec2 v1, Vec2 v2) {
    v1.normalize();
    v2.normalize();
    float a = atan2(v2.y, v2.x) - atan2(v1.y, v1.x);
    if (a < 0) a += M_PI_4;
    return a;
}

void SimpleBird::update(cocos2d::Point mousePosition, cocos2d::Vec2 direction) {
    rearrage();
    
    mousePosition = mousePosition / scaling;
    acceleration = Vec2(0,0);
    auto now = std::chrono::high_resolution_clock::now();
    timeInMillis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()/40;
    
    auto norm = Vec2(getPosition(), centerPosition);
    norm.scale(Vec2(1,3));
    fractal = (fastNoise.GetNoise(position.x/4 ,position.y/4, timeInMillis))*M_PI_2;
    auto angle = norm.getAngle() + M_PI/2-fractal;
    
    
    auto rotate = Vec2::forAngle(angle);
    
    angle = norm.getAngle();
    auto goHome = Vec2::forAngle(angle);
    
    applyForce(goHome, 0.1f);
    //drawNode->clear();
    
    
    fractal = fastNoise.GetNoise(position.x/5/scaling ,position.y/5/scaling, timeInMillis*2 + rndSeed*100)*20;
    
    auto fra = Vec2::forAngle(fractal);
    //drawNode->drawLine(getPosition(), getPosition()+fra*10, Color4F::GREEN);
    
    auto maxDist = visibleSize.height/2;
    auto dist = getPosition().distance(centerPosition);
    
    auto multi = fmax(pow(dist/maxDist,2.0), 0.0)*10 + 1;
    
    float timeSin = (1-pow(sin(timeInMillis/100 + rndSeed*100),4.0))*2.0f;
    
    goHome = goHome*multi*2.0f;
    
    
    auto fractalRotate = fastNoise.GetNoise(position.x/10*scaling ,position.y/10*scaling, timeInMillis);
    if(fractalRotate > 0) {
        fractalRotate = 5;
    } else {
        fractalRotate = -5;
    }
    rotate = rotate * fractalRotate;
    fra = fra*rndSeed*6.0f * random(1.0f,timeSin);

    
    auto pointDist = getPosition().distance(mousePosition) + centerPosition.distance(mousePosition);
    
    if((pointDist - getPosition().distance(centerPosition)) < 10) {
        attached = false;
    }
    if(!attached) {
        //goHome = goHome * -10;
    }
    
    //drawNode->drawLine(getPosition(), getPosition()+goHome*10, Color4F::BLUE);
    //drawNode->drawDot(mousePosition, 3 , Color4F(0.0f,0.0f,1.0f,1.0f));
    if(direction != Vec2(0,0)) {
        //fra = -direction*2;
        rotate = rotate -direction*20;
    }
    
    vector = goHome + rotate + fra;
    vector = limit(vector, maxspeed);
    updateValues();
}

void SimpleBird::move() {
    updateValues();
}


void SimpleBird::updateValues() {
    
    position.add(vector);
    
    auto scale = std::abs((fastNoise.GetNoise(position.x*3,position.y*3, timeInMillis*2)))/2.0f+.1;
    //auto angle = 90-velocity.getAngle() * (180/M_PI);
    
    
    setScale(modulate(getScale(),scale, 10.0f));
    
    auto oldPosition = getPosition();
    setPosition(modulate(getPositionX(), position.x, 50.0f), modulate(getPositionY(), position.y, 50.0f));
    auto angle = (getPosition()-oldPosition).getAngle();
    auto an = 90 - angle * 180/M_PI;
    setRotation(modulate(getRotation(),an,20.0f));
}

float SimpleBird::modulate(float x, float to, float speed) {
    return (x*speed + to)/(speed+1);
}

Vec2 SimpleBird::modulateVec(Vec2 x, Vec2 to, float speed) {
    return (x*speed + to)/(speed+1);
}

void SimpleBird::border() {
    auto changed = false;
    if (position.x < -r) { position.x = visibleSize.width+r; changed = true; }
    if (position.y < -r) { position.y = visibleSize.height+r; changed = true; }
    if (position.x > visibleSize.width+r) { position.x = -r; changed = true; }
    if (position.y > visibleSize.height+r) { position.y = -r; changed = true; }
    if (changed) getParent()->removeChild(this);
}

cocos2d::Vec2 SimpleBird::limit(cocos2d::Vec2 vector, float max) {
    if(vector.length()>max) {
        auto ratio = max/vector.length();
        return vector*ratio;
    }
    return vector;
}

cocos2d::Vec2 SimpleBird::seek(cocos2d::Vec2 target) {
    cocos2d::Vec2 desired = target - position;  // A vector pointing from the position to the target
    // Scale to maximum speed
    desired.normalize();
    desired = desired * maxspeed;
    
    cocos2d::Vec2 steer = desired - vector;
    steer = limit(steer, maxforce);  // Limit to maximum steering force
    return steer;
}

void SimpleBird::applyForce(cocos2d::Vec2 force, float mass) {
    if(mass == 0) return;
    force = limit(force, maxforce);
    acceleration.add(force * mass);
}
