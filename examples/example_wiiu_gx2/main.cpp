#include <cassert>
#include <cstdio>
#include <coreinit/memory.h>
#include <gx2/registers.h>
#include <gx2/swap.h>
#include <padscore/kpad.h>
#include <sndcore2/core.h>
#include <vpad/input.h>
#include <whb/gfx.h>
#include <whb/proc.h>

#include <imgui.h>
#include <imgui_impl_wiiu.h>
#include <imgui_impl_gx2.h>


static const float default_font_size = 32;

void
load_font(OSSharedDataType font, bool merge)
{
    static const char* names[OS_SHAREDDATATYPE_FONT_MAX] = {
        [OS_SHAREDDATATYPE_FONT_CHINESE]   = "CafeCn.ttf",
        [OS_SHAREDDATATYPE_FONT_KOREAN]    = "CafeKr.ttf",
        [OS_SHAREDDATATYPE_FONT_STANDARD]  = "CafeStd.ttf",
        [OS_SHAREDDATATYPE_FONT_TAIWANESE] = "CafeCn.ttf",
    };

    assert(font < OS_SHAREDDATATYPE_FONT_MAX);

    auto& io = ImGui::GetIO();
    ImFontConfig config;
    config.MergeMode = merge;
    config.EllipsisChar = U'…';
    // config.GlyphOffset.y = - default_font_size * (4.0f / 32.0f);
    config.FontDataOwnedByAtlas = false;

    void* font_data = nullptr;
    uint32_t font_size = 0;

    if (OSGetSharedData(font, 0,
                        &font_data, &font_size))
    {
        std::snprintf(config.Name, sizeof config.Name, names[font]);
        io.Fonts->AddFontFromMemoryTTF(font_data, font_size,
                                       default_font_size, &config);
    }
}

void
load_fonts()
{
    load_font(OS_SHAREDDATATYPE_FONT_STANDARD, false);
    load_font(OS_SHAREDDATATYPE_FONT_CHINESE, true);
    load_font(OS_SHAREDDATATYPE_FONT_KOREAN, true);
    load_font(OS_SHAREDDATATYPE_FONT_TAIWANESE, true);
}

int
main()
{
    // Briefly initialize the audio system to stop the boot sound.
    AXInit();
    AXQuit();

    KPADInit();
    WPADEnableURCC(true);
    // WPADEnableWBC();

    WHBProcInit();

    WHBGfxInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    io.LogFilename = nullptr; // don't save log
    io.IniFilename = nullptr; // don't save ini

    io.ConfigDragScroll = true;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.MouseDragThreshold = 25;
    io.ConfigInputTrickleEventQueue = false;

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(2.0f);
    style.FontSizeBase = default_font_size;

    load_fonts();

    // Initialize platform/renderer backends.
    ImGui_ImplWiiU_Init();
    ImGui_ImplGX2_Init();

    VPADStatus vpad;
    KPADStatus kpad[4];
    ImGui_ImplWiiU_ControllerInput wiiu_input;

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (WHBProcIsRunning())
    {

        // Read inputs.
        wiiu_input = {};
        {
            auto ch = VPAD_CHAN_0;
            VPADReadError error;
            auto r = VPADRead(ch, &vpad, 1, &error);
            if (r == 1 && error == VPAD_READ_SUCCESS) {
                wiiu_input.vpad = &vpad;
            }
        }

        for (int ch = 0; ch < 4; ++ch)
        {
            auto r = KPADRead(static_cast<KPADChan>(ch), &kpad[ch], 1);
            if (r == 1)
                wiiu_input.kpad[ch] = &kpad[ch];
        }
        ImGui_ImplWiiU_ProcessInput(&wiiu_input);

        // Process ImGui GUI.

        // We will render on the TV, and copy the result to the DRC.
        GX2ColorBuffer* cb = WHBGfxGetTVColourBuffer();

        // Start the Dear ImGui frame
        ImGui_ImplWiiU_NewFrame(cb);
        ImGui_ImplGX2_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        ImGui::EndFrame();

        // Rendering
        ImGui::Render();

        WHBGfxBeginRender();

        WHBGfxBeginRenderTV();
        GX2SetViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 1.0f);
        WHBGfxClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        // Draw to TV
        ImGui_ImplGX2_RenderDrawData(ImGui::GetDrawData());

        // Render keyboard overlay
        GX2SetViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 1.0f);
        GX2SetScissor(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        ImGui_ImplWiiU_DrawKeyboardOverlay(ImGui_KeyboardOverlay_Auto);

        WHBGfxFinishRenderTV();

        // Copy TV to DRC
        GX2CopyColorBufferToScanBuffer(WHBGfxGetTVColourBuffer(), GX2_SCAN_TARGET_DRC);

        WHBGfxFinishRender();
    }

    ImGui_ImplGX2_Shutdown();
    ImGui_ImplWiiU_Shutdown();

    ImGui::DestroyContext();

    WHBGfxShutdown();

    WHBProcShutdown();

    KPADShutdown();
}
