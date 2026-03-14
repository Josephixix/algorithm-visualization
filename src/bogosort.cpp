#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font.h"

using namespace std;

// Use a small array for Bogo Sort
vector<int> arr = {5, 2, 8, 1, 4, 3, 7, 6, 9, 10};

int currentA=-1;
int currentB=-1;
double sortTime = 0.0;

// Draw bars
void drawBars()
{
    float width = 2.0f / arr.size();
    glBegin(GL_QUADS);
    for(int i=0;i<arr.size();i++)
    {
        float x=-1.0f + i*width;
        float height = arr[i] / 10.0f; // scale for 10 elements

        if(i==currentA || i==currentB)
            glColor3f(1,0.2,0.2);
        else
            glColor3f(0.2,0.8,1);

        glVertex2f(x,-1);
        glVertex2f(x+width,-1);
        glVertex2f(x+width,-1+height*2);
        glVertex2f(x,-1+height*2);
    }
    glEnd();
}

// Draw text
void drawText(int x, int y, string text)
{
    char buffer[99999];
    int num_quads = stb_easy_font_print(x,y,(char*)text.c_str(),NULL,buffer,sizeof(buffer));

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,800,600,0,-1,1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1,1,1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2,GL_FLOAT,16,buffer);
    glDrawArrays(GL_QUADS,0,num_quads*4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Check if array is sorted
bool isSorted()
{
    for(int i=0;i<arr.size()-1;i++)
        if(arr[i]>arr[i+1])
            return false;
    return true;
}

// Bogo Sort with visualization
void bogoSort(GLFWwindow* window)
{
    auto start = chrono::high_resolution_clock::now();
    random_device rd;
    mt19937 g(rd());

    while(!isSorted())
    {
        shuffle(arr.begin(),arr.end(),g);

        currentA=-1; // not highlighting specific elements
        currentB=-1;

        glClear(GL_COLOR_BUFFER_BIT);
        drawBars();
        glfwSwapBuffers(window);
        glfwPollEvents();

        this_thread::sleep_for(chrono::milliseconds(200)); // slower to see the shuffle
    }

    auto end = chrono::high_resolution_clock::now();
    sortTime = chrono::duration<double>(end-start).count();
}

int main()
{
    if(!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800,600,"Bogo Sort Visualization",NULL,NULL);
    if(!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glOrtho(-1,1,-1,1,-1,1);

    bogoSort(window);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        drawBars();

        string timeText = "Sort Time: " + to_string(sortTime).substr(0,5) + " s";
        drawText(10,20,timeText);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}