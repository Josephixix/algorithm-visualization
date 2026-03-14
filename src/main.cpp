#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
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

int highlightStart = -1;
int highlightEnd = -1;
double sortTime = 0.0;

// Draw vertical bars
void drawBars()
{
    float width = 2.0f / arr.size();

    glBegin(GL_QUADS);
    for(int i=0;i<arr.size();i++)
    {
        float x=-1.0f + i*width;
        float height = arr[i] / 100.0f;

        if(i >= highlightStart && i <= highlightEnd)
            glColor3f(1,0.2,0.2); // red for currently merging
        else
            glColor3f(0.2,0.8,1); // cyan for rest

        glVertex2f(x,-1);
        glVertex2f(x+width,-1);
        glVertex2f(x+width,-1+height*2);
        glVertex2f(x,-1+height*2);
    }
    glEnd();
}

// Draw text using stb_easy_font in pixel coordinates
void drawText(int x, int y, string text)
{
    char buffer[99999];
    int num_quads = stb_easy_font_print(x, y, (char*)text.c_str(), NULL, buffer, sizeof(buffer));

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1); // pixel coordinates

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1,1,1);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads*4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Merge function with visualization
void merge(GLFWwindow* window, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for(int i=0;i<n1;i++) L[i] = arr[left+i];
    for(int j=0;j<n2;j++) R[j] = arr[mid+1+j];

    int i=0,j=0,k=left;
    while(i<n1 && j<n2)
    {
        highlightStart = left;
        highlightEnd = right;

        if(L[i]<=R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];

        glClear(GL_COLOR_BUFFER_BIT);
        drawBars();
        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(20));
    }

    while(i<n1)
    {
        arr[k++] = L[i++];
        glClear(GL_COLOR_BUFFER_BIT);
        drawBars();
        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(20));
    }

    while(j<n2)
    {
        arr[k++] = R[j++];
        glClear(GL_COLOR_BUFFER_BIT);
        drawBars();
        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
}

// Recursive Merge Sort
void mergeSort(GLFWwindow* window, int left, int right)
{
    if(left<right)
    {
        int mid = left + (right-left)/2;
        mergeSort(window,left,mid);
        mergeSort(window,mid+1,right);
        merge(window,left,mid,right);
    }
}

// Wrapper to measure time
void runMergeSort(GLFWwindow* window)
{
    auto start = chrono::high_resolution_clock::now();
    mergeSort(window,0,arr.size()-1);
    auto end = chrono::high_resolution_clock::now();
    sortTime = chrono::duration<double>(end-start).count();
    highlightStart = highlightEnd = -1;
}

int main()
{
    if(!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800,600,"Merge Sort Visualization",NULL,NULL);
    if(!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glOrtho(-1,1,-1,1,-1,1);

    runMergeSort(window);

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