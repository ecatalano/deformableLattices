//
//  Node.cpp
//  deformableLattices
//
//  Created by Elliot Catalano on 9/5/18.
//  Copyright © 2018 Elliot Catalano. All rights reserved.
//

#include "Node.h"
#include "Spring.h"
#include <cmath>

Node::Node(GLfloat x, GLfloat y, GLfloat z){
    this->x = x;
    this->y = y;
    this->z = z;
    
    this->initialX = x;
    this->initialY = y;
    this->initialZ = z;
}

GLfloat Node::getX(){
    return this->x;
}
GLfloat Node::getY(){
    return this->y;
}
GLfloat Node::getZ(){
    return this->z;
}

void Node::setX(GLfloat newX){
    this->x = newX;
}

void Node::setY(GLfloat newY){
    this->y = newY;
}

void Node::setZ(GLfloat newZ){
    this->z = newZ;
}

GLfloat Node::getDeltaX(){
    return abs(this->initialX - this->x);
}
GLfloat Node::getDeltaY(){
    return abs(this->initialY - this->y);
}
GLfloat Node::getDeltaZ(){
    return abs(this->initialZ - this->z);
}

void Node::propagateMovementToNeighbors(GLfloat deltaX, GLfloat deltaY, GLfloat deltaZ){
    std::map<int, Spring *>::iterator i;
    
    if(deltaX == 0 && deltaY == 0.0 && deltaZ == 0.0) return;
    
    this->propagated = true;
    
    GLfloat adjustment = .25;
    
    GLfloat newDeltaX = adjustment * deltaX;
    GLfloat newDeltaY = adjustment * deltaY;
    GLfloat newDeltaZ = adjustment * deltaZ;
    
    for(i = connectedSprings.begin(); i != connectedSprings.end(); i++){
        
        Spring *tempSpring = (Spring *)i->second;
        Node *node1 = tempSpring->getNode1();
        Node *node2 = tempSpring->getNode2();

        if(node1 != this){
            if(!node1->didPropagate()){
                node1->setX(node1->getX()+newDeltaX);
                node1->setY(node1->getY()+newDeltaY);
                node1->setZ(node1->getZ()+newDeltaZ);
            }
        }
        else{
            if(!node2->didPropagate()){
                node2->setX(node2->getX()+newDeltaX);
                node2->setY(node2->getY()+newDeltaY);
                node2->setZ(node2->getZ()+newDeltaZ);
            }
        }
    }
    
    for(i = connectedSprings.begin(); i != connectedSprings.end(); i++){
        
        Spring *tempSpring = (Spring *)i->second;
        Node *node1 = tempSpring->getNode1();
        Node *node2 = tempSpring->getNode2();
        
        if(node1 != this){
            if(!node1->didPropagate()){
                node1->propagateMovementToNeighbors(newDeltaX, newDeltaY, newDeltaZ);
            }
        }
        else{
            if(!node2->didPropagate()){
                node2->propagateMovementToNeighbors(newDeltaX, newDeltaY, newDeltaZ);
            }
        }
    }
}

void Node::resetPosition(){
    this->x = initialX;
    this->y = initialY;
    this->z = initialZ;
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

void Node::printAllSpringInfo(){
    std::map<int, Spring *>::iterator it;
    for (it = connectedSprings.begin(); it != connectedSprings.end(); ++it){
        Spring *tempSpring = it->second;
        
        
        int springID = tempSpring->getSpringID();
        GLfloat separation = tempSpring->getSeparation();
        GLfloat actualLength = tempSpring->getActualLength();
        GLfloat deformation = tempSpring->getDeformation();
        GLfloat force = tempSpring->getForce();
        GLfloat naturalLength = tempSpring->getNaturalLength();
        
        if(separation == naturalLength) continue;
        
        printf("-------Spring %d-------\n", springID);
        printf("Length: %f\n", actualLength);
        printf("Natural Length: %f\n", naturalLength);

        printf("deformation: %f\n", deformation);
        printf("force: %f\n", force);
        printf("-----------------------\n");

    }
}
