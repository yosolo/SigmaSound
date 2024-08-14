#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_demo.cpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include "imgui_spectrum.h"
#include <thread>

#include <vector>
#include <string>

/* Data Format */
#define FORMAT              ma_format_f32   /* Must always be f32. */
#define CHANNELS            2
#define SAMPLE_RATE         48000

//namespace ImGui
//{
//    void StyleColorsSpectrum() {
//        ImGuiStyle* style = &ImGui::GetStyle();
//        style->GrabRounding = 4.0f;
//
//        ImVec4* colors = style->Colors;
//        colors[ImGuiCol_Text] = ColorConvertU32ToFloat4(Spectrum::GRAY800); // text on hovered controls is gray900
//        colors[ImGuiCol_TextDisabled] = ColorConvertU32ToFloat4(Spectrum::GRAY500);
//        colors[ImGuiCol_WindowBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100);
//        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
//        colors[ImGuiCol_PopupBg] = ColorConvertU32ToFloat4(Spectrum::GRAY50); // not sure about this. Note: applies to tooltips too.
//        colors[ImGuiCol_Border] = ColorConvertU32ToFloat4(Spectrum::GRAY300);
//        colors[ImGuiCol_BorderShadow] = ColorConvertU32ToFloat4(Spectrum::Static::NONE); // We don't want shadows. Ever.
//        colors[ImGuiCol_FrameBg] = ColorConvertU32ToFloat4(Spectrum::GRAY75); // this isnt right, spectrum does not do this, but it's a good fallback
//        colors[ImGuiCol_FrameBgHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY50);
//        colors[ImGuiCol_FrameBgActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
//        colors[ImGuiCol_TitleBg] = ColorConvertU32ToFloat4(Spectrum::GRAY300); // those titlebar values are totally made up, spectrum does not have this.
//        colors[ImGuiCol_TitleBgActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
//        colors[ImGuiCol_TitleBgCollapsed] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
//        colors[ImGuiCol_MenuBarBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100);
//        colors[ImGuiCol_ScrollbarBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100); // same as regular background
//        colors[ImGuiCol_ScrollbarGrab] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
//        colors[ImGuiCol_ScrollbarGrabHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
//        colors[ImGuiCol_ScrollbarGrabActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
//        colors[ImGuiCol_CheckMark] = ColorConvertU32ToFloat4(Spectrum::BLUE500);
//        colors[ImGuiCol_SliderGrab] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
//        colors[ImGuiCol_SliderGrabActive] = ColorConvertU32ToFloat4(Spectrum::GRAY800);
//        colors[ImGuiCol_Button] = ColorConvertU32ToFloat4(Spectrum::GRAY75); // match default button to Spectrum's 'Action Button'.
//        colors[ImGuiCol_ButtonHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY50);
//        colors[ImGuiCol_ButtonActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
//        colors[ImGuiCol_Header] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
//        colors[ImGuiCol_HeaderHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE500);
//        colors[ImGuiCol_HeaderActive] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
//        colors[ImGuiCol_Separator] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
//        colors[ImGuiCol_SeparatorHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
//        colors[ImGuiCol_SeparatorActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
//        colors[ImGuiCol_ResizeGrip] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
//        colors[ImGuiCol_ResizeGripHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
//        colors[ImGuiCol_ResizeGripActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
//        colors[ImGuiCol_PlotLines] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
//        colors[ImGuiCol_PlotLinesHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
//        colors[ImGuiCol_PlotHistogram] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
//        colors[ImGuiCol_PlotHistogramHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
//        colors[ImGuiCol_TextSelectedBg] = ColorConvertU32ToFloat4((Spectrum::BLUE400 & 0x00FFFFFF) | 0x33000000);
//        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
//        colors[ImGuiCol_NavHighlight] = ColorConvertU32ToFloat4((Spectrum::GRAY900 & 0x00FFFFFF) | 0x0A000000);
//        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
//        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
//        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
//    }
//}
//
//int main(int argc, char** argv)
//{
//    ma_result result;
//    ma_engine engine;
//
//    ma_context context;
//    if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) {
//        return 1;
//    }
//
//    ma_device_info* pPlaybackInfos;
//    ma_uint32 playbackCount;
//    ma_device_info* pCaptureInfos;
//    ma_uint32 captureCount;
//    result = ma_context_get_devices(&context, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount);
//    if (result != MA_SUCCESS) 
//    {
//        return result;
//    }
//
//    // Loop over each device info and do something with it. Here we just print the name with their index. You may want
//    // to give the user the opportunity to choose which device they'd prefer.
//    for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1) 
//    {
//        printf("%d - %s\n", iDevice, pPlaybackInfos[iDevice].name);
//    }
//
//    ma_engine_config engineConfig = ma_engine_config_init();
//    result = ma_engine_init(NULL, &engine);
//    if (result != MA_SUCCESS) 
//    {
//        return result;  // Failed to initialize the engine.
//    }
//
//#define DELAY_IN_SECONDS    0.2f
//#define DECAY               0.25f   /* Volume falloff for each echo. */
//
//    ma_delay_node delayNode;
//    {
//        ma_delay_node_config delayNodeConfig;
//        ma_uint32 channels;
//        ma_uint32 sampleRate;
//
//        channels = ma_engine_get_channels(&engine);
//        sampleRate = ma_engine_get_sample_rate(&engine);
//
//        delayNodeConfig = ma_delay_node_config_init(channels, sampleRate, (ma_uint32)(sampleRate * DELAY_IN_SECONDS), DECAY);
//
//        result = ma_delay_node_init(ma_engine_get_node_graph(&engine), &delayNodeConfig, NULL, &delayNode);
//        if (result != MA_SUCCESS) {
//            printf("Failed to initialize delay node.");
//            return -1;
//        }
//
//        /* Connect the output of the delay node to the input of the endpoint. */
//        ma_node_attach_output_bus(&delayNode, 0, ma_engine_get_endpoint(&engine), 0);
//    }
//
//    ma_sound sound;
//    result = ma_sound_init_from_file(&engine, "eple.mp3", 0, nullptr, nullptr, &sound);
//
//    //ma_node_attach_output_bus(&sound, 0, &delayNode, 0);
//    // ma_sound_set_start_time_in_milliseconds(&sound, 5000.0f);
//    // ma_sound_start(&sound);
//
//    //--------------------------------------------------------------------------------------------------------------------------------------GLWF/IamGUI
//
//    glfwInit();
//    GLFWwindow* window = glfwCreateWindow(800, 600, "Sound", nullptr, nullptr);
//    glfwMakeContextCurrent(window);
//
//
//    ImGui::CreateContext();
//    
//    ImGuiIO& io = ImGui::GetIO();
//    io.Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 15.0f);
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable;
//
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL2_Init();
//
//    ImGui::StyleColorsSpectrum();
//
//    while (!glfwWindowShouldClose(window))
//    {
//        glfwPollEvents();
//
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        ImGui_ImplOpenGL2_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        
//
//        ImGui::Begin("Test1");
//        ImGui::Text("Hello");
//        if (ImGui::Button("Play")) 
//        {
//            static bool playing = false;
//            if (!playing)
//                ma_sound_start(&sound);
//            else
//                ma_sound_stop(&sound);
//
//            playing = !playing;
//        }
//        ImGui::End();
//
//        ImGui::Begin("Test2");
//        ImGui::End();
//
//        ImGui::ShowDemoWindow();
//
//        ImGui::Render();
//        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
//
//        ImGui::EndFrame();
//        GLFWwindow* backup_current_context = glfwGetCurrentContext();
//        ImGui::UpdatePlatformWindows();
//        ImGui::RenderPlatformWindowsDefault();
//        glfwMakeContextCurrent(backup_current_context);
//
//        glfwSwapBuffers(window);
//    }
//
//    return 0;
//}

