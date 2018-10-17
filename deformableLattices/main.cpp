//
//  main.cpp
//  deformableLattices
//
//  Created by Elliot Catalano on 9/5/18.
//  Copyright © 2018 Elliot Catalano. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <array>

#include "Node.h"
#include "Spring.h"

int windowHeight = 500;
int windowWidth = 500;

GLfloat sensitivity = 10.0;

char mode = 'x';
std::vector<Node *> nodes;
std::vector<Spring *> springs;
std::vector<std::array<int, 2>> nodeCoords;
std::vector<Node *> selectedNodes;


void drawNode(Node *node);
void drawSpring(Spring *spring);
void display();

void setScreenCoords(int i) {
    float tempX = nodes[i]->getX();
    float tempY = nodes[i]->getY();
    float tempZ = nodes[i]->getZ();

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble viewVector[3];
    GLdouble projection[16];
    
    GLdouble winX, winY, winZ;//2D point
    
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    
    viewVector[0] = modelview[8];
    viewVector[1] = modelview[9];
    viewVector[2] = modelview[10];
    
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    gluProject(tempX, tempY, tempZ, modelview, projection, viewport, &winX, &winY, &winZ);
    
    int x = (int)winX;
    int y = (int)winY;
    std::array<int, 2> tempArray {x, y};

    nodeCoords.push_back(tempArray);
}

void updateScreenCoords(){
    nodeCoords.clear();

    for(int i = 0; i < nodes.size(); i++){
        setScreenCoords(i);
    }
}

Node *closestNodeToScreenCoord(GLfloat x, GLfloat y) {
    int temp_i = 0;
    
    int tempX = nodeCoords[temp_i][0];
    int tempY = nodeCoords[temp_i][1];
    
    int prod1 = (tempX - x) * (tempX - x);
    int prod2 = (tempY - y) * (tempY - y);
    
    float shortestDistance = sqrt(prod1 + prod2);
    
    //TODO: IMPLEMENT DIVIDE AND CONQUER STRATEGY
    for (int i = 0; i < nodeCoords.size(); i++) {
        tempX = nodeCoords[i][0];
        tempY = nodeCoords[i][1];
            
        int prod1 = (tempX - x) * (tempX - x);
        int prod2 = (tempY - y) * (tempY - y);
            
        float distance = sqrt(prod1 + prod2);
            
        if (distance < shortestDistance) {
            temp_i = i;
            shortestDistance = distance;
        }
        
    }
    return nodes[temp_i];
}


void init(){

    Node *node1 = new Node(0.0, -30.0, -30.0);
    Node *node2 = new Node (0.0 , 30.0, 30.0);
    Node *node3 = new Node (0.0 , -30.0, 30.0);
    Node *node4 = new Node (0.0 , 30.0, -30.0);
    
    
    Spring *spring1 = new Spring(1, 1.0, 1.0, node1, node2);
    Spring *spring2 = new Spring(1, 1.0, 1.0, node1, node3);
    Spring *spring3 = new Spring(1, 1.0, 1.0, node1, node4);
    Spring *spring4 = new Spring(1, 1.0, 1.0, node3, node4);
    Spring *spring5 = new Spring(1, 1.0, 1.0, node2, node4);
    Spring *spring6 = new Spring(1, 1.0, 1.0, node2, node3);


    nodes.push_back(node1);
    nodes.push_back(node2);
    nodes.push_back(node3);
    nodes.push_back(node4);
    
    springs.push_back(spring1);
    springs.push_back(spring2);
    springs.push_back(spring3);
    springs.push_back(spring4);
    springs.push_back(spring5);
    springs.push_back(spring6);
    
    
    /*Node *node5 = new Node(30.0, -30.0, -30.0);
    Node *node6 = new Node (30.0 , 30.0, 30.0);
    Node *node7 = new Node (30.0 , -30.0, 30.0);
    Node *node8 = new Node (30.0 , 30.0, -30.0);

    Spring *spring7 = new Spring(1, 1.0, 1.0, node5, node6);
    Spring *spring8 = new Spring(1, 1.0, 1.0, node5, node7);
    Spring *spring9 = new Spring(1, 1.0, 1.0, node5, node8);
    Spring *spring10 = new Spring(1, 1.0, 1.0, node6, node7);
    Spring *spring11 = new Spring(1, 1.0, 1.0, node7, node8);
    Spring *spring12 = new Spring(1, 1.0, 1.0, node6, node8);
    
    Spring *spring13 = new Spring(1, 1.0, 1.0, node1, node5);
    Spring *spring14 = new Spring(1, 1.0, 1.0, node2, node5);
    Spring *spring15 = new Spring(1, 1.0, 1.0, node3, node5);
    Spring *spring16 = new Spring(1, 1.0, 1.0, node4, node5);
    
    Spring *spring17 = new Spring(1, 1.0, 1.0, node1, node6);
    Spring *spring18 = new Spring(1, 1.0, 1.0, node2, node6);
    Spring *spring19 = new Spring(1, 1.0, 1.0, node3, node6);
    Spring *spring20 = new Spring(1, 1.0, 1.0, node4, node6);
    
    Spring *spring21 = new Spring(1, 1.0, 1.0, node1, node7);
    Spring *spring22 = new Spring(1, 1.0, 1.0, node2, node7);
    Spring *spring23 = new Spring(1, 1.0, 1.0, node3, node7);
    Spring *spring24 = new Spring(1, 1.0, 1.0, node4, node7);
    
    Spring *spring25 = new Spring(1, 1.0, 1.0, node1, node8);
    Spring *spring26 = new Spring(1, 1.0, 1.0, node2, node8);
    Spring *spring27 = new Spring(1, 1.0, 1.0, node3, node8);
    Spring *spring28 = new Spring(1, 1.0, 1.0, node4, node8);
    
    nodes.push_back(node5);
    nodes.push_back(node6);
    nodes.push_back(node7);
    nodes.push_back(node8);
    
    springs.push_back(spring7);
    springs.push_back(spring8);
    springs.push_back(spring9);
    springs.push_back(spring10);
    springs.push_back(spring11);
    springs.push_back(spring12);
    
    springs.push_back(spring13);
    springs.push_back(spring14);
    springs.push_back(spring15);
    springs.push_back(spring16);
    springs.push_back(spring17);
    springs.push_back(spring18);
    springs.push_back(spring19);
    springs.push_back(spring20);
    
    springs.push_back(spring21);
    springs.push_back(spring22);
    springs.push_back(spring23);
    springs.push_back(spring24);
    springs.push_back(spring25);
    springs.push_back(spring26);
    springs.push_back(spring27);
    springs.push_back(spring28);*/

}

