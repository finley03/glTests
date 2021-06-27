
// this is a basic opengl framework I'm messing about with
// some notes on limitations:
// - most of the program is very experimental, there aren't much polished elements, and lots will change in the future
// - while the program can understand .obj files, there are some limitations:
//      - face elements can have any number of vertices, but they must have
//        vertex positions, vertex normals and texture coordinates.
//        this format looks like this:  f v1/vn1/vt1
//        any other format likely won't work
//      - the program can't interpret .mtl files (yet)
//      - multiple .obj features are still unsupported
//      - here are links to two .obj files that this program should work with:
//           https://gist.github.com/MaikKlein/0b6d6bb58772c13593d0a0add6004c1c
//           https://free3d.com/3d-model/low-poly-male-26691.html


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW


#include <iostream>
#include <string>
//#include <fstream> // file operations
//#include <sstream> // stringstream
#include <cmath>
#include <vector>
#include <glad/glad.h> // opengl extension manager
#include <SDL.h> // window / input manager
#include <glm/glm.hpp> // gl mathematics library
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shaders.h"
#include "textures.h"
#include "objects.h"
#include "obj.h"
#include "terminalColors.h"
#include "screen.h"


//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"



#define TITLE "SDL OpenGL Things"
#define WIDTH 1400
#define HEIGHT 1000
#define FRAMETIME 1000/60

int wWidth = WIDTH;
int wHeight = HEIGHT;




//----------GLOBAL DECLARATIONS----------//




SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
SDL_GLContext gl_ctx;

//unsigned int vertexShader;
//unsigned int fragmentShader;
//unsigned int shaderProgram;

//int colorSpace(int nrChannels, int& variable);

int init();



//----------MAIN----------//




