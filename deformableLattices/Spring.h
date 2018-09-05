//
//  Spring.hpp
//  deformableLattices
//
//  Created by Elliot Catalano on 9/5/18.
//  Copyright © 2018 Elliot Catalano. All rights reserved.
//

#ifndef Spring_h
#define Spring_h

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <map>

class Node;

class Spring{
    GLfloat naturalLength;
    GLfloat stiffness;
    Node *node1;
    Node *node2;
    GLfloat vectorSeparation;
    GLfloat actualLength;
    GLfloat force;
    GLfloat deformation;
};

#endif /* Spring_h */