typedef struct
{
    ma_data_source_node node;   /* If you make this the first member, you can pass a pointer to this struct into any ma_node_* API and it will "Just Work". */
    ma_decoder decoder;
} sound_node;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_node_graph* nodeGraph = (ma_node_graph*)pDevice->pUserData;

    ma_node_graph_read_pcm_frames(nodeGraph, pOutput, frameCount, NULL);

    (void)pInput;
}

struct DeviceManager
{
    ma_context context;

    ma_device device1;
    ma_device device2;

    ma_node_graph node_graph1;
    ma_node_graph node_graph2;

    ma_splitter_node splitterNode;

    std::vector<sound_node*> nodes;

    DeviceManager()
    {
        if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) 
        {
            printf("Failed to initialize devices in DeviceManager struct");
            return;
        }

        ma_device_info* pPlaybackInfos;
        ma_uint32 playbackCount;
        ma_device_info* pCaptureInfos;
        ma_uint32 captureCount;
        if (ma_context_get_devices(&context, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount) != MA_SUCCESS) {
            printf("Failed to get physical devices from context in DeviceManager struct");
            return;
        }

        // Loop over each device info and do something with it. Here we just print the name with their index. You may want
        // to give the user the opportunity to choose which device they'd prefer.
        for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1) {
            printf("%d - %s\n", iDevice, pPlaybackInfos[iDevice].name);
        }

        ma_device_config config_device1 = ma_device_config_init(ma_device_type_playback);
        config_device1.playback.pDeviceID = &pPlaybackInfos[4].id;
        config_device1.playback.format = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
        config_device1.playback.channels = 2;               // Set to 0 to use the device's native channel count.
        config_device1.sampleRate = 44100;           // Set to 0 to use the device's native sample rate.
        config_device1.dataCallback = data_callback;   // This function will be called when miniaudio needs more data.
        config_device1.pUserData = &node_graph1;   // Can be accessed from the device object (device.pUserData).

        ma_device_config config_device2 = ma_device_config_init(ma_device_type_playback);
        config_device2.playback.pDeviceID = &pPlaybackInfos[0].id;
        config_device2.playback.format = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
        config_device2.playback.channels = 2;               // Set to 0 to use the device's native channel count.
        config_device2.sampleRate = 44100;           // Set to 0 to use the device's native sample rate.
        config_device2.dataCallback = data_callback;   // This function will be called when miniaudio needs more data.
        config_device2.pUserData = &node_graph2;   // Can be accessed from the device object (device.pUserData).

        if (ma_device_init(NULL, &config_device1, &device1) != MA_SUCCESS ||
            ma_device_init(NULL, &config_device2, &device2) != MA_SUCCESS) {
            printf("ERROR: Failed to initialize devices in DeviceManager struct");
            return;
        }

        ma_device_stop(&device1);
        ma_device_stop(&device2);

        ma_node_graph_config nodeGraphConfig = ma_node_graph_config_init(2);

        ma_node_graph_init(&nodeGraphConfig, NULL, &node_graph1);
        ma_node_graph_init(&nodeGraphConfig, NULL, &node_graph2);


        /* Splitter. */
        {
            ma_splitter_node_config splitterNodeConfig = ma_splitter_node_config_init(CHANNELS);
        
            if (ma_splitter_node_init(&node_graph1, &splitterNodeConfig, NULL, &splitterNode) != MA_SUCCESS) {
                printf("ERROR: Failed to initialize splitter node.");
                return;
            }
        
            ma_node_attach_output_bus(&splitterNode, 0, ma_node_graph_get_endpoint(&node_graph1), 0);
            ma_node_attach_output_bus(&splitterNode, 1, ma_node_graph_get_endpoint(&node_graph2), 0);
        }
        
    }

    ~DeviceManager()
    {
        ma_device_uninit(&device1);
        ma_device_uninit(&device2);
        for(int i = 0; i < nodes.size(); i++)
            delete(nodes[i]);
    }

    void start()
    {
        ma_device_start(&device1);
        ma_device_start(&device2);
    }

    void stop()
    {
        ma_device_stop(&device1);
        ma_device_stop(&device2);
    }

    void clear()
    {
        ma_device_stop(&device1);
        ma_device_stop(&device2);
        for (int i = 0; i < nodes.size(); i++)
            ma_node_detach_all_output_buses(&nodes[0]->node);
    }

    void addSound(std::string path)
    {
        nodes.push_back(new sound_node());

        ma_decoder_init_file(path.c_str(), NULL, &nodes[nodes.size() - 1]->decoder);

        ma_data_source_node_config dataSourceNodeConfig1 = ma_data_source_node_config_init(&nodes[nodes.size() - 1]->decoder);
        
        if (ma_data_source_node_init(&node_graph1, &dataSourceNodeConfig1, NULL, &nodes[nodes.size() - 1]->node) != MA_SUCCESS)
        {
            printf("ERROR: Failed to init source node in DeviceManager struct");
            return;
        }
        
        if (ma_node_attach_output_bus(&nodes[nodes.size() - 1]->node, 0, &splitterNode, 0) != MA_SUCCESS)
        {
            printf("ERROR: Failed to attach in DeviceManager struct");
            return;
        }
    }
};

int main()
{
    DeviceManager device;
    device.start();
    device.addSound("eple.mp3");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    device.addSound("afloat.mp3");
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    device.clear();


    while (1);

    return 0;
}