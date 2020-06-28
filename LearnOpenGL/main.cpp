
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <XOpenGL/Shader.h>
#include <XOpenGL/stb_image.h>

//window size change call back
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0,0,width,height);
}

//process input 
void processInput(GLFWwindow * window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window,true);
    }
}


int main(int argc,char** argv)
{
    //vertex point
    float verPoints[]={
        0.5f,0.5f,0.0f,  1.0f,0.0f,0.0f,   1.0,1.0,0.0,    //right top    
        0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f,  1.0,0.0,0.0,    //right bottom
        -0.5f,-0.5f,0.0f,0.0f,0.0f,1.0f,  0.0,0.0,0.0,    //left bottom
        -0.5f,0.5f,0.0f ,0.0f,0.0f,0.0f,    0.0,1.0,0.0   //left top
    };
    //ebo line
    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR ,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    //create window
    GLFWwindow *window = glfwCreateWindow(800,600,"learn opengl",nullptr,nullptr);
    if(window == nullptr)
    {
        std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return -1;
    }

    //opengl window bind current thread 
    //note : opengl is signle thread api
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<"Failed to init glad"<<std::endl;
        return -1;
    }    

    //set view port size 
    glViewport(0,0,800,600);

    //set view port change call back
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    //shader 
    Shader shader("D:\\XSOURCE\\LearnOpenGL\\LearnOpenGL\\shader\\vshader.glsl","D:\\XSOURCE\\LearnOpenGL\\LearnOpenGL\\shader\\fshaser.glsl");
    

    //check vertex attribute number
    int numberAttribute;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&numberAttribute);
    std::cout<<"maximum numger of vertex attributes is : "<<numberAttribute<<std::endl;

    //vao
    GLuint VAO;
    glGenVertexArrays(1,&VAO);

    //vbo 
    GLuint VBO;
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);

    //ebo
    GLuint EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    //bind vbo
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    //send data in vbo buffer
    glBufferData(GL_ARRAY_BUFFER,sizeof(verPoints),verPoints,GL_STATIC_DRAW);

    //set opengl parse pointer attribute
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,9*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,9*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,9*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    //texture set s,t,r equal x,y,z
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    //texture max and min filter set
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //create texture
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

    stbi_set_flip_vertically_on_load(true);

    //load texture file 
    int width ,height ,nrChannels;
    unsigned char*data = stbi_load("D:\\XSOURCE\\LearnOpenGL\\table.jpg",&width,&height,&nrChannels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"failed to load texture"<<std::endl;
    }
    
    stbi_image_free(data);

    unsigned int texture1;
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    //load texture 2 image
    data = stbi_load("D:\\XSOURCE\\LearnOpenGL\\test.jpg",&width,&height,&nrChannels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"failed to load texture"<<std::endl;
    }
    
    stbi_image_free(data);

    shader.use();
    shader.setInt("picTexture",0);
    shader.setInt("picTexture1",1);


    //set draw line mode 
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); 
    float pos = 1.5;
    //render loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        shader.setFloat("posadd",pos);
        //pos+=0.02;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glBindVertexArray(0);

        //post event
        glfwPollEvents();

        glfwSwapBuffers(window);

    }

    std::cout<<"hello world "<<std::endl;


    //close glfw
    glfwTerminate();
    return 0;
}