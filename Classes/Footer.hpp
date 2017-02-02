//
//  footer.hpp
//  MyGame
//
//  Created by Daniel Feles on 2/2/17.
//
//

#ifndef footer_hpp
#define footer_hpp

#include <stdio.h>

class Footer : public cocos2d::ParallaxNode
{
public:
    Footer();
    ~Footer();
    static Footer* create();
    void initFooter();
    void updatePosition();
private:
    void addImg(std::string str, float deepness, int x = 0, int y = 0) ;
};
#endif /* footer_hpp */
