//
//  Bird.cpp
//  MyGame
//
//  Created by Daniel Feles on 1/15/17.
//
//

#include "Bird.hpp"

USING_NS_CC;


#define MAX_SPEED 2
#define MAX_FORCE 0.05

#define SEPARATION 12.2
#define ALIGNMENT 1.01
#define COHERENT 10.0

#define DESIRED_SEPARATION 10.0f;
#define NEIGHBOR_DISTANCE 100.0f;

// on "init" you need to initialize your instance
Bird::Bird() {}

Bird::~Bird() {}

Bird* Bird::create() {
    Bird* mBird = new Bird();
    mBird->initOptions();
    return mBird;
}

void Bird::initOptions() {
    //setAnchorPoint(Vec2(0.5, 0.5));
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    position = Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
    
    acceleration = Vec2(0, 0);
    
    // This is a new PVector method not yet implemented in JS
    // velocity = PVector.random2D();
    
    // Leaving the code temporarily this way so that this example runs in JS
    float angle = std::rand();
    velocity = Vec2(cos(angle), sin(angle));
    
    r = 5.0;
    maxspeed = MAX_SPEED;
    maxforce = MAX_FORCE;
    
    fastNoise.SetNoiseType(FastNoise::SimplexFractal);
    fastNoise.SetFrequency(0.005f);
}

void Bird::update(cocos2d::Vector<Bird*> birds) {
    flock(birds);
    updateValues();
    render();
}

void Bird::move() {
    updateValues();
    render();
}


void Bird::updateValues() {
    // Update velocity
    velocity.add(acceleration);
    // Limit speed
    velocity = limit(velocity,maxspeed);
    auto now = std::chrono::high_resolution_clock::now();
    auto timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()/40;
    
    float fractal = fastNoise.GetNoise(position.x,position.y, timeMillis)*M_PI_2;
    
    position.add(velocity/2);
    position.add(Vec2(cos(fractal)-1,sin(fractal))*2);
    // Reset accelertion to 0 each cycle
    acceleration = acceleration * 0;
}

void Bird::render() {
    
    if (position.x < -r) position.x = visibleSize.width+r;
    if (position.y < -r) position.y = visibleSize.height+r;
    if (position.x > visibleSize.width+r) position.x = -r;
    if (position.y > visibleSize.height+r) position.y = -r;
    
    //setPosition(position.x, position.y);
}

void Bird::flock(cocos2d::Vector<Bird*> birds) {
    auto touple = separate(birds);
    cocos2d::Vec2 sep = touple.at(0);   // Separation
    cocos2d::Vec2 ali = touple.at(1);      // Alignment
    cocos2d::Vec2 coh = touple.at(2);   // Cohesion
    // Arbitrarily weight these forces
    sep = sep*SEPARATION;
    ali = ali*ALIGNMENT;
    coh = coh*COHERENT;
    // Add the force vectors to acceleration
    applyForce(sep);
    applyForce(ali);
    applyForce(coh);
}

std::vector<cocos2d::Vec2> Bird::separate(cocos2d::Vector<Bird*> birds){
    float desiredseparation = DESIRED_SEPARATION;
    float neighbordist = NEIGHBOR_DISTANCE;
    cocos2d::Vec2 steer = Vec2(0, 0);
    int countSep = 0;
    
    
    cocos2d::Vec2 sumCoh = Vec2(0, 0);   // Start with empty vector to accumulate all positions
    int countCoh = 0;
    
    cocos2d::Vec2 sumAli = Vec2(0, 0);
    int countAlign = 0;
    
    // For every boid in the system, check if it's too close
    int i = 0;
    for (Bird* bird : birds) {
        i++;
        
        //SEP
        float d = position.distance(bird->position);
        
        // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
        if ((d > 0) && (d < desiredseparation)) {
            // Calculate vector pointing away from neighbor
            cocos2d::Vec2 diff = position - bird->position;
            
            diff.normalize();
            diff = diff/d;        // Weight by distance
            steer.add(diff);
            countSep++;            // Keep track of how many
        }
        
        
        ///COH
        if ((d > 0) && (d < neighbordist)) {
            sumCoh.add(bird->position); // Add position
            countCoh++;
            
            sumAli.add(bird->velocity);
            countAlign++;
        }
    }
    // Average -- divide by how many
    if (countSep > 0) {
        steer = steer / (float)countSep;
    }
    
    // As long as the vector is greater than 0
    if (steer.length() > 0) {
        
        // Implement Reynolds: Steering = Desired - Velocity
        steer.normalize();
        steer = steer*maxspeed;
        steer.subtract(velocity);
        steer = limit(steer,maxforce);
    }
    
    if (countCoh > 0) {
        sumCoh = sumCoh/countCoh;
        sumCoh = seek(sumCoh);  // Steer towards the position
    }
    else {
        sumCoh = Vec2(0, 0);
    }
    
    if (countAlign > 0) {
        sumAli = sumAli/(float)countAlign;
        // First two lines of code below could be condensed with new PVector setMag() method
        // Not using this method until Processing.js catches up
        // sum.setMag(maxspeed);
        
        // Implement Reynolds: Steering = Desired - Velocity
        sumAli.normalize();
        sumAli = sumAli*maxspeed;
        cocos2d::Vec2 steer = sumAli - velocity;
        sumAli = limit(steer, maxforce);
    }
    else {
        sumAli = Vec2(0,0);
    }
    
    
    return {steer, sumAli, sumCoh};
}

cocos2d::Vec2 Bird::limit(cocos2d::Vec2 vector, float max) {
    if(vector.length()>max) {
        auto ratio = max/vector.length();
        return vector*ratio;
    }
    return vector;
}

cocos2d::Vec2 Bird::seek(cocos2d::Vec2 target) {
    cocos2d::Vec2 desired = target - position;  // A vector pointing from the position to the target
    // Scale to maximum speed
    desired.normalize();
    desired = desired * maxspeed;
    
    // Above two lines of code below could be condensed with new PVector setMag() method
    // Not using this method until Processing.js catches up
    // desired.setMag(maxspeed);
    
    // Steering = Desired minus Velocity
    
    cocos2d::Vec2 steer = desired - velocity;
    steer = limit(steer, maxforce);  // Limit to maximum steering force
    return steer;
}

void Bird::applyForce(cocos2d::Vec2 force) {
    // We could add mass here if we want A = F / M
    acceleration.add(force);
}
