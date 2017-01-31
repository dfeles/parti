//
//  Bird.hpp
//  MyGame
//
//  Created by Daniel Feles on 1/15/17.
//
//

#ifndef Bird_hpp
#define Bird_hpp

#include "cocos2d.h"
#include "FastNoise.h"

class Bird : public cocos2d::Sprite
{
public:
    Bird();
    ~Bird();
    static Bird* create(int index);
    cocos2d::Sprite* createBird();
    void update(cocos2d::Vector<Bird*> birds);
    void move();
    void makeSpecial();
    void addedToParent();
    
    cocos2d::Vec2 position;
    cocos2d::Vec2 centerPosition;
    cocos2d::Vec2 velocity;
    cocos2d::Vec2 acceleration;
    FastNoise fastNoise;
private:
    float r;
    float maxforce;    // Maximum steering force
    float maxspeed;    // Maximum speed
    float fractal = 0;
    
    float rndSeed;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    float desiredseparation, neighbordist;
    
    void initOptions(int _index);
    void createAnimation();
    
    void flock(cocos2d::Vector<Bird*> birds);
    void updateValues();
    void border();
    float modulate(float x, float to, float speed);
    void applyForce(cocos2d::Vec2 force);
    
    float scaling;
    int index;
    
    std::vector<cocos2d::Vec2> separate(cocos2d::Vector<Bird*> birds);
    
    cocos2d::Vec2 seek(cocos2d::Vec2 target);
    cocos2d::Vec2 limit(cocos2d::Vec2 vector, float max);
    
};

#endif
