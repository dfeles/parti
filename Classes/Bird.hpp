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

class Bird : public cocos2d::Object
{
public:
    Bird();
    ~Bird();
    static Bird* create();
    cocos2d::Sprite* createBird();
    void initOptions();
    void update(cocos2d::Vector<Bird*> birds);
    void move();
    
    cocos2d::Vec2 position;
    cocos2d::Vec2 velocity;
    cocos2d::Vec2 acceleration;
    FastNoise fastNoise;
private:
    float r;
    float maxforce;    // Maximum steering force
    float maxspeed;    // Maximum speed
    cocos2d::Size visibleSize;
    
    void flock(cocos2d::Vector<Bird*> birds);
    void updateValues();
    void render();
    void applyForce(cocos2d::Vec2 force);
    
    std::vector<cocos2d::Vec2> separate(cocos2d::Vector<Bird*> birds);
    
    cocos2d::Vec2 seek(cocos2d::Vec2 target);
    cocos2d::Vec2 limit(cocos2d::Vec2 vector, float max);
    
};

#endif
