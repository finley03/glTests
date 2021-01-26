
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


    //----------INIT AND CONFIGURE----------//



    if (!init()) return -1; // initialize window and opengl



    glViewport(0, 0, wWidth, wHeight); // set opengl viewport size
    glClearColor(0.1, 0.1, 0.1, 1); // 0 red, 0 green, 0 blue, 1 alpha - 100% opacity


    glEnable(GL_DEPTH_TEST); // some opengl rendering settings
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDisable(GL_CULL_FACE);
    //glDepthRange(0.0f, 2.0f);
    //glCullFace(GL_FRONT_AND_BACK);


    // create shader programs
    int shaderSuccess;
    Shader objectShader("a.vert", "a.frag", shaderSuccess);
    if (!shaderSuccess) return -1;

    Shader lightShader("i.vert", "i.frag", shaderSuccess);
    if (!shaderSuccess) return -1;



    // set up objects

    /*int meshsuccess;
    genMeshFromFile("cube.obj", meshsuccess);
    if (!meshsuccess) std::cout << "Could not find file" << std::endl;*/

    //std::vector<float> vertices = {
    //    0.5, -0.5f, -0.5f, 1.0f, 0.0f, // bottom right // vertices[3], textures[2] square
    //    -0.5, -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
    //    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top left
    //    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top right

    //    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    //    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //    0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    //    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

    //    0.5, -0.5f, 0.5f, 1.0f, 0.0f,
    //    -0.5, -0.5f, 0.5f, 0.0f, 0.0f,
    //    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    //    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

    //    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    //    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    //    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

    //    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    //    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    //    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

    //    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //    0.5f, -0.5f, -0.5f, 1.0f, 1.0f
    //};

    ////float* vertexPointer = vertices;

    //unsigned int indices[] = {
    //    0, 1, 2,
    //    2, 3, 0,

    //    4, 5, 6,
    //    6, 7, 4,

    //    8, 9, 10,
    //    10, 11, 8,

    //    12, 13, 14,
    //    14, 15, 12,

    //    16, 17, 18,
    //    18, 19, 16,

    //    20, 21, 22,
    //    22, 23, 20
    //};




    //unsigned int VAO, VBO, EBO;
    //glGenVertexArrays(1, &VAO); // create vertex array object
    //glGenBuffers(1, &VBO); // create vertex buffer object
    //glGenBuffers(1, &EBO); // create element buffer object

    //// bind VAO
    //glBindVertexArray(VAO);
    //// copy data to buffer
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //// set vertex attribute pointers
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // position attribute
    //glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // texture attribute
    //glEnableVertexAttribArray(1);







    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture attribute
    //glEnableVertexAttribArray(2);


    Objects objects; // create objects class
    objects.createFromFile("assets/MaleLow.obj", "thing"); // create objects
    objects.createFromFile("assets/cube.obj", "light"); // filepath, name
    //objects.createFromFile("cube.obj", "cube");
    //objects.createFromFile("BrandenburgGate.obj", "building");





    //----------SET UP TEXTURES----------//




    //Texture textures; // create texture class

    //textures.newTexture("wall.jpg", "texture0", 0, GL_REPEAT, GL_LINEAR);
    ////textures.newTexture("bolsonaro.bmp", "texture1", 1, GL_REPEAT, GL_NEAREST);

    //textures.bind("texture0", 0);
    ////textures.bind("texture1", 1);

    //shaders.use();

    //shaders.setInt("texture0", 0); // set texture to texture unit 0
    ////shaders.setInt("texture1", 1); // set texture to texture unit 1

    //float objectColor[] = { 0.1f, 0.1f, 0.8f };
    float lightColor[] = { 1.0f, 1.0f, 1.0f }; // test lighting and material colors

    float ambientColor[] = { 0.02f, 0.02f, 0.1f };
    float diffuseColor[] = { 0.f, 0.1f, 0.4f };
    float specularColor[] = { 0.2f, 0.2f, 0.2f };
    float smoothness = 32.0f; // a higher number leads to smaller radius specular reflections


    //lightShader.use();
    




    //---------SET UP TRANSFORMATIONS---------//




    glm::mat4 Identity = glm::mat4(1.0f); // create 4x4 identity matrix for model view
    //modelBase = glm::rotate(modelBase, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 model; // initialize model matrix

    //glm::mat4 view = glm::mat4(1.0f); // create 4x4 identity matrix for global view
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //shaders.mat4("view", glm::value_ptr(view));
    float cameraPosition[3] = { 0.0f, 10.0f, 45.0f };

    glm::mat4 view = glm::mat4(1.0f); // set camera matrix
    view = glm::lookAt(
        //glm::vec3(0.0f, 10.0f, 45.0f), // position
        glm::make_vec3(cameraPosition),
        glm::vec3(0.0f, 10.0f, 0.0f), // target
        glm::vec3(0.0f, 1.0f, 0.0f) // up
    );
    //shaders.use();
    
    //lightShader.use();
    

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
    

    float lightPosition[3] = { 12.0f, 24.0f, 5.0f };

    glm::mat4 lightMatrix = glm::translate(Identity, glm::make_vec3(lightPosition));




    //----------SET UNIFORMS----------//




    objectShader.use();
    //objectShader.vec3("objectColor", objectColor);
    objectShader.vec3("lightColor", lightColor);

    objectShader.mat4("view", glm::value_ptr(view));
    objectShader.mat4("projection", glm::value_ptr(projection));

    objectShader.vec3("lightPos", lightPosition);
    objectShader.vec3("cameraPos", cameraPosition);

    objectShader.vec3("material.ambient", ambientColor);
    objectShader.vec3("material.diffuse", diffuseColor);
    objectShader.vec3("material.specular", specularColor);
    objectShader.setFloat("material.smoothness", smoothness);


    lightShader.use();
    lightShader.vec3("lightColor", lightColor);

    lightShader.mat4("view", glm::value_ptr(view));
    lightShader.mat4("projection", glm::value_ptr(projection));

    




    //----------RUNNING LOOP----------//





    bool run = true; // sets variable for loop
    time_t tUnix;

    while (run) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_QUIT:
                run = false;
                std::cout << "Exiting...\n";
                break;

            case SDL_WINDOWEVENT:

                switch (event.window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED: // resize opengl canvas on window resize event
                    wWidth = event.window.data1;
                    wHeight = event.window.data2;
                    glViewport(0, 0, wWidth, wHeight);
                    SDL_GL_SwapWindow(window);
                    break;

                default:
                    std::cout << "Unhandled Window Event: " << event.window.type << std::endl;
                    break;



                }
                break;


            default:
                std::cout << "Unhandled Event: " << event.type << std::endl;
                break;

            }
            
        }

        float tUnix = (float)SDL_GetTicks() / 1000.0f;


        // draw objects
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::translate(Identity, glm::vec3(-10.0f, 0.0f, 0.0f));
        model = glm::rotate(model, tUnix/3, glm::vec3(0.0f, 1.0f, 0.0f));
        objectShader.use();
        objectShader.mat4("model", glm::value_ptr(model));
        objects.draw("thing");

        lightShader.use();
        lightShader.mat4("model", glm::value_ptr(lightMatrix));
        objects.draw("light");

        SDL_GL_SwapWindow(window);


        SDL_Delay(FRAMETIME);

    }

    



    //----------CLEANUP----------//





    SDL_GL_DeleteContext(gl_ctx);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    objects.destroy("thing");
    objects.destroy("cube");
    objects.terminate();
    objectShader.close();
    lightShader.close();
    window = nullptr;
    renderer = nullptr;
    SDL_Quit();

    return 0;
}




