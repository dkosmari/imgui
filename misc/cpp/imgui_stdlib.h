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
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

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

        template<typename T>
        concept enumeration = std::is_enum<T>::value;

    } // namespace concepts


    using InputTextFunction = std::function<void (ImGuiInputTextCallbackData*)>;


    using ListBoxGetterFunction = std::function<std::string (std::size_t idx)>;


    IMGUI_API
    const ImGuiPayload*
    AcceptDragDropPayload(const std::string& type,
                          ImGuiDragDropFlags flags = 0);


    IMGUI_API
    bool
    ArrowButton(const std::string& str_id,
                ImGuiDir dir,
                const ImVec2& size = {0, 0});


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
    BeginListBox(const std::string& label,
                 const ImVec2& size = {0, 0});


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
    void
    BulletText(const std::string& text);


    IMGUI_API
    bool
    Button(const std::string& label,
           const ImVec2& size = {0, 0});


    IMGUI_API
    ImVec2
    CalcTextSize(const std::string& text,
                 bool hide_text_after_double_hash = false,
                 float wrap_width = -1.0f);


    IMGUI_API
    bool
    Checkbox(const std::string& label,
             bool& value);


    /// Instantiated for all integral types.
    template<std::integral I>
    IMGUI_API
    bool
    CheckboxFlags(const std::string& label,
                  I& flags_var,
                  I flags_ref);

    /// Enums are converted to their underlying type.
    template<concepts::enumeration E>
    IMGUI_API
    bool
    CheckboxFlags(const std::string& label,
                  E& flags_var,
                  E flags_ref)
    {
        return CheckboxFlags(label,
                             static_cast<std::underlying_type_t<E>&>(flags_var),
                             static_cast<std::underlying_type_t<E>>(flags_ref));
    }


    IMGUI_API
    bool
    CollapsingHeader(const std::string& label,
                     ImGuiTreeNodeFlags flags = 0);

    IMGUI_API
    bool
    CollapsingHeader(const std::string& label,
                     bool* p_visible,
                     ImGuiTreeNodeFlags flags = 0);


    IMGUI_API
    bool
    ColorButton(const std::string& desc_id,
                const ImVec4& col,
                ImGuiColorEditFlags flags = 0,
                const ImVec2& size = {0, 0});


    template<std::size_t N>
    IMGUI_API
    bool
    ColorEdit(const std::string& label,
              float col[N],
              ImGuiColorEditFlags flags = 0);


    template<std::size_t N>
    IMGUI_API
    bool
    ColorPicker(const std::string& label,
                float col[N],
                ImGuiColorEditFlags flags = 0);


#ifndef IMGUI_DISABLE_DEBUG_TOOLS
    IMGUI_API
    void
    DebugLog(const std::string& msg);
