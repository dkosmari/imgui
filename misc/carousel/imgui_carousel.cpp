/*
 * Carousel widget for ImGui.
 *
 * Copyright (C) 2026  Daniel K. O. <dkosmari>
 * SPDX-License-Identifier: MIT
 */

#include <cmath>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_carousel.h"


namespace {

    enum class CarouselSnapMode : int {
        jump,
        flick,
        nudge,
    };


    enum class CarouselSnapState : int {
        idle,
        started,
        snapping,
    };


    template<typename Pt>
    Pt
    cubic_hermite_spline(const Pt& p0,
                         const Pt& p1,
                         const Pt& d0,
                         const Pt& d1,
                         float t)
    {
        if (t < 0)
            return p0;
        if (t > 1)
            return p1;
        Pt a =  2 * p0 - 2 * p1 +     d0 + d1;
        Pt b = -3 * p0 + 3 * p1 - 2 * d0 - d1;
        Pt c =                        d0;
        Pt d =      p0;
        return a * t*t*t + b * t*t + c * t + d;
    }


    template<typename Pt>
    Pt
    cubic_hermite_spline_deriv(const Pt& p0,
                               const Pt& p1,
                               const Pt& d0,
                               const Pt& d1,
                               float t)
    {
        if (t < 0)
            return d0;
        if (t > 1)
            return d1;
        Pt a =  2 * p0 - 2 * p1 +     d0 + d1;
        Pt b = -3 * p0 + 3 * p1 - 2 * d0 - d1;
        Pt c =                        d0;
        return 3 * a * t*t + 2 * b * t + c;
    }


