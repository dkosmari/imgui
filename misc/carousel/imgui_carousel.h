// -*- mode: c++ -*-
/*
 * Carousel widget for ImGui.
 *
 * Copyright (C) 2026  Daniel K. O. <dkosmari>
 * SPDX-License-Identifier: MIT
 */

#ifndef IMGUI_CAROUSEL_H
#define IMGUI_CAROUSEL_H

#ifndef IMGUI_DISABLE

#include <string>
#include <utility>

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
     * Return the number of horizontal and vertical pages needed to show all content.
     *
     * This is slightly more efficient than calling both CarouselGetColumns() and
     * CarouselGetRows().
     */
    IMGUI_API
    std::pair<int, int>
    CarouselGetColumnsRows();

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
     * Return both horizontal and vertical page.
     *
     * This is slightly more efficient than calling both CarouselGetPageX() and
     * CarouselGetPageY().
     */
    IMGUI_API
    std::pair<int, int>
    CarouselGetPage();

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
     * Jump to a specific horizontal and vertical page.
     *
     * This is slightly more efficient than calling both CarouselJumpPageX() and
     * CarouselJumpPageY().
     */
    IMGUI_API
    void
    CarouselJumpPage(int page_x, int page_y);

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


    // C++ overloads

    /// Overload for std::string.
    IMGUI_API
    bool
    BeginCarousel(const std::string& str_id,
                  const ImVec2& page_size,
                  const ImGuiCarouselSpecs& specs = {});

#if __cplusplus >= 202002L // C++20

    // RAII wrapper
    namespace RAII {

        // NOTE: detail namespace copied from imgui_raii.h

        namespace detail {

            struct Immovable {

                constexpr
                Immovable() noexcept = default;

                Immovable(Immovable&&) = delete;

            }; // struct Immovable


            class Conditional : Immovable {

            protected:

                const bool status;

                Conditional(bool s)
                    noexcept :
                    status{s}
                {}

            public:

                explicit
                operator bool()
                    const noexcept
                {
                    return status;
                }

            }; // class Conditional

        } // namespace detail


        struct Carousel : detail::Conditional {

            Carousel(const auto& str_id,
                     const ImVec2& page_size,
                     const ImGuiCarouselSpecs& specs = {})
                noexcept :
                detail::Conditional{BeginCarousel(str_id, page_size, specs)}
            {}

            ~Carousel()
                noexcept
            {
                if (status)
                    EndCarousel();
            }

        }; // struct Carousel

    } // namespace RAII

#endif // C++20

} // namespace ImGui

#endif // IMGUI_DISABLE

#endif
