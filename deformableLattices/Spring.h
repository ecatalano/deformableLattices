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
public:
    Spring(int springID, GLfloat naturalLength, GLfloat stiffness, Node *node1, Node *node2);
    Spring();
    ~Spring();
    
    GLfloat getNaturalLength();
    void setNaturalLength(GLfloat naturalLength);
    
    GLfloat getStiffness();
    void setStiffness(GLfloat stiffness);
    
    Node *getNode1();
    void setNode1(Node *node1);
    
    Node *getNode2();
    void setNode2(Node *node2);

    int getSpringID();
    void setSpringID(int springID);
    
    GLfloat getSeparation();
    GLfloat getActualLength();
    GLfloat getForce();
    GLfloat getDeformation();
    
private:
    int springID;
    GLfloat naturalLength;
    GLfloat stiffness;
    Node *node1;
    Node *node2;
};

#endif /* Spring_h */
