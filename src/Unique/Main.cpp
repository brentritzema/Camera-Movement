// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// GLEW to handle extensions
#include <GL/glew.h>

// OpenGL for Graphics
#include <Common/OpenGL.hpp>

// GLFW for windowing and input
#include <GL/glfw.h>

// Include GLM for math
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Common/Shader.hpp>

//Some Functions
int Init();

// An array of 3 vectors which represents 3 vertices
GLfloat Vertices[] = {
      -0.8f, -0.8f, 0.0f, 1.0f,
      0.0f,  0.8f, 0.0f, 1.0f,
      0.8f, -0.8f, 0.0f, 1.0f
   };

   GLfloat Colors[] = {
      1.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f, 1.0f
   };

// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
// Camera matrix
   glm::mat4 View       = glm::lookAt(
         glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
         glm::vec3(0,0,0), // and looks at the origin
         glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
         );
   // Model matrix : an identity matrix (model will be at the origin)
   glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !
   // Our ModelViewProjection : multiplication of our 3 matrices
   glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around



int main()
{
    if(Init() != 0)
    {
        return 0;
    }

    // This will identify our vertex buffer
    GLuint vboID[2];

    //Vertex Array Object
    GLuint vaoID;

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(2, &vboID[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);


    GLuint programID = LoadShaders( "res/Simple.vert", "res/Simple.frag" );

    // Get a handle for our "MVP" uniform.
    // Only at initialisation time.
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);


    // Ensure we can capture the escape key being pressed below
    glfwEnable( GLFW_STICKY_KEYS );

    // Use Shaders

    glUseProgram(programID);

    glBindVertexArray(vaoID);

    // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED))
    {
       // Send our transformation to the currently bound shader,
       // in the "MVP" uniform
       // For each model you render, since the MVP will be different (at least the M part)
       glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       // Draw the triangle !
       glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

       // Swap buffers
       glfwSwapBuffers();

    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(2, &vboID[0]);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vaoID);

    glUseProgram(0);

    return 0;

}


int Init()
{
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // I want OpenGL 3.1
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    if(glfwOpenWindow( 1024, 768, 0,0,0,0, 8,0, GLFW_WINDOW) != GL_TRUE )
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        return -1;
    }

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetWindowTitle( "Camera-Movement" );

    return 0;

}


