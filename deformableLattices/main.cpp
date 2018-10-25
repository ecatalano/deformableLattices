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
#include "glm.hpp"

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

std::vector < glm::vec3 > vertices;
std::vector < glm::vec2 > uvs;
std::vector < glm::vec3 > normals;

void drawNode(Node *node);
void drawSpring(Spring *spring);
void display();

void printSelectedNodeSpringInfo(){
    for(int i = 0; i < selectedNodes.size(); i++){
        selectedNodes.at(i)->printAllSpringInfo();
    }
}

void clearAllPropagationFlags(){
    for(int i = 0; i < nodes.size(); i++){
        nodes.at(i)->setPropagated(false);
    }
}


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


bool loadOBJ(const char * path,
                    std::vector < glm::vec3 > & out_vertices,
                    std::vector < glm::vec2 > & out_uvs,
                    std::vector < glm::vec3 > & out_normals) {
    
    //Based off of http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
    
    //temporary variables in which we will store the contents of the .obj :
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    
    //Read from file
    
    FILE *file;
    file = fopen(path, "r");
    //fopen_s(&file, path, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", path);
        return false;
    }
    //Read until EOF
    while (1) {
        char lineHeader[128];
        
        //read first word of line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;        //deal with vertices
        
        
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches == 9) {
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
        }
    }
    //Processing the data
    for (int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    for (int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = temp_uvs[uvIndex - 1];
        out_uvs.push_back(uv);
    }
    for (int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[normalIndex - 1];
        out_normals.push_back(normal);
    }
    return true;
}


void init(){

    const char *objString = "0019.obj";
    
    bool load = loadOBJ(objString, vertices, uvs, normals);
    
    for(int i = 0; i < vertices.size(); i++){
        
        Node *tempNode = new Node(vertices.at(i)[0], vertices.at(i)[1], vertices.at(i)[2]);
        nodes.push_back(tempNode);
    }

    /*Node *node1 = new Node(0.0, -30.0, -30.0);
    Node *node2 = new Node (0.0 , 30.0, 30.0);
    Node *node3 = new Node (0.0 , -30.0, 30.0);
    Node *node4 = new Node (0.0 , 30.0, -30.0);
    
    
    Spring *spring1 = new Spring(1, 1.0, node1, node2);
    Spring *spring2 = new Spring(2, 1.0, node1, node3);
    Spring *spring3 = new Spring(3, 1.0, node1, node4);
    Spring *spring4 = new Spring(4, 1.0, node3, node4);
    Spring *spring5 = new Spring(5, 1.0, node2, node4);
    Spring *spring6 = new Spring(6, 1.0, node2, node3);

    nodes.push_back(node1);
    nodes.push_back(node2);
    nodes.push_back(node3);
    nodes.push_back(node4);
    
    springs.push_back(spring1);
    springs.push_back(spring2);
    springs.push_back(spring3);
    springs.push_back(spring4);
    springs.push_back(spring5);
    springs.push_back(spring6);*/

}

void display(void){
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 250, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    
    for(int i = 0; i < springs.size(); i++){
        drawSpring(springs.at(i));
    }
    
    for(int i = 0; i < nodes.size(); i++){
        drawNode(nodes.at(i));
    }

    glutSwapBuffers();
    //printSelectedNodeSpringInfo();
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
    
    //glVertex3f(x, y, z);
    
    glutSolidSphere(.2, 5.0, 5.0);
    
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
            node->propagateMovementToNeighbors(value, 0.0, 0.0);
            break;
        case 'y':
            node->setY(node->getY()+value);
            node->propagateMovementToNeighbors(0.0, value, 0.0);
            break;
        case 'z':
            node->setZ(node->getZ()+value);
            node->propagateMovementToNeighbors(0.0, 0.0, value);
            break;
    }
    clearAllPropagationFlags();
    
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
        case 'r':
            for(int i = 0; i < nodes.size(); i++){
                Node *tempNode = nodes.at(i);
                tempNode->resetPosition();
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
