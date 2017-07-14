//
//  twistingKnot.hpp
//  eNTERFACE_17
//
//  Created by Jorge Barreiros on 14/07/17.
//
//

#ifndef twistingKnot_hpp
#define twistingKnot_hpp

#include <stdio.h>
#include "ofMain.h"

class twistingknot {
public:
    
    void setup();
    void update();
    void draw();
    void setNormals(ofMesh &mesh);
    void addRandomCircle(ofMesh &mesh);
    
    ofEasyCam cam;
    ofVboMesh mesh;    // mesh
    ofLight light;    // li
    
    // The circle parameters
    float Rad = 50; // Radius of circle
    float circleStep = 3;   //Step size for circle motion
    int circleN = 20;   //Number of points on the circle
    int count;
    
    float mapX, mapY;
    float noise;
    
    // Current circle state
    ofPoint pos;    //Circle center
    ofPoint axyX, axyY, axyZ;   //Circle's coordinates system

    
    twistingknot();
};

#endif /* twistingKnot_hpp */
