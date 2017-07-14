//
//  magnifyingGlass.cpp
//  eNTERFACE_17
//
//  Created by Jorge Barreiros on 12/07/17.
//
//

#include "magnifyingGlass.hpp"

magnifyingGlass::magnifyingGlass(){
    
}

void magnifyingGlass::setupMagnifyingGlass(){
    meshCopy = mesh;
    
    // queremos começar com o efeito desligado.
    mouseDisplacement = false;
    testAction = false;
}

void magnifyingGlass::magnifyingGlassEffect(){
    if(mouseDisplacement){
        // Temos que encontrar a localização do rato, tem de ser relativo ao centro do ecrã por causa do oftranslate() no draw():
        ofVec3f mouse(ofGetMouseX(), ofGetMouseY(), 0);
        
        // Fazemos um loop por todos os vértices na mesh e movemo los para longe do rato.
        for(int i=0; i<mesh.getNumVertices(); ++i){
            ofVec3f vertex = meshCopy.getVertex(i);
            float distanceToMouse = mouse.distance(vertex);
            
            // Vamos escalar o deslocamento baseado na distância até ao rato. Uma distÂncia pequena para o rato deve ter um pequeno deslocamento.
            float displacement = ofMap(distanceToMouse,0,200, 100.0, 0.0, true);
            // Calculamos a direção do rato ao vertex atual
            ofVec3f direction = vertex - mouse;
            
            // Normalizamos a direção para que que tenha um comprimento de 1. Isto permite-nos mudar facilmente o comprimento do vector mais tarde.
            direction.normalize();
            // Empurramos o vertex na direção para longe do rato e empurra mos a uma distancia igual à da variavel displacement.
            ofVec3f displacedVertex = vertex + displacement * direction;
            mesh.setVertex(i, displacedVertex);
        }
    }
}

void magnifyingGlass::toggleMagnifyingGlass(){
    if(testAction == true){
        mouseDisplacement = !mouseDisplacement;
        mesh = meshCopy;
    }
}