//----------FUNCTIONS----------//




int init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { // initialise sdl, return error if failed
        std::cout << "SDL failed to initalise video: " << SDL_GetError() << std::endl;
        return false;
    }
    else {
        std::cout << "SDL initialised video\n";
    }


    window = SDL_CreateWindow(
        TITLE, // window title
        SDL_WINDOWPOS_CENTERED, // window X pos
        SDL_WINDOWPOS_CENTERED, // window Y pos
        wWidth, // width
        wHeight, // height
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE // display flags
    );

    if (!window) { // check window was actually made
        std::cout << "SDL failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }
    else {
        std::cout << "SDL created window\n";
    }


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // create hardware renderer

    if (!renderer) { // check renderer created successfully
        std::cout << "SDL failed to create hardware renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    else {
        std::cout << "SDL created hardware renderer\n";
    }

    gl_ctx = SDL_GL_CreateContext(window); // create opengl context with sdl


    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // increase default depth test bits, 16 is too low


    // unfortunately windows is annoying because no opengl features over 1.1 are included by default
    // so loading opengl extensions using glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) { // load extensions, raise error if fail
        std::cout << "Failed to initialise OpenGL context\n";
        return false;
    }
    else {
        std::cout << "OpenGL context created and extensions loaded\n"
            << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << std::endl;
    }

    return true;
}


//
//int colorSpace(int nrChannels, int& variable) {
//    switch (nrChannels) {
//    case 3:
//        variable = GL_RGB;
//        return true;
//
//    case 4:
//        variable = GL_RGBA;
//        return true;
//
//    default:
//        std::cout << "Error: Texture uses unsupported colorspace code " << nrChannels << std::endl;
//        return false;
//    }
//}