//
//  Node.cpp
//  deformableLattices
//
//  Created by Elliot Catalano on 9/5/18.
//  Copyright © 2018 Elliot Catalano. All rights reserved.
//

#include "Node.h"
#include "Spring.h"

GLfloat Node::getX(){
    return this->x;
}
GLfloat Node::getY(){
    return this->y;
}
GLfloat Node::getZ(){
    return this->z;
}
GLfloat Node::getTotalForce(){
    //Total force = sum of force from each connected spring
    std::map<int, Spring *>::iterator i;
    GLfloat totalForce = 0.0;
    
    for(i = connectedSprings.begin(); i != connectedSprings.end(); i++){
        Spring *tempSpring = (Spring *)i->second;
        totalForce+=tempSpring->getForce();
    }
    return totalForce;
}
void Node::addSpring(int springID, Spring *spring){
    connectedSprings[springID] = spring;
}

void Node::removeSpring(int springID){
    connectedSprings.erase(connectedSprings.find(springID));
}
