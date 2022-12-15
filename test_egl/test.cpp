
#ifdef __ANDROID__
#include <GLES3/gl32.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif



#include <EGL/egl.h>
#include <iostream>
#include "spng.h"

void save_buffer(const char* filename , unsigned char * image, int width , int height ){

   unsigned error = lodepng::encode(filename, image, width, height);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

}


int main(int argc, char *argv[])
{
    // Step 1 - Get the default display.
    EGLDisplay eglDisplay = eglGetDisplay((EGLNativeDisplayType)0);

    // Step 2 - Initialize EGL.
    eglInitialize(eglDisplay, 0, 0);

    // Step 3 - Make OpenGL ES the current API.
    eglBindAPI(EGL_OPENGL_API);


    // Step 4 - Specify the required configuration attributes.
    EGLint pi32ConfigAttribs[25];
    pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
    pi32ConfigAttribs[1] = EGL_PBUFFER_BIT;
    pi32ConfigAttribs[2] = EGL_RENDERABLE_TYPE;
    pi32ConfigAttribs[3] = EGL_OPENGL_BIT;
    pi32ConfigAttribs[4] = EGL_CONFORMANT;
    pi32ConfigAttribs[5] = EGL_OPENGL_BIT;
    pi32ConfigAttribs[6] = EGL_COLOR_BUFFER_TYPE;
    pi32ConfigAttribs[7] = EGL_RGB_BUFFER;
    pi32ConfigAttribs[8] = EGL_LUMINANCE_SIZE;
    pi32ConfigAttribs[9] = 0;
    pi32ConfigAttribs[10] = EGL_RED_SIZE;
    pi32ConfigAttribs[11] = 8;
    pi32ConfigAttribs[12] = EGL_GREEN_SIZE;
    pi32ConfigAttribs[13] = 8;
    pi32ConfigAttribs[14] = EGL_BLUE_SIZE;
    pi32ConfigAttribs[15] = 8;
    pi32ConfigAttribs[16] = EGL_ALPHA_SIZE;
    pi32ConfigAttribs[17] = 8;
    pi32ConfigAttribs[18] = EGL_DEPTH_SIZE;
    pi32ConfigAttribs[19] = 8;
    pi32ConfigAttribs[20] = EGL_LEVEL;
    pi32ConfigAttribs[21] = 0;
    pi32ConfigAttribs[22] = EGL_BUFFER_SIZE;
    pi32ConfigAttribs[23] = 24;
    pi32ConfigAttribs[24] = EGL_NONE;




    // Step 5 - Find a config that matches all requirements.
    int iConfigs;
    EGLConfig eglConfig;
    eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs);


    if (iConfigs != 1)
    {

        int numConfigs;
                if ( EGL_FALSE == eglGetConfigs(eglDisplay, NULL, 0, &numConfigs) )
            {

            return EGL_FALSE;
            }
            std::cout << " configurations available" << numConfigs << std::endl;
            // collect information about the configs
            EGLConfig *configs = new EGLConfig[numConfigs];

            if ( EGL_FALSE == eglGetConfigs(eglDisplay,configs,numConfigs,&numConfigs) )
                {
                 std::cout  << "eglGetConfigs " << eglGetError() << std::endl;;
                delete [] configs;
                return EGL_FALSE;
                }

    for(int c =0 ; c < numConfigs;++c){
    EGLConfig config = configs[c];
        int _blue_size,_green_size,_red_size,_alpha_size;
        eglGetConfigAttrib( eglDisplay, config, EGL_ALPHA_SIZE, &(_alpha_size));
        eglGetConfigAttrib( eglDisplay, config, EGL_BLUE_SIZE, &(_blue_size));
        eglGetConfigAttrib( eglDisplay, config, EGL_GREEN_SIZE, &(_green_size));
        eglGetConfigAttrib( eglDisplay, config, EGL_RED_SIZE, &(_red_size));
        std::cout << "SIZES RGBA " <<_red_size << _green_size<< _blue_size << _alpha_size << std::endl;
        if ((_red_size == 8) && (_green_size == 8) &&(_blue_size == 8) &&(_alpha_size == 8) ){
            eglConfig = config;
            iConfigs = 1;
            std::cout << "Select config num " << c << std::endl;
             break;
        }

    }

       if (iConfigs!= 1 ) exit(-1);
    }

 std::cout << "done select config" << std::endl;

    EGLint pbufferAttribs[5];
    pbufferAttribs[0] = EGL_WIDTH;
    pbufferAttribs[1] = 1920;
    pbufferAttribs[2] = EGL_HEIGHT;
    pbufferAttribs[3] = 1080;
    pbufferAttribs[4] = EGL_NONE;

    // Step 6 - Create a surface to draw to.
    EGLSurface eglSurface;
    eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, pbufferAttribs);


    if (eglSurface == EGL_NO_SURFACE)
    {
       std::cout  << "surface issue";
    }

  std::cout << "done eglSurface  " << std::endl;

    // Step 7 - Create a context.
    EGLContext eglContext;
    eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);


    if (eglContext == EGL_NO_CONTEXT)
    {
        std::cout  << "context issue";
    }

std::cout << "done eglCreateContext  " << std::endl;

    // Step 8 - Bind the context to the current thread
    bool result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

    if (!result)
    {
       std::cout  << "make current error" << eglGetError() << std::endl;;
    }

std::cout << "done eglMakeCurrent  " << std::endl;



    GLuint renderBufferWidth = 1920;
    GLuint renderBufferHeight = 1080;


    int size = 4 * renderBufferHeight * renderBufferWidth;
    unsigned char *data2 = new unsigned char[size];

    {

       if (false)
       {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.7f, 0.0f};

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 pos;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
                                         "}\0";

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 glColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "    glColor = vec4(0.9f, 0.9f, 0.9f, 1.0f);\n"
                                           "}\n";

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glUseProgram(shaderProgram);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
       }
       else
       {
        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
       }

       eglSwapBuffers(eglDisplay, eglSurface);
       glReadPixels(0, 0, renderBufferWidth, renderBufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    }
    std::cout << "done render " << std::endl;

    save_buffer("/data/local/tmp/egl/buffer.png", data2, renderBufferWidth, renderBufferHeight);

    std::cout << "done save " << std::endl;

    return 0;
}