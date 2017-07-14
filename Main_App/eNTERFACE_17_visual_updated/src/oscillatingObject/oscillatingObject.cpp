//
//  oscillatingObject.cpp
//  emptyExample
//
//  Created by Jorge Barreiros on 11/07/17.
//
//test

#include "oscillatingObject.hpp"
oscillatingObject::oscillatingObject(){
    
}

void oscillatingObject::setup(){
    
    //Grid Size
    w = 75;
    h = 150;
    
    rotate = 0;
    
    for(int y=0; y<h; y++){
        for(int x=0; x<w; x++){
            mesh.addVertex(ofPoint((x-w)*25, (y-h)*25, 0));
            mesh.addColor(ofColor(255,255,255));
        }
    }
    
    // Set up triangle indices
    for(int y=0; y<h-1; y++){
        for(int x=0; x<w-1; x++){
            int i1 = x + w * y;
            int i2 = x+1 + w * y;
            int i3 = x + w * (y+1);
            int i4 = x+1 + w * (y+1);
            mesh.addTriangle(i1, i2, i3);
            mesh.addTriangle(i2, i4, i3);
        }
    }
    setNormals(mesh);
    light.enable();
    angle = 0;
}

void oscillatingObject::update(){
    float time = ofGetElapsedTimef();
    
    // Change Vertices
    for(int y=0; y<h; y++){
        for(int x=0; x<w; x++){
            int i = x + w * y; // formula para o index do vertice
            p = mesh.getVertex(i);
            
            //Get perlin noise value
            float value = ofNoise(x * sin(ofGetElapsedTimef()) * 10.55, y * sin(ofGetElapsedTimef()) * 0.35, time * 0.75);
            
            //Change z coordinate of vertex
            p.z = value * ofMap(ofGetMouseX(),0,ofGetWidth(),20,350);
            //p.y = value * ofMap(ofGetMouseY(),0,ofGetWidth(),0.1,125);
            //p.x = value * ofMap(ofGetMouseY(),0,ofGetHeight(),20,500);
            mesh.setVertex(i, p);
            
            
            r = value*sin(ofGetElapsedTimef())*27.5+227.5;
            g = value*sin(ofGetElapsedTimef())*25+175;
            b = value*sin(ofGetElapsedTimef())*77.5+177.5;
            a = 255;

            // Change color of vertex
            mesh.setColor(i, ofColor(r, g, b, a));
        }
    }
    setNormals(mesh);
}

void oscillatingObject::randomWalkingVertex(){
    chance = ofRandom(1);
    
    speedX =20.25;
    speedY =20.25;
    
    if(chance < 0.25){
        p.x+=speedX;
    } else if(chance < 0.5){
        p.x-=speedX;
    } else if (chance < 0.75){
        p.y+=speedY;
    } else {
        p.y-=speedY;
    }

}

void oscillatingObject::draw(){
    ofColor centerColor = ofColor(255, 255, 255);
    ofColor edgeColor(0,0,0);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);
    
    ofEnableDepthTest();
    
    ofPushMatrix(); // store the coordinate system
    
    // Move the coordinate center
    ofTranslate(ofGetWidth(), ofGetHeight());
    
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    setNormals(mesh);
    
    mesh.draw();
    
    ofPopMatrix();
}

void oscillatingObject::setNormals(ofMesh &mesh){
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
