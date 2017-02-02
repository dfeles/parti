//
//  Bird.hpp
//  MyGame
//
//  Created by Daniel Feles on 1/15/17.
//
//

#ifndef SimpleBird_hpp
#define SimpleBird_hpp

#include "cocos2d.h"
#include "FastNoise.h"

class SimpleBird : public cocos2d::Sprite
{
public:
    SimpleBird();
    ~SimpleBird();
    static SimpleBird* create(int index, int x = -1, int y = -1);
    cocos2d::Sprite* createBird();
    void update(cocos2d::Point middleLoc, cocos2d::Vec2 direction);
    void move();
    void makeSpecial();
    void addedToParent();
    
    cocos2d::Vec2 position = cocos2d::Vec2(-1,-1);
    cocos2d::Vec2 centerPosition;
    cocos2d::Vec2 vector;
    cocos2d::Vec2 acceleration;
    float angle;
    FastNoise fastNoise;
    
    static float modulate(float x, float to, float speed);
    static cocos2d::Vec2 modulateVec(cocos2d::Vec2 x, cocos2d::Vec2 to, float speed);
    
private:
    float r;
    float maxforce;    // Maximum steering force
    float maxspeed;    // Maximum speed
    float fractal = 0;
    
    bool attached = true;
    
    float timeInMillis;
    cocos2d::DrawNode* drawNode;

    float rndSeed;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    float desiredseparation, neighbordist;
    
    void initOptions(int _index);
    void createAnimation();
    void rearrage();
    
    void flock(cocos2d::Vector<SimpleBird*> birds);
    void updateValues();
    void border();
    
    void applyForce(cocos2d::Vec2 force, float mass);
    
    float scaling;
    int index;
    
    std::vector<cocos2d::Vec2> separate(cocos2d::Vector<SimpleBird*> birds);
    
    cocos2d::Vec2 seek(cocos2d::Vec2 target);
    cocos2d::Vec2 limit(cocos2d::Vec2 vector, float max);
    
};

#endif
