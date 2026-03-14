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

// Array to sort
vector<int> arr = {
57,12,83,4,96,21,65,38,72,9,
44,1,78,30,55,18,90,63,27,41,
6,70,14,99,52,33,80,25,47,68,
2,74,59,36,88,11,94,23,61,45,
7,81,29,53,16,97,40,71,34,66,
19,84,48,3,92,60,13,75,32,58,
20,87,46,10,69,95,24,54,37,82,
8,73,15,64,50,91,42,28,77,35,
62,17,98,49,31,86,22,56,67,39,
5,76,43,26,79,93,51,89,85,100
};

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
        float height = arr[i] / 100.0f;

        if(i==currentA || i==currentB)
            glColor3f(1,0.2,0.2); // red for swap/highlight
        else
            glColor3f(0.2,0.8,1); // cyan normal

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

// Heapify function with visualization
void heapify(GLFWwindow* window, int n, int i)
{
    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if(left<n && arr[left] > arr[largest])
        largest = left;
    if(right<n && arr[right] > arr[largest])
        largest = right;

    if(largest != i)
    {
        swap(arr[i], arr[largest]);
        currentA = i;
        currentB = largest;

        glClear(GL_COLOR_BUFFER_BIT);
        drawBars();
        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(20));

        heapify(window, n, largest);
    }
}

// Heap Sort function
void heapSort(GLFWwindow* window)
{
    auto start = chrono::high_resolution_clock::now();

    int n = arr.size();

    // Build max heap
    for(int i = n/2 - 1; i >= 0; i--)
        heapify(window, n, i);

    // Extract elements from heap
    for(int i = n-1; i>=0; i--)
    {
        swap(arr[0], arr[i]);
        currentA = 0;
        currentB = i;

        glClear(GL_COLOR_BUFFER_BIT);
        drawBars();
        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(20));

        heapify(window, i, 0);
    }

    auto end = chrono::high_resolution_clock::now();
    sortTime = chrono::duration<double>(end-start).count();

    currentA = currentB = -1;
}

int main()
{
    if(!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800,600,"Heap Sort Visualization",NULL,NULL);
    if(!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glOrtho(-1,1,-1,1,-1,1);

    heapSort(window);

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