//
//  Node.hpp
//  deformableLattices
//
//  Created by Elliot Catalano on 9/5/18.
//  Copyright © 2018 Elliot Catalano. All rights reserved.
//

#ifndef Node_h
#define Node_h

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <map>

class Spring;

class Node{
public:
    GLfloat getX();
    GLfloat getY();
    GLfloat getZ();
    GLfloat getTotalForce();
    void addSpring(int springID, Spring *spring);
    void removeSpring(int springID);
    
private:
    GLfloat x, y, z;
    GLfloat totalForce;
    std::map <int, Spring *> connectedSprings;
};


#endif /* Node_h */
