//
//  twistingKnot.cpp
//  eNTERFACE_17
//
//  Created by Jorge Barreiros on 14/07/17.
//
//

#include "twistingKnot.hpp"
twistingknot::twistingknot(){
    
}

void twistingknot::setup(){
    pos = ofPoint(0,0,0);   //Start from center of coordinate
    axyX = ofPoint(1,0,0); //Set initial coordinate system
    axyY = ofPoint(0,1,0);
    axyZ = ofPoint(0,0,1);
    light.enable(); //Enable lighting
    ofSetFrameRate(60); //Set the rate of screen redrawing
    //noise = 0.1;

}

void twistingknot::update(){
    addRandomCircle(mesh);
}

void twistingknot::draw(){
    ofEnableDepthTest();    //Enable z-buffering
    
    //Set a gradient background from white to gray
    ofBackground(0);//Gradient(ofColor(255), ofColor(128));
    
    //cam.begin();
    
    ofPushMatrix(); //Store the coordinate system
    //Move the coordinate center to creen's center
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5, 0);
    
    //Calculate teh rotation angle
    float time = ofGetElapsedTimef();   //Get time in seconds
    float angle = time * 20;    //Compute the angle. We rotate at speed 20 degrees per second
    ofRotate(angle,0,1,0);  //Rotate teh coordinate system along y-axis
    
    //Shift the coordinate center so the mesh will be drawn in the screen center
    ofTranslate(-mesh.getCentroid());
    
    //Draw the mesh
    //Here ofSetColor() does not affects the result of drawing, because the mesh has its own vertices colors
    mesh.draw();
    //cout << "Num Indices: " << mesh.getNumVertices() << endl;
    setNormals(mesh);
    ofPopMatrix();
    
    //cam.end();
 
}

void twistingknot::addRandomCircle(ofMesh &mesh){
    mesh.setMode(OF_PRIMITIVE_LINES);
    
    float time = ofGetElapsedTimef();   //Time
    
    //Parameters - twisting and rotating angles and color
    float twistAngle = 5.0 * ofSignedNoise(time * 0.3 + 332.4);
    float rotateAngle = 1.5;
    
    ofFloatColor color(ofNoise(time*0.6)*50,250);//ofNoise(time*0.05),
    //ofNoise(time*0.1),
    //ofNoise(time*0.15));
    
    color.setSaturation(1.6);   //Make the color maximally colorful
    
    //Rotate the coordinate system of the circle
    axyX.rotate(twistAngle, axyZ);
    axyY.rotate(twistAngle, axyZ);
    
    axyX.rotate(twistAngle, axyY);
    axyZ.rotate(twistAngle, axyY);
    
    
    
    //mapX = (float)ofMap(ofGetMouseX, 0, ofGetWidth(), -100,100);
    //mapX = ofMap(ofGetMouseX(), 0, ofGetWidth(), -10, 10);
    //mapY = ofMap(ofGetMouseY(), 0, ofGetHeight(), -10, 10);
    
    //Move the circle on a step
    //ofPoint moveX = axyX*circleStep;
    //ofPoint moveY = axyY*circleStep*0.1;
    ofPoint moveZ = axyZ*circleStep;
    //pos += moveX;
    //pos += moveY;
    pos += moveZ;
    
    
    
    //if(mesh.getNumVertices() <=100000){
    //Add vertices
    for(int i=0; i<circleN; i++){
        float angle = float(i) / circleN*TWO_PI;
        float x = Rad * cos( angle );
        float y = Rad * sin( angle );
        //We would like to distort this point to make the knot's surface embossed
        float distort = ofNoise(x * 0.2, y * 0.2, time * 0.2 + 30);
        
        noise +=0.1;
        distort = ofMap(distort, 0.2, 0.8, 0.8, 1.2);
        x*=distort;
        y*=distort;
        
        ofPoint p = axyX * x + axyY * y + pos;
        mesh.addVertex(p);
        mesh.addColor(color);
    }
    
    //Add the triangles
    int base = mesh.getNumVertices() - 2 * circleN;
    if( base >= 0 ){    // Check if it is not the first step and we really need to add teh triangles
        for(int i=0; i<circleN; i++){
            int a = base + i;
            int b = base + (i + 1) % circleN;
            int c = circleN + a;
            int d = circleN + b;
            
            mesh.addTriangle(a, b, d);  //Clock-wise
            mesh.addTriangle(a, d, c);
        }
        //Update the normals
        setNormals( mesh );
    }
    //}
}

void twistingknot::setNormals(ofMesh &mesh){
    //The number of the vertices
    int nV = mesh.getNumVertices();
    
    //The number of the triangles
    int nT = mesh.getNumIndices()/3;
    
    vector<ofPoint> norm(nV); // array for the normals
    
    //Scan all the triangles. For each triangle add its normal to norm's vectors of triangle's vertices
    for(int t=0; t<nT; t++){
        //Get indices of the triangle t
        int i1 = mesh.getIndex(3*t);
        int i2 = mesh.getIndex(3*t+1);
        int i3 = mesh.getIndex(3*t+2);
        
        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex(i1);
        const ofPoint &v2 = mesh.getVertex(i2);
        const ofPoint &v3 = mesh.getVertex(i3);
        
        //Compute the triangle's normal
        ofPoint dir = ((v2-v1).cross(v3-v1)).normalize();
        
        //Accumulate it to norm array for i1, i2, i3
        norm[i1] += dir;
        norm[i2] += dir;
        norm[i3] += dir;
    }
    
    //Normalize the normal's length
    for(int i=0; i<nV; i++){
        norm[i].normalize();
    }
    
    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals(norm);
}