    template<typename T>
    T
    clamp(T x, T min, T max)
    {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

} // namespace


bool
ImGui::BeginCarousel(const char *str_id,
                     const ImVec2& page_size,
                     const ImGuiCarouselSpecs& specs)
{
    PushID(str_id);

    const ImGuiStyle& style = GetStyle();
    ImVec2 carousel_size = page_size;

    ImGuiWindowFlags window_flags = specs.window_flags;
    if (!specs.enable_hscrollbar && !specs.enable_vscrollbar)
        window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (specs.enable_hscrollbar)
    {
        window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
        window_flags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
        carousel_size.y += style.ScrollbarSize;
    }
    if (specs.enable_vscrollbar)
    {
        window_flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
        carousel_size.x += style.ScrollbarSize;
    }

    bool visible = BeginChild("#CAROUSEL", carousel_size, specs.child_flags, window_flags);
    if (!visible) {
        EndChild();
        PopID();
        return false;
    }

    ImGuiStorage* storage = GetStateStorage();
    storage->SetFloat(GetID("snap_duration"), specs.snap_duration);
    storage->SetBool(GetID("flickable"), specs.flickable);
    storage->SetFloat(GetID("page_size.x"), page_size.x);
    storage->SetFloat(GetID("page_size.y"), page_size.y);

    ImVec2 step_size = page_size + style.ItemSpacing;
    storage->SetFloat(GetID("step_size.x"), step_size.x);
    storage->SetFloat(GetID("step_size.y"), step_size.y);

    return true;
}


void
ImGui::EndCarousel()
{
    ImVec2 current_position{ GetScrollX(), GetScrollY() };
    ImVec2 max_scroll{ GetScrollMaxX(), GetScrollMaxY() };

    if (max_scroll.x == 0 && max_scroll.y == 0)
    {
        EndChild();
        PopID();
        // early out, when there's nothing to scroll.
        return;
    }

    ImGuiStorage* storage = GetStateStorage();

    ImVec2 step_size;
    step_size.x = storage->GetFloat(GetID("step_size.x"));
    step_size.y = storage->GetFloat(GetID("step_size.y"));

    int columns = step_size.x > 0
        ? 1.0f + std::floor(max_scroll.x / step_size.x)
        : 1.0f;
    int rows    = step_size.y > 0
        ? 1.0f + std::floor(max_scroll.y / step_size.y)
        : 1.0f;

    int current_page_x = std::lround(current_position.x / step_size.x);
    int current_page_y = std::lround(current_position.y / step_size.y);

    int target_page_x = storage->GetInt(GetID("target_page_x"), current_page_x);
    int target_page_y = storage->GetInt(GetID("target_page_y"), current_page_y);

    bool flickable = storage->GetBool(GetID("flickable"));
    // bool scrollbar = storage->GetBool(GetID("scrollbar"));
    bool scrollbar_active =
        IsWindowVerticalScrollbarActive() || IsWindowHorizontalScrollbarActive();
    CarouselSnapMode snap_mode = storage->Get<CarouselSnapMode>(GetID("snap_mode"),
                                                                CarouselSnapMode::jump);
    CarouselSnapState snap_state = storage->Get<CarouselSnapState>(GetID("snap_state"),
                                                                   CarouselSnapState::idle);

    ImVec2 snap_start_position;
    snap_start_position.x = storage->GetFloat(GetID("snap_start_position.x"));
    snap_start_position.y = storage->GetFloat(GetID("snap_start_position.y"));

    ImVec2 current_velocity;
    current_velocity.x = storage->GetFloat(GetID("current_velocity.x"));
    current_velocity.y = storage->GetFloat(GetID("current_velocity.y"));
    ImVec2 snap_start_velocity;
    snap_start_velocity.x = storage->GetFloat(GetID("snap_start_velocity.x"));
    snap_start_velocity.y = storage->GetFloat(GetID("snap_start_velocity.y"));

    float snap_time = storage->GetFloat(GetID("snap_time"));

    // If user started scrolling the content, the buttons have no more control.
    if (scrollbar_active)
    {
        snap_mode = CarouselSnapMode::nudge;
        snap_state = CarouselSnapState::idle;
        current_velocity = {};
    }
    else if (IsDragScrolling())
    {
        snap_mode = CarouselSnapMode::nudge;
        snap_state = CarouselSnapState::idle;
        current_velocity = GetDragScrollVelocity();
    }

    if (snap_mode == CarouselSnapMode::nudge)
    {
        target_page_x = current_page_x;
        target_page_y = current_page_y;
        ImVec2 snap_finish_position;
        snap_finish_position.x = target_page_x * step_size.x;
        snap_finish_position.y = target_page_y * step_size.y;
        // If user is not manipulating the scroll position, start the scroll snap.
        if (snap_state == CarouselSnapState::idle
            && !IsDragScrolling()
            && !IsDragScrollGliding()
            && !scrollbar_active
            && snap_finish_position != current_position)
        {
            snap_state = CarouselSnapState::started;
            snap_start_velocity = {};
        }
    }

    // If a flick happened, setup the snap.
    if (flickable)
    {
        ImVec2 flick = GetDragScrollFlick();
        bool got_new_snap_start_velocity = false;
        ImVec2 new_snap_start_velocity = {0, 0};
        if (flick.x != 0)
        {
            snap_mode = CarouselSnapMode::flick;
            snap_state = CarouselSnapState::started;
            got_new_snap_start_velocity = true;
            new_snap_start_velocity.x = flick.x;
            target_page_x = current_page_x;
            if (flick.x < 0 && target_page_x > 0)
                --target_page_x;
            if (flick.x > 0 && target_page_x < columns - 1)
                ++target_page_x;
        }
        if (flick.y != 0)
        {
            snap_mode = CarouselSnapMode::flick;
            snap_state = CarouselSnapState::started;
            got_new_snap_start_velocity = true;
            new_snap_start_velocity.y = flick.y;
            target_page_y = current_page_y;
            if (flick.y < 0 && target_page_y > 0)
                --target_page_y;
            if (flick.y > 0 && target_page_y < rows - 1)
                ++target_page_y;
        }
        if (got_new_snap_start_velocity)
            snap_start_velocity = new_snap_start_velocity;
    }


    // Actual snapping code starts here.

    if (snap_state == CarouselSnapState::started)
    {
        snap_time = 0;
        snap_start_position = current_position;
        snap_state = CarouselSnapState::snapping;
    }


    if (snap_state == CarouselSnapState::snapping)
    {
        ImVec2 snap_finish_position;
        snap_finish_position.x = target_page_x * step_size.x;
        snap_finish_position.y = target_page_y * step_size.y;
        SetDragScrollVelocity({0, 0});
        // Interpolate between start_pos and target_pos, using Hermite
        // interpolation.
        float snap_duration = storage->GetFloat(GetID("snap_duration"), 0.25f);
        float t = snap_time / snap_duration;
        /*
         * Prevent overshooting.
         *
         * Fritsch, F. N.; Carlson, R. E. (1980). "Monotone Piecewise Cubic
         * Interpolation". SIAM Journal on Numerical Analysis. 17 (2). SIAM:
         * 238–246. doi:10.1137/0717021.
         */
        const ImVec2 snap_finish_velocity{0, 0};
        ImVec2 secant_slope = snap_finish_position - snap_start_position;
        if (std::signbit(secant_slope.x) == std::signbit(snap_start_velocity.x)
            && snap_start_velocity.x / secant_slope.x > 3)
            snap_start_velocity.x = 3 * secant_slope.x;
        if (std::signbit(secant_slope.y) == std::signbit(snap_start_velocity.y)
            && snap_start_velocity.y / secant_slope.y > 3)
            snap_start_velocity.y = 3 * secant_slope.y;
        current_position = cubic_hermite_spline(snap_start_position,
                                                snap_finish_position,
                                                snap_start_velocity,
                                                snap_finish_velocity,
                                                t);
        current_velocity = cubic_hermite_spline_deriv(snap_start_position,
                                                      snap_finish_position,
                                                      snap_start_velocity,
                                                      snap_finish_velocity,
                                                      t);

        SetScrollX(current_position.x);
        SetScrollY(current_position.y);

        // If snap animation ended, go back to idle state.
        if (snap_time >= snap_duration)
        {
            snap_state = CarouselSnapState::idle;
            current_position = snap_finish_position;
            current_velocity = {0, 0};
        }
        else
        {
            const ImGuiIO& io = GetIO();
            snap_time += io.DeltaTime;
        }
    }

    storage->SetInt(GetID("target_page_x"), target_page_x);
    storage->SetInt(GetID("target_page_y"), target_page_y);
    storage->Set<CarouselSnapMode>(GetID("snap_mode"), snap_mode);
    storage->Set<CarouselSnapState>(GetID("snap_state"), snap_state);
    storage->SetFloat(GetID("snap_time"), snap_time);

    storage->SetFloat(GetID("current_velocity.x"), current_velocity.x);
    storage->SetFloat(GetID("current_velocity.y"), current_velocity.y);

    storage->SetFloat(GetID("snap_start_velocity.x"), snap_start_velocity.x);
    storage->SetFloat(GetID("snap_start_velocity.y"), snap_start_velocity.y);

    storage->SetFloat(GetID("snap_start_position.x"), snap_start_position.x);
    storage->SetFloat(GetID("snap_start_position.y"), snap_start_position.y);

    EndChild();
    PopID();
}


int
ImGui::CarouselGetColumns()
{
    ImGuiStorage* storage = GetStateStorage();
    float step_size_x = storage->GetFloat(GetID("step_size.x"));
    float max_scroll_x = GetScrollMaxX();
    int columns = static_cast<int>(step_size_x > 0
                                   ? 1.0f + std::floor(max_scroll_x / step_size_x)
                                   : 1.0f);
    return columns;
}


int
ImGui::CarouselGetRows()
{
    ImGuiStorage* storage = GetStateStorage();
    float step_size_y = storage->GetFloat(GetID("step_size.y"));
    float max_scroll_y = GetScrollMaxY();
    int rows = static_cast<int>(step_size_y > 0
                                ? 1.0f + std::floor(max_scroll_y / step_size_y)
                                : 1.0f);
    return rows;
}


std::pair<int, int>
ImGui::CarouselGetColumnsRows()
{
    ImGuiStorage* storage = GetStateStorage();
    ImVec2 step_size{
        storage->GetFloat(GetID("step_size.x")),
        storage->GetFloat(GetID("step_size.y"))
    };
    ImVec2 max_scroll{
        GetScrollMaxX(),
        GetScrollMaxY()
    };
    int columns = static_cast<int>(step_size.x > 0
                                   ? 1.0f + std::floor(max_scroll.x / step_size.x)
                                   : 1.0f);
    int rows = static_cast<int>(step_size.y > 0
                                ? 1.0f + std::floor(max_scroll.y / step_size.y)
                                : 1.0f);
    return { columns, rows };
}


int
ImGui::CarouselGetPageX()
{
    ImGuiStorage* storage = GetStateStorage();
    float step_size_x = storage->GetFloat(GetID("step_size.x"));
    float position_x = GetScrollX();
    int page_x = std::lround(position_x / step_size_x);
    return page_x;
}


int
ImGui::CarouselGetPageY()
{
    ImGuiStorage* storage = GetStateStorage();
    float step_size_y = storage->GetFloat(GetID("step_size.y"));
    float position_y = GetScrollY();
    int page_y = std::lround(position_y / step_size_y);
    return page_y;
}


std::pair<int, int>
ImGui::CarouselGetPage()
{
    ImGuiStorage* storage = GetStateStorage();
    ImVec2 step_size{
        storage->GetFloat(GetID("step_size.x")),
        storage->GetFloat(GetID("step_size.y"))
    };
    ImVec2 position{
        GetScrollX(),
        GetScrollY()
    };
    int page_x = std::lround(position.x / step_size.x);
    int page_y = std::lround(position.y / step_size.y);
    return { page_x, page_y };
}


void
ImGui::CarouselJumpPageX(int page_x)
{
    ImGuiStorage* storage = GetStateStorage();
    int columns = CarouselGetColumns();
    page_x = clamp(page_x, 0, columns);
    storage->SetInt(GetID("target_page_x"), page_x);
    // Make sure the jump starts with the current velocity.
    float current_velocity_x = storage->GetFloat(GetID("current_velocity.x"));
    storage->SetFloat(GetID("snap_start_velocity.x"), current_velocity_x);
    storage->Set<CarouselSnapMode>(GetID("snap_mode"), CarouselSnapMode::jump);
    storage->Set<CarouselSnapState>(GetID("snap_state"), CarouselSnapState::started);
}


void
ImGui::CarouselJumpPageY(int page_y)
{
    ImGuiStorage* storage = GetStateStorage();
    int rows = CarouselGetRows();
    page_y = clamp(page_y, 0, rows);
    storage->SetInt(GetID("target_page_y"), page_y);
    // Make sure the jump starts with the current velocity.
    float current_velocity_y = storage->GetFloat(GetID("current_velocity.y"));
    storage->SetFloat(GetID("snap_start_velocity.y"), current_velocity_y);
    storage->Set<CarouselSnapMode>(GetID("snap_mode"), CarouselSnapMode::jump);
    storage->Set<CarouselSnapState>(GetID("snap_state"), CarouselSnapState::started);
}


void
ImGui::CarouselJumpPage(int page_x,
                        int page_y)
{
    ImGuiStorage* storage = GetStateStorage();
    auto extents = CarouselGetColumnsRows();
    page_x = clamp(page_x, 0, std::get<0>(extents));
    page_y = clamp(page_y, 0, std::get<1>(extents));
    storage->SetInt(GetID("target_page_x"), page_x);
    storage->SetInt(GetID("target_page_y"), page_y);
    // Make sure the jump starts with the current velocity.
    ImVec2 current_velocity{
        storage->GetFloat(GetID("current_velocity.x")),
        storage->GetFloat(GetID("current_velocity.y"))
    };
    storage->SetFloat(GetID("snap_start_velocity.x"), current_velocity.x);
    storage->SetFloat(GetID("snap_start_velocity.y"), current_velocity.y);
    storage->Set<CarouselSnapMode>(GetID("snap_mode"), CarouselSnapMode::jump);
    storage->Set<CarouselSnapState>(GetID("snap_state"), CarouselSnapState::started);
}


void
ImGui::CarouselJumpPageLeft()
{
    ImGuiStorage* storage = GetStateStorage();
    int target_page_x = storage->GetInt(GetID("target_page_x"), -1);
    if (target_page_x < 0)
        target_page_x = CarouselGetPageX();
    if (target_page_x > 0)
        CarouselJumpPageX(target_page_x - 1);
}


void
ImGui::CarouselJumpPageRight()
{
    ImGuiStorage* storage = GetStateStorage();
    int target_page_x = storage->GetInt(GetID("target_page_x"), -1);
    if (target_page_x < 0)
        target_page_x = CarouselGetPageX();
    int columns = CarouselGetColumns();
    if (target_page_x + 1 < columns)
        CarouselJumpPageX(target_page_x + 1);
}


void
ImGui::CarouselJumpPageUp()
{
    ImGuiStorage* storage = GetStateStorage();
    int target_page_y = storage->GetInt(GetID("target_page_y"), -1);
    if (target_page_y < 0)
        target_page_y = CarouselGetPageY();
    if (target_page_y > 0)
        CarouselJumpPageY(target_page_y - 1);
}


void
ImGui::CarouselJumpPageDown()
{
    ImGuiStorage* storage = GetStateStorage();
    int target_page_y = storage->GetInt(GetID("target_page_y"), -1);
    if (target_page_y < 0)
        target_page_y = CarouselGetPageY();
    int rows = CarouselGetRows();
    if (target_page_y + 1 < rows)
        CarouselJumpPageY(target_page_y + 1);
}


bool
ImGui::BeginCarousel(const std::string& str_id,
                     const ImVec2& page_size,
                     const ImGuiCarouselSpecs& specs)
{
    return BeginCarousel(str_id.data(), page_size, specs);
}
