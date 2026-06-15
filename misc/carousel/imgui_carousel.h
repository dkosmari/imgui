#ifndef IMGUI_CAROUSEL_H
#define IMGUI_CAROUSEL_H

#ifndef IMGUI_DISABLE

// Carousel widget implementation.
#include <imgui.h>

/**
 * Options for Carousel windows.
 */
struct ImGuiCarouselSpecs {
    /// How long a snap animation lasts, in seconds.
    float            snap_duration     = 0.25f;
    /// If flick scrolling is enabled, it replaces kinetic scrolling.
    bool             flickable         = true;
    /// If a horizontal scrollbar is desired.
    bool             enable_hscrollbar = false;
    /// If a vertical scrollbar is desired.
    bool             enable_vscrollbar = false;
    /// Custom ImGuiChildFlags for the Carousel window.
    ImGuiChildFlags  child_flags       = ImGuiChildFlags_None;
    /// Custom ImGuiWindowFlags for the Carousel window.
    ImGuiWindowFlags window_flags      = ImGuiWindowFlags_None;
};

namespace ImGui
{
    /**
     * Begin a Carousel child window.
     */
    IMGUI_API
    bool
    BeginCarousel(const char* str_id,
                  const ImVec2& page_size,
                  const ImGuiCarouselSpecs& specs = {});

    /**
     * End a Carousel child window.
     *
     * \note Only call this function if BeginCarousel() returned `true`.
     */
    IMGUI_API
    void
    EndCarousel();

    /**
     * Return the number of horizontal pages needed to show all content.
     *
     * Call after adding all content.
     */
    IMGUI_API
    int
    CarouselGetColumns();

    /**
     * Return the number of vertical pages needed to show all content.
     *
     * Call after adding all content.
     */
    IMGUI_API
    int
    CarouselGetRows();

    /**
     * Return the current horizontal page.
     */
    IMGUI_API
    int
    CarouselGetPageX();

    /**
     * Return the current vertical page.
     */
    IMGUI_API
    int
    CarouselGetPageY();

    /**
     * Jump to a specific horizontal page.
     */
    IMGUI_API
    void
    CarouselJumpPageX(int page_x);

    /**
     * Jump to a specific vertical page.
     */
    IMGUI_API
    void
    CarouselJumpPageY(int page_y);

    /**
     * Jump one page to the left.
     */
    IMGUI_API
    void
    CarouselJumpPageLeft();

    /**
     * Jump one page to the right.
     */
    IMGUI_API
    void
    CarouselJumpPageRight();

    /**
     * Jump one page up.
     */
    IMGUI_API
    void
    CarouselJumpPageUp();

    /**
     * Jump one page down.
     */
    IMGUI_API
    void
    CarouselJumpPageDown();

} // namespace ImGui

#endif // IMGUI_DISABLE

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
