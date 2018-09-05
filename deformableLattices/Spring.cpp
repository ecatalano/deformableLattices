//
//  Spring.cpp
//  deformableLattices
//
//  Created by Elliot Catalano on 9/5/18.
//  Copyright © 2018 Elliot Catalano. All rights reserved.
//

#include "Spring.h"
#include "Node.h"
#include <math.h>

GLfloat Spring::getNaturalLength(){
    return  this->naturalLength;
}
void Spring::setNaturalLength(GLfloat naturalLength){
    this->naturalLength = naturalLength;
}
GLfloat Spring::getStiffness(){
    return this->stiffness;
}
void Spring::setStiffness(GLfloat stiffness){
    this->stiffness = stiffness;
}
Node * Spring::getNode1(){
    return this->node1;
}
void Spring::setNode1(Node *node1){
    this->node1 = node1;
}
Node * Spring::getNode2(){
    return this->node2;
}
void Spring::setNode2(Node *node2){
    this->node2 = node2;
}
GLfloat Spring::getSeparation(){
    GLfloat dx = (node1->getX() - node2->getX());
    GLfloat dy = (node1->getY() - node2->getY());
    GLfloat dz = (node1->getZ() - node2->getZ());
    return sqrt(dx * dx + dy * dy + dz * dz);
}
GLfloat Spring::getActualLength(){
    return abs(getSeparation());
}
GLfloat Spring::getDeformation(){
    return getActualLength() - naturalLength;
}
GLfloat Spring::getForce(){
    return ((stiffness * getDeformation()) / getActualLength()) * getSeparation();
}
