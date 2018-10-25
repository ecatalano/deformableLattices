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
    Node(GLfloat x, GLfloat y, GLfloat z);
    Node();
    ~Node();
    
    GLfloat getX();
    GLfloat getY();
    GLfloat getZ();
    
    GLfloat getDeltaX();
    GLfloat getDeltaY();
    GLfloat getDeltaZ();
    
    GLfloat getTotalForce();
    
    void setX(GLfloat newX);
    void setY(GLfloat newY);
    void setZ(GLfloat newZ);
    
    void setPropagated(bool propagated){ this->propagated = propagated; }
    
    void addSpring(int springID, Spring *spring);
    void removeSpring(int springID);

    void printAllSpringInfo();
    void setIsSelected(bool value){ isSelected = value; }
    bool getIsSelected(){ return isSelected; }
    
    bool didPropagate(){ return propagated; }
    
    void propagateMovementToNeighbors(GLfloat deltaX, GLfloat deltaY, GLfloat deltaZ);
    
    void resetPosition();
    
private:
    bool isSelected = false;
    GLfloat initialX, initialY, initialZ;
    bool propagated = false;
    GLfloat x, y, z;
    std::map <int, Spring *> connectedSprings;
};


#endif /* Node_h */
