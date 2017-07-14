//
//  controllingOscillatingObject.cpp
//  eNTERFACE_17
//
//  Created by Jorge Barreiros on 12/07/17.
//
//

#include "controllingOscillatingObject.hpp"
controllerOscillatingObject::controllerOscillatingObject(){
    
}

void controllerOscillatingObject::setup(){
    myOscillatingObject.setup();
    myMagnifyingGlass.setupMagnifyingGlass();
}

//--------------------------------------------------------------
void controllerOscillatingObject::update(){
    myOscillatingObject.update();
    myOscillatingObject.randomWalkingVertex();
}

//--------------------------------------------------------------
void controllerOscillatingObject::draw(){
    myOscillatingObject.draw();
}
