#include <coreinit/memory.h>
#include <gx2/registers.h>
#include <gx2/swap.h>
#include <padscore/kpad.h>
#include <vpad/input.h>
#include <whb/gfx.h>
#include <whb/proc.h>

#include <imgui.h>
#include <imgui_impl_wiiu.h>
#include <imgui_impl_gx2.h>


static const float default_font_size = 32;

void
load_fonts()
{
    auto& io = ImGui::GetIO();
    // Load main font: CafeStd
    ImFontConfig config;
    config.EllipsisChar = U'…';
    // config.GlyphOffset.y = - default_font_size * (4.0f / 32.0f);
    config.FontDataOwnedByAtlas = false;

    void* font_data = nullptr;
    uint32_t font_size = 0;

    if (OSGetSharedData(OS_SHAREDDATATYPE_FONT_STANDARD, 0,
                        &font_data, &font_size))
        io.Fonts->AddFontFromMemoryTTF(font_data, font_size,
                                       default_font_size, &config);

    config.MergeMode = true;

    if (OSGetSharedData(OS_SHAREDDATATYPE_FONT_CHINESE, 0,
                        &font_data, &font_size))
        io.Fonts->AddFontFromMemoryTTF(font_data, font_size,
                                       default_font_size, &config);

    if (OSGetSharedData(OS_SHAREDDATATYPE_FONT_KOREAN, 0,
                        &font_data, &font_size))
        io.Fonts->AddFontFromMemoryTTF(font_data, font_size,
                                       default_font_size, &config);

    if (OSGetSharedData(OS_SHAREDDATATYPE_FONT_TAIWANESE, 0,
                        &font_data, &font_size))
        io.Fonts->AddFontFromMemoryTTF(font_data, font_size,
                                       default_font_size, &config);
}

int
main()
{
    KPADInit();
    WPADEnableURCC(true);
    // WPADEnableWBC();

    WHBProcInit();

    WHBGfxInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
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

    while (WHBProcIsRunning()) {

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

        for (int ch = 0; ch < 4; ++ch) {
            auto r = KPADRead(static_cast<KPADChan>(ch), &kpad[ch], 1);
            if (r == 1)
                wiiu_input.kpad[ch] = &kpad[ch];
        }
        ImGui_ImplWiiU_ProcessInput(&wiiu_input);

        // Process ImGui GUI.
        auto cb = WHBGfxGetTVColourBuffer();
        io.DisplaySize.x = cb->surface.width;
        io.DisplaySize.y = cb->surface.height;

        ImGui_ImplGX2_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::EndFrame();
        ImGui::Render();


        WHBGfxBeginRender();

        WHBGfxBeginRenderTV();
        GX2SetViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 1.0f);
        WHBGfxClearColor(0.4f, 0.0f, 0.2f, 1.0f);
        // Draw to TV
        ImGui_ImplGX2_RenderDrawData(ImGui::GetDrawData());

        // Render keyboard overlay
        GX2SetViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 1.0f);
        GX2SetScissor(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        // Gamma fix: otherwise the swkbd comes out with the wrong colors.
        if (cb->surface.format == GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8) {
            auto old_format = cb->surface.format;
            cb->surface.format = GX2_SURFACE_FORMAT_SRGB_R8_G8_B8_A8;
            GX2InitColorBufferRegs(cb);
            GX2SetColorBuffer(cb, GX2_RENDER_TARGET_0);
            ImGui_ImplWiiU_DrawKeyboardOverlay();
            cb->surface.format = old_format;
            GX2InitColorBufferRegs(cb);
            GX2SetColorBuffer(cb, GX2_RENDER_TARGET_0);
        } else {
            ImGui_ImplWiiU_DrawKeyboardOverlay();
        }

        WHBGfxFinishRenderTV();

        // Copy TV to DRC
        GX2CopyColorBufferToScanBuffer(WHBGfxGetTVColourBuffer(),
                                       GX2_SCAN_TARGET_DRC);

        WHBGfxFinishRender();
    }

    ImGui_ImplGX2_Shutdown();
    ImGui_ImplWiiU_Shutdown();

    ImGui::DestroyContext();

    WHBGfxShutdown();

    WHBProcShutdown();

    KPADShutdown();
}