int main(int argc, char* argv[])
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //----------INIT AND CONFIGURE----------//



    if (!init()) return -1; // initialize window and opengl




    // create shader programs
    int shaderSuccess;
    Shader objectShader("a.vert", "a.frag", shaderSuccess);
    if (!shaderSuccess) return -1;

    Shader lightShader("i.vert", "i.frag", shaderSuccess);
    if (!shaderSuccess) return -1;

    Shader textureShader("t.vert", "t.frag", shaderSuccess);
    if (!shaderSuccess) return -1;





    //----------SET UP TEXTURES----------//



    Texture textures;

    Objects objects; // create objects class

    objects.objectShader = &objectShader;
    objects.textureShader = &textureShader;
    objects.textures = &textures;

    //objects.createFromFile("assets/backpack/backpack.obj", "thing"); // create objects
    //objects.createFromFile("assets/MaleLow/MaleLow.obj", "thing"); // create objects
    objects.createFromFile("assets/MaleLow/MaleLow.obj", "thing");
    objects.createFromFile("assets/floor/woodfloor.obj", "floor");
    objects.createFromFile("assets/cube/cube.obj", "light0"); // filepath, name
    //objects.createFromFile("assets/cube/cube.obj", "light1");
    //objects.createFromFile("assets/cube/cube2.obj", "cube");




    struct Light {
        float ambient[3] = { 0.01f, 0.01f, 0.01f };
        float diffuse[3] = { 0.5f, 0.5f, 0.5f };
        float specular[3] = { 1.0f, 1.0f, 1.0f };
        float position[3] = { 12.0f, 13.0f, -3.0f };
        float position1[3] = { 12.0f, 4.0f, 5.0f };

        float a = 0.00007f;
        float b = 0.0014f;
        float c = 1.0f;
    };


    Light light;
    //




    //---------SET UP TRANSFORMATIONS---------//




    glm::mat4 Identity = glm::mat4(1.0f); // create 4x4 identity matrix for model view
    //modelBase = glm::rotate(modelBase, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 model; // initialize model matrix

    //glm::mat4 view = glm::mat4(1.0f); // create 4x4 identity matrix for global view
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //shaders.mat4("view", glm::value_ptr(view));
    float cameraPosition[3] = { -15.0f, 25.0f, 25.0f };

    glm::mat4 view = glm::mat4(1.0f); // set camera matrix
    view = glm::lookAt(
        //glm::vec3(0.0f, 10.0f, 45.0f), // position
        glm::make_vec3(cameraPosition),
        glm::vec3(0.0f, 5.0f, 0.0f), // target
        glm::vec3(0.0f, 1.0f, 0.0f) // up
    );
    //shaders.use();
    
    lightShader.use();
    

    glm::mat4 projection = glm::mat4(1.0f); // create 4x4 identity matrix for global view
    // generate perspective projection matrix
    projection = glm::perspective(glm::radians(45.0f), (float)wWidth/wHeight, 3.0f, 100.0f);
    //projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
    //float* projectionArr = glm::value_ptr(projection);
    //for (int i = 0; i < 16; ++i) {
    //    std::cout << projectionArr[i] << std::endl;
    //}
    //shaders.use();
    
    //lightShader.use();
    

    //float lightPosition[3] = { 12.0f, 24.0f, 5.0f };

    glm::mat4 lightMatrix = glm::translate(Identity, glm::make_vec3(light.position));
    glm::mat4 lightMatrix1 = glm::translate(Identity, glm::make_vec3(light.position1));




    //----------SET UNIFORMS----------//




    objectShader.use();
    //objectShader.vec3("objectColor", objectColor);
    objectShader.vec3("lightColor", light.diffuse);

    objectShader.mat4("view", glm::value_ptr(view));
    objectShader.mat4("projection", glm::value_ptr(projection));

    //objectShader.vec3("lightPos", light.position);
    objectShader.vec3("cameraPos", cameraPosition);

    //objectShader.vec3("material.ambient", material.ambient);
    //objectShader.vec3("material.diffuse", material.diffuse);
    //objectShader.vec3("material.specular", material.specular);
    //objectShader.setFloat("material.smoothness", material.smoothness);
    objectShader.setUint("nrPointLights", 1);

    objectShader.vec3("light[0].ambient", light.ambient);
    objectShader.vec3("light[0].diffuse", light.diffuse);
    objectShader.vec3("light[0].specular", light.specular);
    objectShader.vec3("light[0].position", light.position);
    objectShader.setFloat("light[0].a", light.a);
    objectShader.setFloat("light[0].b", light.b);
    objectShader.setFloat("light[0].c", light.c);

    //objectShader.vec3("light[1].ambient", light.ambient);
    //objectShader.vec3("light[1].diffuse", light.diffuse);
    //objectShader.vec3("light[1].specular", light.specular);
    //objectShader.vec3("light[1].position", light.position1);
    //objectShader.setFloat("light[1].a", light.a);
    //objectShader.setFloat("light[1].b", light.b);
    //objectShader.setFloat("light[1].c", light.c);



    textureShader.use();
    textureShader.vec3("lightColor", light.diffuse);

    textureShader.mat4("view", glm::value_ptr(view));
    textureShader.mat4("projection", glm::value_ptr(projection));

    textureShader.vec3("cameraPos", cameraPosition);

    textureShader.setInt("material.diffuse", 0);
    textureShader.setInt("material.specular", 1);
    //textureShader.setFloat("material.smoothness", material.smoothness);
    textureShader.setUint("nrPointLights", 1);

    textureShader.vec3("light[0].ambient", light.ambient);
    textureShader.vec3("light[0].diffuse", light.diffuse);
    textureShader.vec3("light[0].specular", light.specular);
    textureShader.vec3("light[0].position", light.position);
    textureShader.setFloat("light[0].a", light.a);
    textureShader.setFloat("light[0].b", light.b);
    textureShader.setFloat("light[0].c", light.c);

    //textureShader.vec3("light[1].ambient", light.ambient);
    //textureShader.vec3("light[1].diffuse", light.diffuse);
    //textureShader.vec3("light[1].specular", light.specular);
    //textureShader.vec3("light[1].position", light.position1);
    //textureShader.setFloat("light[1].a", light.a);
    //textureShader.setFloat("light[1].b", light.b);
    //textureShader.setFloat("light[1].c", light.c);



    lightShader.use();
    lightShader.vec3("lightColor", light.specular);

    lightShader.mat4("view", glm::value_ptr(view));
    lightShader.mat4("projection", glm::value_ptr(projection));



    std::cout << color::success << "\nGraphics Loaded!" << color::std << std::endl;





    //----------FRAMEBUFFER----------//






    int screenSuccess;
    Screen screen(wWidth, wHeight, screenSuccess);
    if (!screenSuccess) return -1;


    //screen.clear();
    //screen.swap();
    //SDL_GL_SwapWindow(window);

    //SDL_Delay(5000);
    //return 0;



    //----------RUNNING LOOP----------//




    

    bool run = true; // sets variable for loop
    time_t tUnix;

    while (run) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_QUIT:
                run = false;
                std::cout << color::process << "Exiting...\n" << color::std;
                break;

            case SDL_WINDOWEVENT:

                switch (event.window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED: // resize opengl canvas on window resize event
                    wWidth = event.window.data1;
                    wHeight = event.window.data2;
                    glViewport(0, 0, wWidth, wHeight);
                    //projection = glm::perspective(glm::radians(45.0f), (float)wWidth / wHeight, 3.0f, 100.0f);
                    //textureShader.mat4("projection", glm::value_ptr(projection));
                    //objectShader.mat4("projection", glm::value_ptr(projection));
                    //lightShader.mat4("projection", glm::value_ptr(projection));
                    SDL_GL_SwapWindow(window);
                    break;

                default:
                    //std::cout << "Unhandled Window Event: " << color::value << event.window.type << color::std << std::endl;
                    break;


                    
                }
                break;


            default:
                //std::cout << "Unhandled Event: " << color::value << event.type << color::std << std::endl;
                break;

            }
            
        }

        float tUnix = (float)SDL_GetTicks() / 1000.0f;


        // draw objects
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screen.clear();

        model = glm::translate(Identity, glm::vec3(-7.0f, 5.0f, 0.0f)); // -10.0f, 2.0f, 0.0f
        model = glm::rotate(model, tUnix/3, glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::translate(model, glm::vec3());
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        textureShader.use();
        textureShader.mat4("model", glm::value_ptr(model));
        objectShader.use();
        objectShader.mat4("model", glm::value_ptr(model));
        objects.draw("thing");

        model = glm::scale(Identity, glm::vec3(15.0f, 15.0f, 15.0f));
        textureShader.use();
        textureShader.mat4("model", glm::value_ptr(model));
        objects.draw("floor");

        lightShader.use();
        lightShader.mat4("model", glm::value_ptr(lightMatrix));
        objects.drawLight("light0");
        //lightShader.mat4("model", glm::value_ptr(lightMatrix1));
        //objects.drawLight("light1");

        screen.swap();
        SDL_GL_SwapWindow(window);


        SDL_Delay(FRAMETIME);

    }

    



    //----------CLEANUP----------//





    SDL_GL_DeleteContext(gl_ctx);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    objects.destroy("thing");
    objects.destroy("cube");
    objects.destroy("light0");
    objects.terminate();
    objectShader.close();
    textureShader.close();
    lightShader.close();

    //delete (textures);

    //screen.close();
    //glDeleteFramebuffers(1, &FBO);
    window = nullptr;
    renderer = nullptr;
    SDL_Quit();

    //_CrtMemDumpAllObjectsSince(&s1);
    //_CrtDumpMemoryLeaks();

    return 0;
}




