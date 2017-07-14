//
//  controllingOscillatingObject.hpp
//  eNTERFACE_17
//
//  Created by Jorge Barreiros on 12/07/17.
//
//

#ifndef controllingOscillatingObject_hpp
#define controllingOscillatingObject_hpp

#include <stdio.h>
#include "ofMain.h"
#include "oscillatingObject.hpp"
#include "magnifyingGlass.hpp"

class controllerOscillatingObject {
public:
    
    void setup();
    void update();
    void draw();
    
    controllerOscillatingObject();
    
    oscillatingObject myOscillatingObject;
    magnifyingGlass myMagnifyingGlass;
};


#endif /* controllingOscillatingObject_hpp */
