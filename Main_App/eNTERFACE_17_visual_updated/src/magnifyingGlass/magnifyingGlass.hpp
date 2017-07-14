//
//  magnifyingGlass.hpp
//  eNTERFACE_17
//
//  Created by Jorge Barreiros on 12/07/17.
//
//

#ifndef magnifyingGlass_hpp
#define magnifyingGlass_hpp

#include <stdio.h>
#include "ofMain.h"

class magnifyingGlass {
public:
    
    void setupMagnifyingGlass();
    void magnifyingGlassEffect();
    void toggleMagnifyingGlass();
    
    ofMesh mesh;
    ofMesh meshCopy;
    
    bool mouseDisplacement;
    bool testAction;
    
    
    magnifyingGlass();
    
private:
};

#endif /* magnifyingGlass_hpp */
