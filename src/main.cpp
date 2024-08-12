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

#define SAMPLE_FORMAT   ma_format_f32
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     48000

//std::vector<ma_decoder> pDecoders1;
//std::vector<ma_decoder> pDecoders2;

ma_uint32 read_and_mix_pcm_frames_f32(ma_decoder* pDecoder, float* pOutputF32, ma_uint32 frameCount)
{
    /*
    The way mixing works is that we just read into a temporary buffer, then take the contents of that buffer and mix it with the
    contents of the output buffer by simply adding the samples together. You could also clip the samples to -1..+1, but I'm not
    doing that in this example.
    */
    ma_result result;
    float temp[4096];
    ma_uint32 tempCapInFrames = ma_countof(temp) / CHANNEL_COUNT;
    ma_uint32 totalFramesRead = 0;

    while (totalFramesRead < frameCount) {
        ma_uint64 iSample;
        ma_uint64 framesReadThisIteration;
        ma_uint32 totalFramesRemaining = frameCount - totalFramesRead;
        ma_uint32 framesToReadThisIteration = tempCapInFrames;
        if (framesToReadThisIteration > totalFramesRemaining) {
            framesToReadThisIteration = totalFramesRemaining;
        }

        //get chunk of data into "temp"
        result = ma_decoder_read_pcm_frames(pDecoder, temp, framesToReadThisIteration, &framesReadThisIteration);
        if (result != MA_SUCCESS || framesReadThisIteration == 0) {
            break;
        }

        /* Mix the frames together. */
        for (iSample = 0; iSample < framesReadThisIteration * CHANNEL_COUNT; ++iSample) {
            pOutputF32[totalFramesRead * CHANNEL_COUNT + iSample] += temp[iSample];
        }

        totalFramesRead += (ma_uint32)framesReadThisIteration;

        if (framesReadThisIteration < (ma_uint32)framesToReadThisIteration) {
            break;  /* Reached EOF. */
        }
    }

    return totalFramesRead;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    // In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
    // pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
    // frameCount frames.
    
    //ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;

    float temp[4096];
    float* pOutputF32 = (float*)pOutput;

    std::vector<ma_decoder> pDecoderList = *(std::vector<ma_decoder>*)pDevice->pUserData;
    if (!pDecoderList.data()) {
        return;
    }

    for (int i = 0; i < pDecoderList.size(); i++)
    {
        read_and_mix_pcm_frames_f32(&pDecoderList[i], pOutputF32, frameCount);
    }


    (void)pInput;
}

struct DeviceManager
{
    ma_device device1;
    ma_device device2;

    std::vector<ma_decoder> pDecoders1;
    std::vector<ma_decoder> pDecoders2;

    DeviceManager(ma_device_info* pPlaybackInfos)
    {
        ma_device_config config_device1 = ma_device_config_init(ma_device_type_playback);
        config_device1.playback.pDeviceID = &pPlaybackInfos[3].id;
        config_device1.playback.format = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
        config_device1.playback.channels = 2;               // Set to 0 to use the device's native channel count.
        config_device1.sampleRate = 44100;           // Set to 0 to use the device's native sample rate.
        config_device1.dataCallback = data_callback;   // This function will be called when miniaudio needs more data.
        config_device1.pUserData = &pDecoders1;   // Can be accessed from the device object (device.pUserData).

        ma_device_config config_device2 = ma_device_config_init(ma_device_type_playback);
        config_device2.playback.pDeviceID = &pPlaybackInfos[0].id;
        config_device2.playback.format = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
        config_device2.playback.channels = 2;               // Set to 0 to use the device's native channel count.
        config_device2.sampleRate = 44100;           // Set to 0 to use the device's native sample rate.
        config_device2.dataCallback = data_callback;   // This function will be called when miniaudio needs more data.
        config_device2.pUserData = &pDecoders2;   // Can be accessed from the device object (device.pUserData).

        if (ma_device_init(NULL, &config_device1, &device1) != MA_SUCCESS ||
            ma_device_init(NULL, &config_device2, &device2) != MA_SUCCESS) {
            printf("Failed to initialize devices in DeviceManager struct");
            return;
        }

        ma_device_stop(&device1);
        ma_device_stop(&device2);
    }

    ~DeviceManager()
    {
        ma_device_uninit(&device1);
        ma_device_uninit(&device2);
        pDecoders1.clear();
        pDecoders2.clear();
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
        pDecoders1.clear();
        pDecoders2.clear();
    }

