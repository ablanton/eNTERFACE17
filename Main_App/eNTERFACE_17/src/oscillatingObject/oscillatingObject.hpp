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
    void setNormals(ofMesh &mesh);
    
    int w,h;
    
    float r, g, b, a, time;
    
    ofEasyCam cam;
    ofVboMesh mesh;
    ofLight light;
    ofImage img;
    
    oscillatingObject();
    
    float chance;
    float angle;
    float speedX, speedY;
};

#endif /* oscillatingObject_hpp */