//----------FUNCTIONS----------//




int init() {

    //std::cout << "\033[38;2;255;0;0mRed Text\033[0m\n";
    //std::cout << color::red << "Red Text" << color::white << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) { // initialise sdl, return error if failed
        std::cout << color::error << "SDL failed to initalise video: " << SDL_GetError() << std::endl;
        return false;
    }
    else {
        std::cout << color::std << "initialised " << color::process << "video...";
    }


    // enable antialiasing
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);


    window = SDL_CreateWindow(
        TITLE, // window title
        SDL_WINDOWPOS_CENTERED, // window X pos
        SDL_WINDOWPOS_CENTERED, // window Y pos
        wWidth, // width
        wHeight, // height
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE // display flags
    );

    if (!window) { // check window was actually made
        std::cout << color::error << "\nSDL failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }
    else {
        std::cout << "window...";
    }


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // create hardware renderer

    if (!renderer) { // check renderer created successfully
        std::cout << color::error << "\nSDL failed to create hardware renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    else {
        std::cout << "hardware renderer" << color::std << std::endl;
    }


    gl_ctx = SDL_GL_CreateContext(window); // create opengl context with sdl


    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // increase default depth test bits, 16 is too low




    // unfortunately windows is annoying because no opengl features over 1.1 are included by default
    // so loading opengl extensions using glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) { // load extensions, raise error if fail
        std::cout << color::error << "Failed to initialise OpenGL context\n";
        return false;
    }
    else {
        std::cout << "OpenGL context created - " << color::value << "Version " << GLVersion.major << "." << GLVersion.minor << color::std << "\n" << std::endl;
    }

    glViewport(0, 0, wWidth, wHeight); // set opengl viewport size
    glClearColor(0.1, 0.1, 0.1, 1); // 0 red, 0 green, 0 blue, 1 alpha - 100% opacity


    glEnable(GL_DEPTH_TEST); // some opengl rendering settings
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDisable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_FRAMEBUFFER_SRGB);

    return true;
}


