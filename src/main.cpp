#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <GLFW/glfw3.h>

int main(int argc, char** argv)
{
    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) 
    {
        return result;  // Failed to initialize the engine.
    }

    ma_sound sound;
    result = ma_sound_init_from_file(&engine, "eple.mp3", 0, nullptr, nullptr, &sound);
    // ma_sound_set_start_time_in_milliseconds(&sound, 5000.0f);
    ma_sound_start(&sound);

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Sound", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}
