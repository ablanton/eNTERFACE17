//
//  oscillatingObject.hpp
//  emptyExample
//
//  Created by Jorge Barreiros on 11/07/17.
//
//

#ifndef oscillatingObject_hpp
#define oscillatingObject_hpp

#include <stdio.h>
#include "ofMain.h"

class oscillatingObject {
public:
    
    void setup();
    void update();
    void draw();
    void randomWalkingVertex();
    void setNormals(ofMesh &mesh);
    
    float w,h;
    
    float rotate;
    
    float r, g, b, a, time;
    float chance, angle, speedX, speedY;
    
    ofVboMesh mesh;
    ofLight light;
    ofImage img;
    ofPoint p;
    
    oscillatingObject();
    
private:
};

#endif /* oscillatingObject_hpp */