void display(void){
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(100.0, 0.0, 0, 0, 0, 0, 0.0, 1.0, 0.0);

    
    for(int i = 0; i < springs.size(); i++){
        drawSpring(springs.at(i));
    }
    
    for(int i = 0; i < nodes.size(); i++){
        drawNode(nodes.at(i));
    }

    glutSwapBuffers();
    

}

void drawNode(Node *node) {
    
    GLfloat x = node->getX();
    GLfloat y = node->getY();
    GLfloat z = node->getZ();
    glPushMatrix();

    glPointSize(8.0);
    glBegin(GL_MODELVIEW);
    glTranslatef(x, y, z);

    if(node->getIsSelected()) glColor3f(0.0, 1.0, 0.0);
    else glColor3f(1.0, 0.0, 0.0);
    
    glutSolidSphere(1.0, 5.0, 5.0);
    
    glEnd();
    glPopMatrix();
}

void drawSpring (Spring *spring){
    glPushMatrix();
    
    Node *node1 = spring->getNode1();
    Node *node2 = spring->getNode2();
    
    GLfloat x1 = node1->getX();
    GLfloat y1 = node1->getY();
    GLfloat z1 = node1->getZ();
    
    GLfloat x2 = node2->getX();
    GLfloat y2 = node2->getY();
    GLfloat z2 = node2->getZ();
    
    glBegin(GL_LINES);
    
    glColor3f(0.0, 0.0, 1.0);
    
    glLineWidth(0.5);
    
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);

    glEnd();
    
    glPopMatrix();
}


void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 10000);
    glMatrixMode(GL_MODELVIEW);
}

void selectNode(int x, int y) {
    Node *tempNode = closestNodeToScreenCoord(x, y);
    
    if(!tempNode->getIsSelected()){
        tempNode->setIsSelected(true);
        selectedNodes.push_back(tempNode);
    }
    else{
        tempNode->setIsSelected(false);
        selectedNodes.erase(std::remove(selectedNodes.begin(), selectedNodes.end(), tempNode), selectedNodes.end());
    }
    
    display();
}

void deselectNode(int x, int y){
    Node *tempNode = closestNodeToScreenCoord(x, y);
    tempNode->setIsSelected(false);
    selectedNodes.erase(std::remove(selectedNodes.begin(), selectedNodes.end(), tempNode), selectedNodes.end());

    display();
}

void onMouseClick(int button, int state, int x, int y)
{
    updateScreenCoords();
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                /*if(mode == 's'){
                    selectNode(x, windowHeight - y);
                    break;
                }
                else if(mode == 'd'){
                    deselectNode(x, windowHeight - y);
                    break;
                }*/
                selectNode(x, windowHeight - y);
            }
    }
}

void changeNodeCoord(Node *node, GLfloat value){
    switch(mode){
        case 'x':
            node->setX(node->getX()+value);
            break;
        case 'y':
            node->setY(node->getY()+value);
            break;
        case 'z':
            node->setZ(node->getZ()+value);
            break;
    }
    
}

void keyboardFunction(unsigned char key, int x, int y) {
    std::string modeStr;
    switch (key) {
        case 'x':
            mode = 'x';
            modeStr = "x";
            break;
        case 'y':
            mode = 'y';
            modeStr = "y";
            break;
        case 'z':
            mode = 'z';
            modeStr = "z";
            break;
        case '=':
            for(int i = 0; i < selectedNodes.size(); i++){
                Node *tempNode = selectedNodes.at(i);
                changeNodeCoord(tempNode, sensitivity);
            }
            display();

            break;
        case '-':
            for(int i = 0; i < selectedNodes.size(); i++){
                Node *tempNode = selectedNodes.at(i);
                changeNodeCoord(tempNode, -sensitivity);
            }
            display();

            break;
    }
    if(modeStr!="") std::cout << "Current Mode: " << modeStr << std::endl;
}

int main(int argc, char** argv) {
    init();
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (windowWidth, windowHeight);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);

    glutDisplayFunc(display);
    glutMouseFunc(onMouseClick);
    glutKeyboardFunc(keyboardFunction);

    glOrtho(0.0, 1.0, 1.0, 0.0, -50, 50);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
    
}
