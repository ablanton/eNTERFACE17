//
//  oscillatingObject.cpp
//  emptyExample
//
//  Created by Jorge Barreiros on 11/07/17.
//
//

#include "oscillatingObject.hpp"
oscillatingObject::oscillatingObject(){
    
}

void oscillatingObject::setup(){
    img.load("tex.jpg");
    
    //Grid Size
    w = 75;
    h = 150;
    
    for(int y=0; y<h; y++){
        for(int x=0; x<w; x++){
            mesh.addVertex(ofPoint((x-w)*10, (y-h)*10, 0));
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

}

void oscillatingObject::update(){
    float time = ofGetElapsedTimef();
    
    // Change Vertices
    for(int y=0; y<h; y++){
        for(int x=0; x<w; x++){
            int i = x + w * y; // formula para o index do vertice
            ofPoint p = mesh.getVertex(i);
            
            //Get perlin noise value
            float value = ofNoise(x * 0.05, y * 0.05, time * 0.5);
            
            //Change z coordinate of vertex
            p.z = value * ofMap(ofGetMouseX(),0,ofGetWidth(),20,500);
            mesh.setVertex(i, p);
            
            // Change color of vertex
            mesh.setColor(i, ofColor(value*255, value*255, value*255));
        }
    }
    setNormals(mesh);
}

void oscillatingObject::draw(){
    ofBackground(0);
    
    ofEnableDepthTest();
//    cam.begin();
    ofPushMatrix(); // store the coordinate system
    
    // Move the coordinate center to screen's center
    //ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5, 0);
    
    // Calculate the rotation angle
    float time = ofGetElapsedTimef();
    float angle = time;  // compoute angle. we rotate at speed 20 degrees per second
    
    // rotate coordenate system
    ofRotate(30, 1, 0, 0);
    ofRotate(angle, 0, 0, 1);
    
    ofTranslate(ofGetWidth(), ofGetHeight());
    
    //Draw mesh
    //Here ofSetColor() does not affects the result of drawing, because the mesh has its own vertices colors
    
    mesh.drawWireframe();
//    cam.end();
    
    ofPopMatrix();
    
    ofPushMatrix();
    
    //ofRotateX(90);
    float transX = ofMap(ofGetMouseX(),0,ofGetWidth(),-100,100);
    float transY = ofMap(ofGetMouseY(),0,ofGetHeight(),-100,100);
    
    cout<< "transX: " << transX << endl;
    cout<< "transY: " << transY << endl;
    
    //mesh.drawWireframe();
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
