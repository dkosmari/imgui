// -*- mode: c++ -*-
/*
 * Overloads for C++ standard library types.
 *
 * Copyright (C) 2026  Daniel K. O. <dkosmari>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifndef IMGUI_DISABLE

#include <concepts>
#include <cstdarg>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>

#include <imgui.h>


namespace ImGui {

    namespace concepts {

        template<typename T>
        concept arithmetic = std::is_arithmetic_v<T>;


        template<typename T>
        concept vec2 = std::convertible_to<decltype(T::x), float> &&
                       std::convertible_to<decltype(T::y), float>;

        template<typename T>
        concept vec4 = std::convertible_to<decltype(T::x), float> &&
                       std::convertible_to<decltype(T::y), float> &&
                       std::convertible_to<decltype(T::z), float> &&
                       std::convertible_to<decltype(T::w), float>;

        template<typename T>
        concept rgba = std::convertible_to<decltype(T::r), float> &&
                       std::convertible_to<decltype(T::g), float> &&
                       std::convertible_to<decltype(T::b), float> &&
                       std::convertible_to<decltype(T::a), float>;

    } // namespace concepts


    using InputTextFunction = std::move_only_function<void (ImGuiInputTextCallbackData*)>;


    IMGUI_API
    bool
    Begin(const std::string& name,
          bool* p_open = nullptr,
          ImGuiWindowFlags flags = 0);


    IMGUI_API
    bool
    BeginChild(const std::string& str_id,
               const ImVec2& size = {0, 0},
               ImGuiChildFlags child_flags = 0,
               ImGuiWindowFlags window_flags = 0);


    IMGUI_API
    bool
    BeginCombo(const std::string& label,
               const std::string& preview,
               ImGuiComboFlags flags = 0);


    IMGUI_API
    bool
    BeginMenu(const std::string& label,
              bool enabled = true);


    IMGUI_API
    bool
    BeginPopup(const std::string& id,
               ImGuiWindowFlags flags = 0);


    IMGUI_API
    bool
    BeginPopupContextItem(const std::string& str_id,
                          ImGuiPopupFlags popup_flags = 0);


    IMGUI_API
    bool
    BeginPopupContextWindow(const std::string& str_id,
                            ImGuiPopupFlags popup_flags = 0);


    IMGUI_API
    bool
    BeginPopupContextVoid(const std::string& str_id,
                          ImGuiPopupFlags popup_flags = 0);


    IMGUI_API
    bool
    BeginPopupModal(const std::string& name,
                    bool* p_open = nullptr,
                    ImGuiWindowFlags flags = 0);


    IMGUI_API
    bool
    BeginTabBar(const std::string& id,
                ImGuiTabBarFlags flags = 0);


    IMGUI_API
    bool
    BeginTabItem(const std::string& label,
                 bool* p_open = nullptr,
                 ImGuiTabItemFlags flags = 0);


    IMGUI_API
    bool
    BeginTable(const std::string& id,
               int columns,
               ImGuiTableFlags flags = 0,
               const ImVec2& outer_size = {},
               float inner_width = 0);


    IMGUI_API
    bool
    Button(const std::string& label,
           const ImVec2& size = {0, 0});


    IMGUI_API
    ImVec2
    CalcTextSize(const std::string& text,
                 bool hide_text_after_double_hash = false,
                 float wrap_width = -1.0f);


    /// Wrapper for DragScalar().
    template<concepts::arithmetic T>
    IMGUI_API
    bool
    Drag(const std::string& label,
         T& v,
         float speed = 1.0f,
         std::optional<T> v_min = {},
         std::optional<T> v_max = {},
         const std::string& format = "",
         ImGuiSliderFlags flags = 0);


    /// Wrapper for DragScalarN().
    template<concepts::arithmetic T,
             std::size_t N>
    requires (N >= 2 && N <= 4)
    IMGUI_API
    bool
    Drag(const std::string& label,
         T (&v)[N],
         float speed = 1.0f,
         std::optional<T> v_min = {},
         std::optional<T> v_max = {},
         const std::string& format = "",
         ImGuiSliderFlags flags = 0);


    /// Wrapper for InputScalar().
    template<concepts::arithmetic T>
    IMGUI_API
    bool
    Input(const std::string& label,
          T& v,
          std::optional<T> step = {},
          std::optional<T> step_fast = {},
          const std::string& format = "",
          ImGuiInputTextFlags flags = 0);

    /// Wrapper for InputScalarN().
    template<concepts::arithmetic T,
             std::size_t N>
    requires (N >= 2 && N <= 4)
    IMGUI_API
    bool
    Input(const std::string& label,
          T (&v)[N],
          std::optional<T> step = {},
          std::optional<T> step_fast = {},
          const std::string& format = "",
          ImGuiInputTextFlags flags = 0);


    IMGUI_API
    bool
    InputText(const std::string& label,
              std::string& value,
              ImGuiInputTextFlags flags = 0,
              InputTextFunction func = {});

    IMGUI_API
    bool
    InputText(const std::string& label,
              std::string* value,
              ImGuiInputTextFlags flags = 0,
              InputTextFunction func = {});


    IMGUI_API
    bool
    InputTextMultiline(const std::string& label,
                       std::string& value,
                       const ImVec2& size = ImVec2(0, 0),
                       ImGuiInputTextFlags flags = 0,
                       InputTextFunction func = {});

    IMGUI_API
    bool
    InputTextMultiline(const std::string& label,
                       std::string* value,
                       const ImVec2& size = ImVec2(0, 0),
                       ImGuiInputTextFlags flags = 0,
                       InputTextFunction func = {});


    IMGUI_API
    bool
    InputTextWithHint(const std::string& label,
                      const std::string& hint,
                      std::string& value,
                      ImGuiInputTextFlags flags = 0,
                      InputTextFunction func = {});

    IMGUI_API
    bool
    InputTextWithHint(const std::string& label,
                      const std::string& hint,
                      std::string* value,
                      ImGuiInputTextFlags flags = 0,
                      InputTextFunction func = {});


    IMGUI_API
    bool
    IsPopupOpen(const std::string& str_id,
                ImGuiPopupFlags flags = 0);


    // TODO: ListBox()


    IMGUI_API
    void
    OpenPopup(const std::string& str_id,
              ImGuiPopupFlags popup_flags = 0);


    // TODO: OpenPopupOnItemClick()


    IMGUI_API
    void
    PushID(const std::string& id);


    IMGUI_API
    void
    PushID(std::string_view id);


    IMGUI_API
    bool
    Selectable(const std::string& label,
               bool selected = false,
               ImGuiSelectableFlags flags = 0,
               const ImVec2& size = {0, 0});


    IMGUI_API
    void
    SeparatorText(const std::string& label);


    IMGUI_API
    bool
    SetDragDropPayload(const std::string& type,
                       const void* data,
                       std::size_t sz,
                       ImGuiCond cond = 0);

    template<typename T,
             std::size_t E>
    bool
    SetDragDropPayload(const std::string& type,
                       std::span<T, E> data,
                       ImGuiCond cond = 0)
    {
        return SetDragDropPayload(type,
                                  data.data(),
                                  data.size_bytes(),
                                  cond);
    }



    IMGUI_API
    void
    SetItemTooltip(const std::string& text);


    IMGUI_API
    void
    SetTabItemClosed(const std::string& tab_or_docked_window_label);


    IMGUI_API
    void
    SetTooltip(const std::string& text);


    /// Wrapper for SliderScalar()
    template<concepts::arithmetic T>
    IMGUI_API
    bool
    Slider(const std::string& label,
           T& v,
           T v_min,
           T v_max,
           const std::string& format = "",
           ImGuiSliderFlags flags = 0);

    /// Wrapper for SliderScalarN()
    template<concepts::arithmetic T,
             std::size_t N>
    requires (N >= 2 && N <= 4)
    IMGUI_API
    bool
    Slider(const std::string& label,
           T (&v)[N],
           T v_min,
           T v_max,
           const std::string& format = "",
           ImGuiSliderFlags flags = 0);


    IMGUI_API
    bool
    TabItemButton(const std::string& label,
                  ImGuiTabItemFlags flags = 0);


    IMGUI_API
    void
    Text(const std::string& text);


    IMGUI_API
    bool
    TextLink(const std::string& label);


    IMGUI_API
    bool
    TextLinkOpenURL(const std::string& label);

    IMGUI_API
    bool
    TextLinkOpenURL(const std::string& label,
                    const std::string& url);


    IMGUI_API
    void
    TextUnformatted(std::string_view text);


    IMGUI_API
    void
    TextWrapped(const std::string& text);


    /// Convert anything with .x, .y to ImVec2
    template<concepts::vec2 V>
    IMGUI_API
    ImVec2
    ToVec2(const V& v)
    {
        return ImVec2(v.x, v.y);
    }


    /// Convert anything with .r, .g, .b, .a to ImVec4
    template<concepts::rgba T>
    IMGUI_API
    ImVec4
    ToVec4(const T& c)
    {
        return ImVec4(c.r, c.g, c.b, c.a);
    }


    /// Convert anything with .x, .y, .z, .w to ImVec4
    template<concepts::vec4 T>
    IMGUI_API
    ImVec4
    ToVec4(const T& v)
    {
        return ImVec4(v.x, v.y, v.z, v.w);
    }


    template<typename T>
    IMGUI_API
    void
    Value(const std::string& prefix,
          T value);


    IMGUI_API
    void
    Value(const std::string& prefix,
          const std::string& value);

} // namespace ImGui

#endif // IMGUI_DISABLE