    void addSound(std::string path)
    {
        ma_decoder decoder1;
        ma_decoder decoder2;
        if (ma_decoder_init_file(path.c_str(), NULL, &decoder1) != MA_SUCCESS ||
            ma_decoder_init_file(path.c_str(), NULL, &decoder2) != MA_SUCCESS)
        {
            printf("Failed to add sound in DeviceManager struct");
            return;
        }
        pDecoders1.push_back(decoder1);
        pDecoders2.push_back(decoder2);
    }
};

int main()
{
    ma_context context;
    if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) {
        return 1;
    }

    ma_device_info* pPlaybackInfos;
    ma_uint32 playbackCount;
    ma_device_info* pCaptureInfos;
    ma_uint32 captureCount;
    if (ma_context_get_devices(&context, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount) != MA_SUCCESS) {
        return 1;
    }

    // Loop over each device info and do something with it. Here we just print the name with their index. You may want
    // to give the user the opportunity to choose which device they'd prefer.
    for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1) {
        printf("%d - %s\n", iDevice, pPlaybackInfos[iDevice].name);
    }

   // ma_decoder decoder;
   // ma_decoder decoder2;
   // if (ma_decoder_init_file("eple.mp3", NULL, &decoder) != MA_SUCCESS || 
   //     ma_decoder_init_file("eple.mp3", NULL, &decoder2) != MA_SUCCESS)
   // {
   //     printf("Decoder");
   //     return 0;
   // }
   // pDecoders1.push_back(decoder);
   // pDecoders2.push_back(decoder2);
//
   // ma_device_config config_device1 = ma_device_config_init(ma_device_type_playback);
   // config_device1.playback.pDeviceID = &pPlaybackInfos[3].id;
   // config_device1.playback.format = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
   // config_device1.playback.channels = 2;               // Set to 0 to use the device's native channel count.
   // config_device1.sampleRate = 44100;           // Set to 0 to use the device's native sample rate.
   // config_device1.dataCallback = data_callback;   // This function will be called when miniaudio needs more data.
   // config_device1.pUserData = &pDecoders1;   // Can be accessed from the device object (device.pUserData).
//
   // ma_device_config config_device2 = ma_device_config_init(ma_device_type_playback);
   // config_device2.playback.pDeviceID = &pPlaybackInfos[0].id;
   // config_device2.playback.format = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
   // config_device2.playback.channels = 2;               // Set to 0 to use the device's native channel count.
   // config_device2.sampleRate = 44100;           // Set to 0 to use the device's native sample rate.
   // config_device2.dataCallback = data_callback;   // This function will be called when miniaudio needs more data.
   // config_device2.pUserData = &pDecoders2;   // Can be accessed from the device object (device.pUserData).
//
   // ma_device device1;
   // ma_device device2;
   // if (ma_device_init(NULL, &config_device1, &device1) != MA_SUCCESS || 
   //     ma_device_init(NULL, &config_device2, &device2) != MA_SUCCESS) {
   //     return -1;  // Failed to initialize the device.
   // }
//
   // ma_device_stop(&device1);
   // ma_device_stop(&device2);
   // //ma_device_start(&device);     // The device is sleeping by default so you'll need to start it manually.
//
   // // Do something here. Probably your program's main loop.
   // std::this_thread::sleep_for(std::chrono::seconds(1)); 
   // ma_device_start(&device1);
   // ma_device_start(&device2);
   // //std::this_thread::sleep_for(std::chrono::seconds(3));
   // //ma_device_stop(&device1);
   // //ma_device_stop(&device2);
   // //std::this_thread::sleep_for(std::chrono::seconds(1));
   // //ma_device_start(&device1);
   // //ma_device_start(&device2);

   // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
   // ma_decoder decoder3;
   // ma_decoder decoder4;
   // ma_decoder_init_file("eple.mp3", NULL, &decoder3);
   // ma_decoder_init_file("eple.mp3", NULL, &decoder4);
   // pDecoders1.push_back(decoder3);
   // pDecoders2.push_back(decoder4);
   // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
   // //ma_device_stop(&device1);
   // pDecoders1.erase(pDecoders1.begin() + 1);
   // //ma_device_start(&device1);

    DeviceManager device(pPlaybackInfos);
    device.start();
    device.addSound("eple.mp3");

    while (1)
    {

    }

    ma_context_uninit(&context);
    return 0;
}