#endif


    IMGUI_API
    void
    DebugTextEncoding(const std::string& text);


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


    IMGUI_API
    ImGuiID
    GetID(const std::string& str);


    IMGUI_API
    bool
    ImageButton(const std::string& str_id,
                ImTextureRef tex_ref,
                const ImVec2& image_size,
                const ImVec2& uv0 = {0, 0},
                const ImVec2& uv1 = {1, 1},
                const ImVec4& bg_col = {0, 0, 0, 0},
                const ImVec4& tint_col = {1, 1, 1, 1});


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
              const InputTextFunction& func = {});

    IMGUI_API
    bool
    InputText(const std::string& label,
              std::string* value,
              ImGuiInputTextFlags flags = 0,
              const InputTextFunction& func = {});


    IMGUI_API
    bool
    InputTextMultiline(const std::string& label,
                       std::string& value,
                       const ImVec2& size = ImVec2(0, 0),
                       ImGuiInputTextFlags flags = 0,
                       const InputTextFunction& func = {});

    IMGUI_API
    bool
    InputTextMultiline(const std::string& label,
                       std::string* value,
                       const ImVec2& size = ImVec2(0, 0),
                       ImGuiInputTextFlags flags = 0,
                       const InputTextFunction& func = {});


    IMGUI_API
    bool
    InputTextWithHint(const std::string& label,
                      const std::string& hint,
                      std::string& value,
                      ImGuiInputTextFlags flags = 0,
                      const InputTextFunction& func = {});

    IMGUI_API
    bool
    InputTextWithHint(const std::string& label,
                      const std::string& hint,
                      std::string* value,
                      ImGuiInputTextFlags flags = 0,
                      const InputTextFunction& func = {});


    IMGUI_API
    bool
    InvisibleButton(const std::string& str_id,
                    const ImVec2& size,
                    ImGuiButtonFlags flags = 0);


    IMGUI_API
    bool
    IsPopupOpen(const std::string& str_id,
                ImGuiPopupFlags flags = 0);


    IMGUI_API
    void
    LabelText(const std::string& label,
              const std::string& text);


    IMGUI_API
    bool
    ListBox(const std::string& label,
            std::size_t& current_item,
            const std::vector<std::string>& items,
            int height_in_items = -1);


    IMGUI_API
    bool
    ListBox(const std::string& label,
            int& current_item,
            const ListBoxGetterFunction& getter,
            std::size_t items_count,
            int height_in_items = -1);


    IMGUI_API
    void
    LoadIniSettingsFromDisk(const std::filesystem::path& ini_filename);


    IMGUI_API
    void
    LoadIniSettingsFromMemory(const std::string& ini);

    template<typename T,
             std::size_t Extent>
    IMGUI_API
    void
    LoadIniSettingsFromMemory(std::span<T, Extent> ini)
    {
        LoadIniSettingsFromMemory(reinterpret_cast<const char*>(ini.data()), ini.size_bytes());
    }


    IMGUI_API
    void
    LogText(const std::string& text);


    IMGUI_API
    void
    LogToFile(int auto_open_depth,
              const std::filesystem::path& filename);


    IMGUI_API
    bool
    MenuItem(const std::string& label,
             const std::string& shortcut = "",
             bool selected = false,
             bool enabled = true);

    IMGUI_API
    bool
    MenuItem(const std::string& label,
             const std::string& shortcut,
             bool* p_selected,
             bool enabled = true);


    IMGUI_API
    void
    OpenPopup(const std::string& str_id,
              ImGuiPopupFlags popup_flags = 0);


    IMGUI_API
    void
    OpenPopupOnItemClick(const std::string& str_id = "",
                         ImGuiPopupFlags popup_flags = 0);


    // TODO: PlotLines()


    // TODO: PlotHistorgram()


    IMGUI_API
    void
    ProgressBar(float fraction,
                const ImVec2& size_arg,
                const std::string& overlay);


    IMGUI_API
    void
    PushID(const std::string& id);


    IMGUI_API
    bool
    RadioButton(const std::string& label,
                bool active);


    template<typename T>
    IMGUI_API
    inline
    bool
    RadioButton(const std::string& label,
                T& variable,
                T reference)
    {
        bool result = RadioButton(label, variable == reference);
        if (result)
            variable = reference;
        return result;
    }


    IMGUI_API
    void
    SaveIniSettingsToDisk(const std::filesystem::path& ini_filename);


    IMGUI_API
    std::string
    SaveIniSettingsToString();


    IMGUI_API
    bool
    Selectable(const std::string& label,
               bool selected = false,
               ImGuiSelectableFlags flags = 0,
               const ImVec2& size = {0, 0});

    IMGUI_API
    bool
    Selectable(const std::string& label,
               bool* p_selected,
               ImGuiSelectableFlags flags = 0,
               const ImVec2& size = {0, 0});


    IMGUI_API
    void
    SeparatorText(const std::string& label);


    IMGUI_API
    void
    SetClipboardText(const std::string& text);


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
    SmallButton(const std::string& label);


    IMGUI_API
    bool
    TabItemButton(const std::string& label,
                  ImGuiTabItemFlags flags = 0);


    IMGUI_API
    void
    TableSetupColumn(const std::string& label,
                     ImGuiTableColumnFlags flags = 0,
                     float init_width_or_weight = 0.0f,
                     ImGuiID user_id = 0);


    IMGUI_API
    void
    Text(const std::string& text);


    IMGUI_API
    void
    TextAligned(float align,
                float width,
                const std::string& text);


    IMGUI_API
    void
    TextColored(const ImVec4& col, const std::string& text);


    IMGUI_API
    void
    TextDisabled(const std::string& text);


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


    IMGUI_API
    std::string
    to_string(ImGuiCol_ color);


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


    IMGUI_API
    bool
    TreeNode(const std::string& label);

    IMGUI_API
    bool
    TreeNode(const std::string& str_id,
             const std::string& label);

    IMGUI_API
    bool
    TreeNode(const void* ptr_id,
             const std::string& label);


    IMGUI_API
    bool
    TreeNodeEx(const std::string& label,
               ImGuiTreeNodeFlags flags = 0);

    IMGUI_API
    bool
    TreeNodeEx(const std::string& str_id,
               ImGuiTreeNodeFlags flags,
               const std::string& label);

    IMGUI_API
    bool
    TreeNodeEx(const void* ptr_id,
               ImGuiTreeNodeFlags flags,
               const std::string& label);


    IMGUI_API
    void
    TreePush(const std::string& str_id);


    template<concepts::arithmetic T>
    IMGUI_API
    void
    Value(const std::string& prefix,
          T value,
          const std::string& format = "");

    IMGUI_API
    void
    Value(const std::string& prefix,
          bool value);

    IMGUI_API
    void
    Value(const std::string& prefix,
          const std::string& value);


    template<concepts::arithmetic T>
    IMGUI_API
    bool
    VSlider(const std::string& label,
            const ImVec2& size,
            T& variable,
            T min,
            T max,
            const std::string& format = "",
            ImGuiSliderFlags flags = 0);

} // namespace ImGui

#endif // IMGUI_DISABLE
