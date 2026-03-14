#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cmath>
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

        if(i>=highlightStart && i<=highlightEnd)
            glColor3f(1,0.2,0.2); // red for current digit processing
        else
            glColor3f(0.2,0.8,1); // cyan

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

// Get maximum number in array
int getMax()
{
    return *max_element(arr.begin(), arr.end());
}

// Counting sort for each digit
void countingSort(GLFWwindow* window, int exp)
{
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0};

    // Count occurrences of digits
    for(int i=0;i<n;i++)
    {
        int digit = (arr[i]/exp)%10;
        count[digit]++;
    }

    // Change count[i] so that count[i] contains actual position
    for(int i=1;i<10;i++) count[i]+=count[i-1];

    // Build output array
    for(int i=n-1;i>=0;i--)
    {
        int digit = (arr[i]/exp)%10;
        output[count[digit]-1]=arr[i];
        count[digit]--;

        highlightStart = 0;
        highlightEnd = n-1;

        glClear(GL_COLOR_BUFFER_BIT);
        drawBars();
        glfwSwapBuffers(window);
        glfwPollEvents();
        this_thread::sleep_for(chrono::milliseconds(20));
    }

    // Copy output to arr
    for(int i=0;i<n;i++) arr[i] = output[i];
}

// Radix Sort
void radixSort(GLFWwindow* window)
{
    auto start = chrono::high_resolution_clock::now();

    int m = getMax();

    for(int exp=1;m/exp>0;exp*=10)
    {
        countingSort(window,exp);
    }

    auto end = chrono::high_resolution_clock::now();
    sortTime = chrono::duration<double>(end-start).count();
    highlightStart = highlightEnd = -1;
}

int main()
{
    if(!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800,600,"Radix Sort Visualization",NULL,NULL);
    if(!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glOrtho(-1,1,-1,1,-1,1);

    radixSort(window);

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