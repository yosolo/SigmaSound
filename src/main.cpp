#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_demo.cpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include "imgui_spectrum.h"

namespace ImGui
{
    void StyleColorsSpectrum() {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->GrabRounding = 4.0f;

        ImVec4* colors = style->Colors;
        colors[ImGuiCol_Text] = ColorConvertU32ToFloat4(Spectrum::GRAY800); // text on hovered controls is gray900
        colors[ImGuiCol_TextDisabled] = ColorConvertU32ToFloat4(Spectrum::GRAY500);
        colors[ImGuiCol_WindowBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ColorConvertU32ToFloat4(Spectrum::GRAY50); // not sure about this. Note: applies to tooltips too.
        colors[ImGuiCol_Border] = ColorConvertU32ToFloat4(Spectrum::GRAY300);
        colors[ImGuiCol_BorderShadow] = ColorConvertU32ToFloat4(Spectrum::Static::NONE); // We don't want shadows. Ever.
        colors[ImGuiCol_FrameBg] = ColorConvertU32ToFloat4(Spectrum::GRAY75); // this isnt right, spectrum does not do this, but it's a good fallback
        colors[ImGuiCol_FrameBgHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY50);
        colors[ImGuiCol_FrameBgActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
        colors[ImGuiCol_TitleBg] = ColorConvertU32ToFloat4(Spectrum::GRAY300); // those titlebar values are totally made up, spectrum does not have this.
        colors[ImGuiCol_TitleBgActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
        colors[ImGuiCol_TitleBgCollapsed] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
        colors[ImGuiCol_MenuBarBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100);
        colors[ImGuiCol_ScrollbarBg] = ColorConvertU32ToFloat4(Spectrum::GRAY100); // same as regular background
        colors[ImGuiCol_ScrollbarGrab] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
        colors[ImGuiCol_ScrollbarGrabHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
        colors[ImGuiCol_ScrollbarGrabActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
        colors[ImGuiCol_CheckMark] = ColorConvertU32ToFloat4(Spectrum::BLUE500);
        colors[ImGuiCol_SliderGrab] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
        colors[ImGuiCol_SliderGrabActive] = ColorConvertU32ToFloat4(Spectrum::GRAY800);
        colors[ImGuiCol_Button] = ColorConvertU32ToFloat4(Spectrum::GRAY75); // match default button to Spectrum's 'Action Button'.
        colors[ImGuiCol_ButtonHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY50);
        colors[ImGuiCol_ButtonActive] = ColorConvertU32ToFloat4(Spectrum::GRAY200);
        colors[ImGuiCol_Header] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
        colors[ImGuiCol_HeaderHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE500);
        colors[ImGuiCol_HeaderActive] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
        colors[ImGuiCol_Separator] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
        colors[ImGuiCol_SeparatorHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
        colors[ImGuiCol_SeparatorActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
        colors[ImGuiCol_ResizeGrip] = ColorConvertU32ToFloat4(Spectrum::GRAY400);
        colors[ImGuiCol_ResizeGripHovered] = ColorConvertU32ToFloat4(Spectrum::GRAY600);
        colors[ImGuiCol_ResizeGripActive] = ColorConvertU32ToFloat4(Spectrum::GRAY700);
        colors[ImGuiCol_PlotLines] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
        colors[ImGuiCol_PlotLinesHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
        colors[ImGuiCol_PlotHistogram] = ColorConvertU32ToFloat4(Spectrum::BLUE400);
        colors[ImGuiCol_PlotHistogramHovered] = ColorConvertU32ToFloat4(Spectrum::BLUE600);
        colors[ImGuiCol_TextSelectedBg] = ColorConvertU32ToFloat4((Spectrum::BLUE400 & 0x00FFFFFF) | 0x33000000);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ColorConvertU32ToFloat4((Spectrum::GRAY900 & 0x00FFFFFF) | 0x0A000000);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    }
}

int main(int argc, char** argv)
{
    ma_result result;
    ma_engine engine;

    ma_context context;
    if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) {
        return 1;
    }

    ma_device_info* pPlaybackInfos;
    ma_uint32 playbackIndex;
    ma_uint32 playbackCount;
    ma_device_info* pCaptureInfos;
    ma_uint32 captureCount;
    result = ma_context_get_devices(&context, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount);
    if (result != MA_SUCCESS) 
    {
        return result;
    }

    // Loop over each device info and do something with it. Here we just print the name with their index. You may want
    // to give the user the opportunity to choose which device they'd prefer.
    for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1) 
    {
        printf("%d - %s\n", iDevice, pPlaybackInfos[iDevice].name);
        if (pPlaybackInfos[iDevice].isDefault)
            playbackIndex = iDevice;
    }

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.pDeviceID = &pPlaybackInfos[playbackIndex].id;

    ma_device primaryDevice;
    ma_device_init(&context, &deviceConfig, &primaryDevice);
    ma_engine_config engineConfig = ma_engine_config_init();
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) 
    {
        return result;  // Failed to initialize the engine.
    }

#define DELAY_IN_SECONDS    0.2f
#define DECAY               0.25f   /* Volume falloff for each echo. */

    // ma_delay_node delayNode;
    // {
    //     ma_delay_node_config delayNodeConfig;
    //     ma_uint32 channels;
    //     ma_uint32 sampleRate;
    // 
    //     channels = ma_engine_get_channels(&engine);
    //     sampleRate = ma_engine_get_sample_rate(&engine);
    // 
    //     delayNodeConfig = ma_delay_node_config_init(channels, sampleRate, (ma_uint32)(sampleRate * DELAY_IN_SECONDS), DECAY);
    // 
    //     result = ma_delay_node_init(ma_engine_get_node_graph(&engine), &delayNodeConfig, NULL, &delayNode);
    //     if (result != MA_SUCCESS) {
    //         printf("Failed to initialize delay node.");
    //         return -1;
    //     }
    // 
    //     /* Connect the output of the delay node to the input of the endpoint. */
    //     ma_node_attach_output_bus(&delayNode, 0, ma_engine_get_endpoint(&engine), 0);
    // }

    ma_sound sound;
    result = ma_sound_init_from_file(&engine, "eple.mp3", 0, nullptr, nullptr, &sound);

    // ma_node_attach_output_bus(&sound, 0, &delayNode, 0);
    // ma_sound_set_start_time_in_milliseconds(&sound, 5000.0f);
    // ma_sound_start(&sound);

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Sound", nullptr, nullptr);
    glfwMakeContextCurrent(window);


    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 15.0f);
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

#if 0
    ImGui::StyleColorsSpectrum();
#else
    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.39f, 0.38f, 0.77f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.92f, 0.91f, 0.88f, 0.70f);
    style->Colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.58f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.92f, 0.91f, 0.88f, 0.92f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.84f, 0.83f, 0.80f, 0.65f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.99f, 1.00f, 0.40f, 0.78f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.47f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.21f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.90f, 0.91f, 0.00f, 0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    // style->Colors[ImGuiCol_ComboBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 1.00f, 0.00f, 0.80f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.99f, 1.00f, 0.22f, 0.86f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.25f, 1.00f, 0.00f, 0.76f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.86f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    // style->Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.00f, 0.00f, 0.32f);
    // style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.78f);
    // style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.78f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    // style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    // style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    // style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    // style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
#endif

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool use_work_area = false;
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

        // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
        // Based on your use case you may want one or the other.
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

        ImGui::Begin("App", 0, flags);
        
        const char* playbackDevicePreview = pPlaybackInfos[playbackIndex].name;
        if (ImGui::BeginCombo("Playback Device", playbackDevicePreview))
        {
            for (int n = 0; n < playbackCount; n++)
            {
                ma_uint32 oldIndex = playbackIndex;

                const bool is_selected = (playbackIndex == n);
                if (ImGui::Selectable(pPlaybackInfos[n].name, is_selected))
                    playbackIndex = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();

                if (oldIndex != playbackIndex)
                {
                    // ma_device_init()

                    ma_sound_stop(&sound);
                    ma_sound_uninit(&sound);
                    ma_engine_uninit(&engine);

                    engineConfig.pPlaybackDeviceID = &pPlaybackInfos[playbackIndex].id;
                    result = ma_engine_init(&engineConfig, &engine);
                    if (result != MA_SUCCESS)
                    {
                        return result;  // Failed to initialize the engine.
                    }

                    result = ma_sound_init_from_file(&engine, "eple.mp3", 0, nullptr, nullptr, &sound);
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Play")) 
        {
            static bool playing = false;
            if (!playing)
                ma_sound_start(&sound);
            else
                ma_sound_stop(&sound);

            playing = !playing;
        }
        ImGui::End();

        // ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        ImGui::EndFrame();
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);

        glfwSwapBuffers(window);
    }

    return 0;
}
