//
//  footer.cpp
//  MyGame
//
//  Created by Daniel Feles on 2/2/17.
//
//

#include "Footer.hpp"
USING_NS_CC;

Footer::Footer() {}

Footer::~Footer() {}

class PointObject : public Ref
{
public:
    inline void setRation(Point ratio) {_ratio = ratio;}
    inline void setOffset(Point offset) {_offset = offset;}
    inline void setChild(Node *var) {_child = var;}
    inline Point getOffset() const {return _offset;}
    inline Node* getChild() const {return _child;}
private:
    Point _ratio;
    Point _offset;
    Node* _child;
};


Footer* Footer::create() {
    Footer* mFooter = new Footer();
    if(mFooter) {
        // Add it to autorelease pool
        mFooter->autorelease();
        mFooter->initFooter();
    } else {
        // Otherwise delete
        delete mFooter;
        mFooter = 0;
    }
    return mFooter;
}

void Footer::initFooter() {
    
    addImg("footer/mountain.png", 0.1);
    addImg("footer/mountain2.png", 0.12);
    addImg("footer/house.png", 0.5, 10, 50);
    addImg("footer/tree.png", 0.5, 100, 50);
    addImg("footer/wire.png", 0.6, 600, 50);
    
    auto rock = Sprite::create("footer/footer.png");
    rock->setAnchorPoint(Point::ZERO);
    addChild(rock, 0, Point(0.0, 0.4), Point(0,0));
}

void Footer::addImg(std::string str, float deepness, int x, int y) {
    // Create a sprite with rock texture
    auto rock = Sprite::create(str);
    rock->setAnchorPoint(Point::ZERO);
    addChild(rock, 0, Point(deepness, deepness), Point(x,y));
}

void Footer::updatePosition()
{
    int safeOffset = -10;
    // Get visible size
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // 1. For each child of an parallax node
    for(int i = 0; i < _children.size(); i++)
    {
        auto node = _children.at(i);
        // 2. We check whether it is out of the left side of the visible area
        if(convertToWorldSpace(node->getPosition()).x + node->getContentSize().width < safeOffset)
            // 3. Find PointObject that corresponds to current node
            for(int i = 0; i < _parallaxArray->num; i++)
            {
                auto po = (PointObject*)_parallaxArray->arr[i];
                // If yes increase its current offset on the value of visible width
                if(po->getChild() == node)
                    po->setOffset(po->getOffset() +
                                  Point(visibleSize.width + node->getContentSize().width,0));
            }
    }
}
