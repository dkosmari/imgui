// -*- mode: c++ -*-
/*
 * Copyright (C) 2026  Daniel K. O. <dkosmari>
 * SPDX-License-Identifier: MIT
 */

#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <unordered_map>

#include <imgui.h>
#include <imgui_internal.h>

#include "imgui_stdlib.h"


#ifndef IMGUI_DISABLE

using namespace std::literals;


namespace ImGui {

    namespace {

        template<concepts::arithmetic T>
        extern
        const
        ImGuiDataType data_type_v;

        template<>
        constexpr
        ImGuiDataType data_type_v<std::int8_t> = ImGuiDataType_S8;

        template<>
        constexpr
        ImGuiDataType data_type_v<std::uint8_t> = ImGuiDataType_U8;

        template<>
        constexpr
        ImGuiDataType data_type_v<std::int16_t> = ImGuiDataType_S16;

        template<>
        constexpr
        ImGuiDataType data_type_v<std::uint16_t> = ImGuiDataType_U16;

        template<>
        constexpr
        ImGuiDataType data_type_v<std::int32_t> = ImGuiDataType_S32;

        template<>
        constexpr
        ImGuiDataType data_type_v<std::uint32_t> = ImGuiDataType_U32;

        template<>
        constexpr
        ImGuiDataType data_type_v<std::int64_t> = ImGuiDataType_S64;

        template<>
        constexpr
        ImGuiDataType data_type_v<std::uint64_t> = ImGuiDataType_U64;


        template<>
        constexpr
        ImGuiDataType data_type_v<char> =
            std::numeric_limits<char>::is_signed ? ImGuiDataType_S8 : ImGuiDataType_U8;

        template<>
        constexpr
        ImGuiDataType data_type_v<wchar_t> =
            std::numeric_limits<wchar_t>::is_signed
            ? (sizeof(wchar_t) == 2 ? ImGuiDataType_S16 : ImGuiDataType_S32)
            : (sizeof(wchar_t) == 2 ? ImGuiDataType_U16 : ImGuiDataType_U32);

        template<>
        constexpr
        ImGuiDataType data_type_v<char8_t> = ImGuiDataType_U8;

        template<>
        constexpr
        ImGuiDataType data_type_v<char16_t> = ImGuiDataType_U16;

        template<>
        constexpr
        ImGuiDataType data_type_v<char32_t> = ImGuiDataType_U32;

        template<>
        constexpr
        ImGuiDataType data_type_v<float> = ImGuiDataType_Float;

        template<>
        constexpr
        ImGuiDataType data_type_v<double> = ImGuiDataType_Double;

    } // namespace


    const ImGuiPayload*
    AcceptDragDropPayload(const std::string& type,
                          ImGuiDragDropFlags flags)
    {
        return AcceptDragDropPayload(type.data(), flags);
    }


    bool
    ArrowButton(const std::string& str_id,
                ImGuiDir dir,
                const ImVec2& size)
    {
        return ArrowButton(str_id.data(), dir, size);
    }


    bool
    Begin(const std::string& name,
          bool* p_open,
          ImGuiWindowFlags flags)
    {
        return Begin(name.data(), p_open, flags);
    }


    bool
    BeginChild(const std::string& str_id,
               const ImVec2& size,
               ImGuiChildFlags child_flags,
               ImGuiWindowFlags window_flags)
    {
        return BeginChild(str_id.data(),
                          size,
                          child_flags,
                          window_flags);
    }


    bool
    BeginCombo(const std::string& label,
               const std::string& preview,
               ImGuiComboFlags flags)
    {
        return BeginCombo(label.data(), preview.data(), flags);
    }


    bool
    BeginListBox(const std::string& label,
                 const ImVec2& size)
    {
        return BeginListBox(label.data(), size);
    }


    bool
    BeginMenu(const std::string& label,
              bool enabled)
    {
        return BeginMenu(label.data(), enabled);
    }


    bool
    BeginPopup(const std::string& id,
               ImGuiWindowFlags flags)
    {
        return BeginPopup(id.data(), flags);
    }


    bool
    BeginPopupContextItem(const std::string& str_id,
                          ImGuiPopupFlags popup_flags)
    {
        return BeginPopupContextItem(str_id.data(), popup_flags);
    }


    bool
    BeginPopupContextWindow(const std::string& str_id,
                            ImGuiPopupFlags popup_flags)
    {
        return BeginPopupContextWindow(str_id.data(), popup_flags);
    }


    bool
    BeginPopupContextVoid(const std::string& str_id,
                          ImGuiPopupFlags popup_flags)
    {
        return BeginPopupContextVoid(str_id.data(), popup_flags);
    }


    bool
    BeginPopupModal(const std::string& name,
                    bool* p_open,
                    ImGuiWindowFlags flags)
    {
        return BeginPopupModal(name.data(), p_open, flags);
    }


    bool
    BeginTabBar(const std::string& id,
                ImGuiTabBarFlags flags)
    {
        return BeginTabBar(id.data(), flags);
    }


    bool
    BeginTabItem(const std::string& label,
                 bool* p_open,
                 ImGuiTabItemFlags flags)
    {
        return BeginTabItem(label.data(), p_open, flags);
    }


    bool
    BeginTable(const std::string& id,
               int columns,
               ImGuiTableFlags flags,
               const ImVec2& outer_size,
               float inner_width)
    {
        return BeginTable(id.data(),
                          columns,
                          flags,
                          outer_size,
                          inner_width);
    }


    void
    BulletText(const std::string& text)
    {
        BulletText("%s", text.data());
    }


    bool
    Button(const std::string& label,
           const ImVec2& size)
    {
        return Button(label.data(), size);
    }


    ImVec2
    CalcTextSize(const std::string& text,
                 bool hide_text_after_double_hash,
                 float wrap_width)
    {
        return CalcTextSize(text.data(),
                            text.data() + text.size(),
                            hide_text_after_double_hash,
                            wrap_width);
    }


    bool
    Checkbox(const std::string& label,
             bool& value)
    {
        return Checkbox(label.data(), &value);
    }



    template<std::integral I>
    IMGUI_API
    bool
    CheckboxFlags(const std::string& label,
                  I& flags_var,
                  I flags_ref)
    {
        bool exact = (flags_var & flags_ref) == flags_ref;
        bool partial = (flags_var & flags_ref) != 0;
        bool pressed;
        if (!exact && !partial) {
            ImGuiContext& g = *GImGui;
            g.NextItemData.ItemFlags |= ImGuiItemFlags_MixedValue;
        }
        pressed = Checkbox(label, exact);
        if (pressed) {
            if (exact)
                flags_var |= flags_ref;
            else
                flags_var &= ~flags_ref;
        }
        return pressed;
    }

    /* --------------------------------------------- */
    /* Explicit instantiations for CheckboxFlags<>() */
    /* --------------------------------------------- */

#define INSTANTIATE(x)                          \
    template                                    \
    bool                                        \
    CheckboxFlags<x>(const std::string& label,  \
                     x& flags_var,              \
                     x flags_ref)

    INSTANTIATE(std::int8_t);
    INSTANTIATE(std::uint8_t);
    INSTANTIATE(std::int16_t);
    INSTANTIATE(std::uint16_t);
    INSTANTIATE(std::int32_t);
    INSTANTIATE(std::uint32_t);
    INSTANTIATE(std::int64_t);
    INSTANTIATE(std::uint64_t);
    INSTANTIATE(char);
    INSTANTIATE(wchar_t);
    INSTANTIATE(char8_t);
    INSTANTIATE(char16_t);
    INSTANTIATE(char32_t);

#undef INSTANTIATE


    bool
    CollapsingHeader(const std::string& label,
                     ImGuiTreeNodeFlags flags)
    {
        return CollapsingHeader(label.data(), flags);
    }


    bool
    CollapsingHeader(const std::string& label,
                     bool* p_visible,
                     ImGuiTreeNodeFlags flags)
    {
        return CollapsingHeader(label.data(), p_visible, flags);
    }


    bool
    ColorButton(const std::string& desc_id,
                const ImVec4& col,
                ImGuiColorEditFlags flags,
                const ImVec2& size)
    {
        return ColorButton(desc_id.data(), col, flags, size);
    }


    template<>
    bool
    ColorEdit<3>(const std::string& label,
              float col[3],
              ImGuiColorEditFlags flags)
    {
        return ColorEdit3(label.data(), col, flags);
    }


    template<>
    bool
    ColorEdit<4>(const std::string& label,
              float col[4],
              ImGuiColorEditFlags flags)
    {
        return ColorEdit4(label.data(), col, flags);
    }


    template<>
    bool
    ColorPicker<3>(const std::string& label,
                   float col[3],
                   ImGuiColorEditFlags flags)
    {
        return ColorPicker3(label.data(), col, flags);
    }


    template<>
    bool
    ColorPicker<4>(const std::string& label,
                   float col[4],
                   ImGuiColorEditFlags flags)
    {
        return ColorPicker4(label.data(), col, flags);
    }


#ifndef IMGUI_DISABLE_DEBUG_TOOLS
    void
    DebugLog(const std::string& msg)
    {
        DebugLog("%s", msg.data());
    }
#endif


    void
    DebugTextEncoding(const std::string& text)
    {
        DebugTextEncoding(text.data());
    }


    template<concepts::arithmetic T>
    bool
    Drag(const std::string& label,
         T& v,
         float speed,
         std::optional<T> v_min,
         std::optional<T> v_max,
         const std::string& format,
         ImGuiSliderFlags flags)
    {
        return DragScalar(label.data(),
                          data_type_v<T>,
                          &v,
                          speed,
                          v_min ? &*v_min : nullptr,
                          v_max ? &*v_max : nullptr,
                          format.empty() ? nullptr : format.data(),
                          flags);
    }

    /* ------------------------------------- */
    /* Explicit instantiations for Drag<T>() */
    /* ------------------------------------- */

#define INSTANTIATE(x)                          \
    template                                    \
    bool                                        \
    Drag<x>(const std::string&,                 \
            x&,                                 \
            float,                              \
            std::optional<x>,                   \
            std::optional<x>,                   \
            const std::string&,                 \
            ImGuiSliderFlags)

    INSTANTIATE(std::int8_t);
    INSTANTIATE(std::uint8_t);
    INSTANTIATE(std::int16_t);
    INSTANTIATE(std::uint16_t);
    INSTANTIATE(std::int32_t);
    INSTANTIATE(std::uint32_t);
    INSTANTIATE(std::int64_t);
    INSTANTIATE(std::uint64_t);
    INSTANTIATE(char);
    INSTANTIATE(wchar_t);
    INSTANTIATE(char8_t);
    INSTANTIATE(char16_t);
    INSTANTIATE(char32_t);
    INSTANTIATE(float);
    INSTANTIATE(double);

#undef INSTANTIATE


    template<concepts::arithmetic T,
             std::size_t N>
    requires (N >=2 && N <= 4)
    bool
    Drag(const std::string& label,
         T (&v)[N],
         float speed,
         std::optional<T> v_min,
         std::optional<T> v_max,
         const std::string& format,
         ImGuiSliderFlags flags)
    {
        return DragScalarN(label.data(),
                           data_type_v<T>,
                           &v,
                           N,
                           speed,
                           v_min ? &*v_min : nullptr,
                           v_max ? &*v_max : nullptr,
                           format.empty() ? nullptr : format.data(),
                           flags);
    }

    /* ---------------------------------------- */
    /* Explicit instantiations for Drag<T, N>() */
    /* ---------------------------------------- */

#define INSTANTIATE(x, n)                       \
    template                                    \
    bool                                        \
    Drag<x, n>(const std::string&,              \
               x(&)[n],                         \
               float,                           \
               std::optional<x>,                \
               std::optional<x>,                \
               const std::string&,              \
               ImGuiSliderFlags)

#define INSTANTIATE_N(x) \
    INSTANTIATE(x, 2);   \
    INSTANTIATE(x, 3);   \
    INSTANTIATE(x, 4)

    INSTANTIATE_N(std::int8_t);
    INSTANTIATE_N(std::uint8_t);
    INSTANTIATE_N(std::int16_t);
    INSTANTIATE_N(std::uint16_t);
    INSTANTIATE_N(std::int32_t);
    INSTANTIATE_N(std::uint32_t);
    INSTANTIATE_N(std::int64_t);
    INSTANTIATE_N(std::uint64_t);
    INSTANTIATE_N(char);
    INSTANTIATE_N(wchar_t);
    INSTANTIATE_N(char8_t);
    INSTANTIATE_N(char16_t);
    INSTANTIATE_N(char32_t);
    INSTANTIATE_N(float);
    INSTANTIATE_N(double);

#undef INSTANTIATE_N
#undef INSTANTIATE


    ImGuiID
    GetID(const std::string& str)
    {
        return GetID(str.data(), str.data() + str.size());
    }


    bool
    ImageButton(const std::string& str_id,
                ImTextureRef tex_ref,
                const ImVec2& image_size,
                const ImVec2& uv0,
                const ImVec2& uv1,
                const ImVec4& bg_col,
                const ImVec4& tint_col)
    {
        return ImageButton(str_id.data(),
                           tex_ref,
                           image_size,
                           uv0,
                           uv1,
                           bg_col,
                           tint_col);
    }


    template<concepts::arithmetic T>
    bool
    Input(const std::string& label,
          T& v,
          std::optional<T> step,
          std::optional<T> step_fast,
          const std::string& format,
          ImGuiInputTextFlags flags)
    {
        return InputScalar(label.data(),
                           data_type_v<T>,
                           &v,
                           step ? &*step : nullptr,
                           step_fast ? &*step_fast : nullptr,
                           format.empty() ? nullptr : format.data(),
                           flags);
    }

    /* -------------------------------------- */
    /* Explicit instantiations for Input<T>() */
    /* -------------------------------------- */

#define INSTANTIATE(x)                          \
    template                                    \
    bool                                        \
    Input<x>(const std::string&,                \
             x&,                                \
             std::optional<x>,                  \
             std::optional<x>,                  \
             const std::string&,                \
             ImGuiInputTextFlags)

    INSTANTIATE(std::int8_t);
    INSTANTIATE(std::uint8_t);
    INSTANTIATE(std::int16_t);
    INSTANTIATE(std::uint16_t);
    INSTANTIATE(std::int32_t);
    INSTANTIATE(std::uint32_t);
    INSTANTIATE(std::int64_t);
    INSTANTIATE(std::uint64_t);
    INSTANTIATE(char);
    INSTANTIATE(wchar_t);
    INSTANTIATE(char8_t);
    INSTANTIATE(char16_t);
    INSTANTIATE(char32_t);
    INSTANTIATE(float);
    INSTANTIATE(double);

#undef INSTANTIATE


    template<concepts::arithmetic T,
             std::size_t N>
    requires (N >= 2 && N <= 4)
    bool
    Input(const std::string& label,
          T (&v)[N],
          std::optional<T> step,
          std::optional<T> step_fast,
          const std::string& format,
          ImGuiInputTextFlags flags)
    {
        return InputScalarN(label.data(),
                            data_type_v<T>,
                            &v,
                            N,
                            step ? &*step : nullptr,
                            step_fast ? &*step_fast : nullptr,
                            format.empty() ? nullptr : format.data(),
                            flags);
    }

    /* ----------------------------------------- */
    /* Explicit instantiations for Input<T, N>() */
    /* ----------------------------------------- */

#define INSTANTIATE(x, n)                       \
    template                                    \
    bool                                        \
    Input<x, n>(const std::string&,             \
                x (&)[n],                       \
                std::optional<x>,               \
                std::optional<x>,               \
                const std::string&,             \
                ImGuiInputTextFlags)

#define INSTANTIATE_N(x)                        \
    INSTANTIATE(x, 2);                          \
    INSTANTIATE(x, 3);                          \
    INSTANTIATE(x, 4)

    INSTANTIATE_N(std::int8_t);
    INSTANTIATE_N(std::uint8_t);
    INSTANTIATE_N(std::int16_t);
    INSTANTIATE_N(std::uint16_t);
    INSTANTIATE_N(std::int32_t);
    INSTANTIATE_N(std::uint32_t);
    INSTANTIATE_N(std::int64_t);
    INSTANTIATE_N(std::uint64_t);
    INSTANTIATE_N(char);
    INSTANTIATE_N(wchar_t);
    INSTANTIATE_N(char8_t);
    INSTANTIATE_N(char16_t);
    INSTANTIATE_N(char32_t);
    INSTANTIATE_N(float);
    INSTANTIATE_N(double);

#undef INSTANTIATE_N
#undef INSTANTIATE


    namespace {

        namespace InputTextHelper {

            struct Data {
                std::string* str;
                const InputTextFunction* func;
            };

            int
            Callback(ImGuiInputTextCallbackData* data)
                noexcept
            {
                auto user_data = reinterpret_cast<Data*>(data->UserData);
                IM_ASSERT(user_data);
                if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
                    auto& str = *user_data->str;
                    IM_ASSERT(data->Buf == str.data());
                    str.resize(data->BufTextLen);
                    data->Buf = str.data();
                } else if (*user_data->func)
                    (*user_data->func)(data);
                return 0;
            }

        } // Namespace InputTextHelper

    } // namespace


    bool
    InputText(const std::string& label,
              std::string& value,
              ImGuiInputTextFlags flags,
              const InputTextFunction& func)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;
        InputTextHelper::Data data;
        data.str = &value;
        data.func = &func;
        return InputText(label.data(),
                         value.data(),
                         value.capacity() + 1,
                         flags,
                         &InputTextHelper::Callback,
                         &data);
    }


    bool
    InputText(const std::string& label,
              std::string* value,
              ImGuiInputTextFlags flags,
              const InputTextFunction& func)
    {
        return InputText(label, *value, flags, func);
    }


    bool
    InputTextMultiline(const std::string& label,
                       std::string& value,
                       const ImVec2& size,
                       ImGuiInputTextFlags flags,
                       const InputTextFunction& func)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;
        InputTextHelper::Data data;
        data.str = &value;
        data.func = &func;
        return InputTextMultiline(label.data(),
                                  value.data(),
                                  value.capacity() + 1,
                                  size,
                                  flags,
                                  &InputTextHelper::Callback,
                                  &data);
    }


    bool
    InputTextMultiline(const std::string& label,
                       std::string* value,
                       const ImVec2& size,
                       ImGuiInputTextFlags flags,
                       const InputTextFunction& func)
    {
        return InputTextMultiline(label, *value, size, flags, func);
    }


    bool
    InputTextWithHint(const std::string& label,
                      const std::string& hint,
                      std::string& value,
                      ImGuiInputTextFlags flags,
                      const InputTextFunction& func)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;
        InputTextHelper::Data data;
        data.str = &value;
        data.func = &func;
        return InputTextWithHint(label.data(),
                                 hint.data(),
                                 value.data(),
                                 value.capacity() + 1,
                                 flags,
                                 &InputTextHelper::Callback,
                                 &data);
    }


    bool
    InputTextWithHint(const std::string& label,
                      const std::string& hint,
                      std::string* value,
                      ImGuiInputTextFlags flags,
                      const InputTextFunction& func)
    {
        return InputTextWithHint(label, hint, *value, flags, func);
    }


    bool
    InvisibleButton(const std::string& str_id,
                    const ImVec2& size,
                    ImGuiButtonFlags flags)
    {
        return InvisibleButton(str_id.data(), size, flags);
    }


    bool
    IsPopupOpen(const std::string& str_id,
                ImGuiPopupFlags flags)
    {
        return IsPopupOpen(str_id.data(), flags);
    }


    void
    LabelText(const std::string& label,
              const std::string& text)
    {
        LabelText(label.data(), "%s", text.data());
    }


    bool
    ListBox(const std::string& label,
            std::size_t& current_item,
            const std::vector<std::string>& items,
            int height_in_items)
    {
        std::vector<const char*> c_items;
        c_items.reserve(items.size());
        for (const auto& item : items)
            c_items.push_back(item.data());
        int c_current_item = current_item;
        bool result = ListBox(label.data(),
                              &c_current_item,
                              c_items.data(),
                              c_items.size(),
                              height_in_items);
        current_item = c_current_item;
        return result;
    }


    namespace {

        namespace ListBoxHelper {

            struct Context {
                const ListBoxGetterFunction* getter = nullptr;
                std::string last_result = {};
            };

            const char*
            Getter(void* user_data,
                   int idx_)
            {
                auto ctx = reinterpret_cast<Context*>(user_data);
                std::size_t idx = idx_;
                const char* result = nullptr;
                try {
                    if (*ctx->getter) {
                        ctx->last_result = (*ctx->getter)(idx);
                        result = ctx->last_result.data();
                    }
                }
                catch (std::exception& e) {
                    IMGUI_DEBUG_LOG("leaked an exception: %s", e.what());
                }
                return result;
            }

        } // namespace ListBoxHelper

    } // namespace


    bool
    ListBox(const std::string& label,
            int& current_item,
            const ListBoxGetterFunction& getter,
            std::size_t items_count,
            int height_in_items)
    {
        int c_current_item = current_item;
        ListBoxHelper::Context ctx{&getter};
        bool result = ListBox(label.data(),
                              &c_current_item,
                              ListBoxHelper::Getter,
                              &ctx,
                              items_count,
                              height_in_items);
        current_item = c_current_item;
        return result;
    }

    void
    LoadIniSettingsFromDisk(const std::filesystem::path& ini_filename)
    {
        LoadIniSettingsFromDisk(ini_filename.c_str());
    }


    void
    LoadIniSettingsFromMemory(const std::string& ini)
    {
        LoadIniSettingsFromMemory(ini.data(), ini.size());
    }


    void
    LogText(const std::string& text)
    {
        LogText("%s", text.data());
    }


    void
    LogToFile(int auto_open_depth,
              const std::filesystem::path& filename)
    {
        LogToFile(auto_open_depth, filename.c_str());
    }


    bool
    MenuItem(const std::string& label,
             const std::string& shortcut,
             bool selected,
             bool enabled)
    {
        return MenuItem(label.data(),
                        shortcut.empty() ? nullptr : shortcut.data(),
                        selected,
                        enabled);
    }


    bool
    MenuItem(const std::string& label,
             const std::string& shortcut,
             bool* p_selected,
             bool enabled)
    {
        return MenuItem(label.data(),
                        shortcut.empty() ? nullptr : shortcut.data(),
                        p_selected,
                        enabled);
    }


    void
    OpenPopup(const std::string& str_id,
              ImGuiPopupFlags popup_flags)
    {
        OpenPopup(str_id.data(), popup_flags);
    }


    void
    OpenPopupOnItemClick(const std::string& str_id,
                         ImGuiPopupFlags popup_flags)
    {
        OpenPopupOnItemClick(str_id.empty() ? nullptr : str_id.data(), popup_flags);
    }


    void
    ProgressBar(float fraction,
                const ImVec2& size_arg,
                const std::string& overlay)
    {
        ProgressBar(fraction, size_arg, overlay.data());
    }


    void
    PushID(const std::string& id)
    {
        PushID(id.data(), id.data() + id.size());
    }


    bool
    RadioButton(const std::string& label,
                bool active)
    {
        return RadioButton(label.data(), active);
    }


    void
    SaveIniSettingsToDisk(const std::filesystem::path& ini_filename)
    {
        SaveIniSettingsToDisk(ini_filename.c_str());
    }


    std::string
    SaveIniSettingsToString()
    {
        std::size_t size;
        const char* str = SaveIniSettingsToMemory(&size);
        return std::string(str, size);
    }


    bool
    Selectable(const std::string& label,
               bool selected,
               ImGuiSelectableFlags flags,
               const ImVec2& size)
    {
        return Selectable(label.data(),
                          selected,
                          flags,
                          size);
    }


    bool
    Selectable(const std::string& label,
               bool* p_selected,
               ImGuiSelectableFlags flags,
               const ImVec2& size)
    {
        return Selectable(label.data(), p_selected, flags, size);
    }


    void
    SeparatorText(const std::string& label)
    {
        SeparatorText(label.data());
    }


    void
    SetClipboardText(const std::string& text)
    {
        SetClipboardText(text.data());
    }


    bool
    SetDragDropPayload(const std::string& type,
                       const void* data,
                       std::size_t sz,
                       ImGuiCond cond)
    {
        return SetDragDropPayload(type.data(),
                                  data,
                                  sz,
                                  cond);
    }


    void
    SetItemTooltip(const std::string& text)
    {
        SetItemTooltip("%s", text.data());
    }


    void
    SetTabItemClosed(const std::string& tab_or_docked_window_label)
    {
        SetTabItemClosed(tab_or_docked_window_label.data());
    }


    void
    SetTooltip(const std::string& text)
    {
        SetTooltip("%s", text.data());
    }


    template<concepts::arithmetic T>
    bool
    Slider(const std::string& label,
           T& v,
           T v_min,
           T v_max,
           const std::string& format,
           ImGuiSliderFlags flags)
    {
        return SliderScalar(label.data(),
                            data_type_v<T>,
                            &v,
                            &v_min,
                            &v_max,
                            format.empty() ? nullptr : format.data(),
                            flags);
    }

    /* --------------------------------------- */
    /* Explicit instantiations for Slider<T>() */
    /* --------------------------------------- */

#define INSTANTIATE(x)                          \
    template                                    \
    bool                                        \
    Slider<x>(const std::string&,               \
              x&,                               \
              x,                                \
              x,                                \
              const std::string&,               \
              ImGuiSliderFlags)

    INSTANTIATE(std::int8_t);
    INSTANTIATE(std::uint8_t);
    INSTANTIATE(std::int16_t);
    INSTANTIATE(std::uint16_t);
    INSTANTIATE(std::int32_t);
    INSTANTIATE(std::uint32_t);
    INSTANTIATE(std::int64_t);
    INSTANTIATE(std::uint64_t);
    INSTANTIATE(char);
    INSTANTIATE(wchar_t);
    INSTANTIATE(char8_t);
    INSTANTIATE(char16_t);
    INSTANTIATE(char32_t);
    INSTANTIATE(float);
    INSTANTIATE(double);

#undef INSTANTIATE


    template<concepts::arithmetic T,
             std::size_t N>
    requires (N >= 2 && N <= 4)
    bool
    Slider(const std::string& label,
           T (&v)[N],
           T v_min,
           T v_max,
           const std::string& format,
           ImGuiSliderFlags flags)
    {
        return SliderScalarN(label.data(),
                             data_type_v<T>,
                             &v,
                             N,
                             &v_min,
                             &v_max,
                             format.empty() ? nullptr : format.data(),
                             flags);
    }

    /* ------------------------------------------ */
    /* Explicit instantiations for Slider<T, N>() */
    /* ------------------------------------------ */

#define INSTANTIATE(x, n)                       \
    template                                    \
    bool                                        \
    Slider<x, n>(const std::string&,            \
                 x (&)[n],                      \
                 x,                             \
                 x,                             \
                 const std::string&,            \
                 ImGuiSliderFlags)

#define INSTANTIATE_N(x)                        \
    INSTANTIATE(x, 2);                          \
    INSTANTIATE(x, 3);                          \
    INSTANTIATE(x, 4)

    INSTANTIATE_N(std::int8_t);
    INSTANTIATE_N(std::uint8_t);
    INSTANTIATE_N(std::int16_t);
    INSTANTIATE_N(std::uint16_t);
    INSTANTIATE_N(std::int32_t);
    INSTANTIATE_N(std::uint32_t);
    INSTANTIATE_N(std::int64_t);
    INSTANTIATE_N(std::uint64_t);
    INSTANTIATE_N(char);
    INSTANTIATE_N(wchar_t);
    INSTANTIATE_N(char8_t);
    INSTANTIATE_N(char16_t);
    INSTANTIATE_N(char32_t);
    INSTANTIATE_N(float);
    INSTANTIATE_N(double);

#undef INSTANTIATE_N
#undef INSTANTIATE


    bool
    SmallButton(const std::string& label)
    {
        return SmallButton(label.data());
    }


    bool
    TabItemButton(const std::string& label,
                  ImGuiTabItemFlags flags)
    {
        return TabItemButton(label.data(), flags);
    }


    void
    TableSetupColumn(const std::string& label,
                     ImGuiTableColumnFlags flags,
                     float init_width_or_weight,
                     ImGuiID user_id)
    {
        TableSetupColumn(label.data(), flags, init_width_or_weight, user_id);
    }


    void
    Text(const std::string& text)
    {
        Text("%s", text.data());
    }


    void
    TextAligned(float align,
                float width,
                const std::string& text)
    {
        TextAligned(align, width, "%s", text.data());
    }


    void
    TextColored(const ImVec4& col, const std::string& text)
    {
        TextColored(col, "%s", text.data());
    }


    void
    TextDisabled(const std::string& text)
    {
        TextDisabled("%s", text.data());
    }


    bool
    TextLink(const std::string& label)
    {
        return TextLink(label.data());
    }


    bool
    TextLinkOpenURL(const std::string& label)
    {
        return TextLinkOpenURL(label.data());
    }


    bool
    TextLinkOpenURL(const std::string& label,
                    const std::string& url)
    {
        return TextLinkOpenURL(label.data(),
                               url.data());
    }


    void
    TextUnformatted(std::string_view text)
    {
        TextUnformatted(text.data(), text.data() + text.size());
    }


    void
    TextWrapped(const std::string& text)
    {
        TextWrapped("%s", text.data());
    }


    std::string
    to_string(ImGuiCol_ color)
    {
        return GetStyleColorName(color);
    }


    bool
    TreeNode(const std::string& label)
    {
        return TreeNode(label.data());
    }


    bool
    TreeNode(const std::string& str_id,
             const std::string& label)
    {
        return TreeNode(str_id.data(), "%s", label.data());
    }


    bool
    TreeNode(const void* ptr_id,
             const std::string& label)
    {
        return TreeNode(ptr_id, "%s", label.data());
    }


    bool
    TreeNodeEx(const std::string& label,
               ImGuiTreeNodeFlags flags)
    {
        return TreeNodeEx(label.data(), flags);
    }


    bool
    TreeNodeEx(const std::string& str_id,
               ImGuiTreeNodeFlags flags,
               const std::string& label)
    {
        return TreeNodeEx(str_id.data(), flags, "%s", label.data());
    }


    bool
    TreeNodeEx(const void* ptr_id,
               ImGuiTreeNodeFlags flags,
               const std::string& label)
    {
        return TreeNodeEx(ptr_id, flags, "%s", label.data());
    }


    void
    TreePush(const std::string& str_id)
    {
        return TreePush(str_id.data());
    }


    template<concepts::arithmetic T>
    void
    Value(const std::string& prefix,
          T value,
          const std::string& format)
    {
        if (format.empty())
            Text("%s: %s",
                 prefix.data(),
                 std::to_string(value).data());
        else
            Text(("%s: " + format).data(),
                 prefix.data(),
                 value);
    }

    /* -------------------------------------- */
    /* Explicit instantiations for Value<T>() */
    /* -------------------------------------- */

#define INSTANTIATE(x)                          \
    template                                    \
    void                                        \
    Value<x>(const std::string& prefix,         \
             x,                                 \
             const std::string& format)

    INSTANTIATE(std::int8_t);
    INSTANTIATE(std::uint8_t);
    INSTANTIATE(std::int16_t);
    INSTANTIATE(std::uint16_t);
    INSTANTIATE(std::int32_t);
    INSTANTIATE(std::uint32_t);
    INSTANTIATE(std::int64_t);
    INSTANTIATE(std::uint64_t);
    INSTANTIATE(char);
    INSTANTIATE(wchar_t);
    INSTANTIATE(char8_t);
    INSTANTIATE(char16_t);
    INSTANTIATE(char32_t);
    INSTANTIATE(float);
    INSTANTIATE(double);

#undef INSTANTIATE


    void
    Value(const std::string& prefix,
          bool value)
    {
        Value(prefix.data(), value);
    }


    void
    Value(const std::string& prefix,
          const std::string& value)
    {
        Text("%s: %s", prefix.data(), value.data());
    }


    template<concepts::arithmetic T>
    bool
    VSlider(const std::string& label,
            const ImVec2& size,
            T& variable,
            T min,
            T max,
            const std::string& format,
            ImGuiSliderFlags flags)
    {
        return VSliderScalar(label.data(),
                             size,
                             data_type_v<T>,
                             &variable,
                             &min,
                             &max,
                             format.empty() ? nullptr : format.data(),
                             flags);
    }

    /* ---------------------------------------- */
    /* Explicit instantiations for VSlider<T>() */
    /* ---------------------------------------- */

#define INSTANTIATE(x)                          \
    template                                    \
    bool                                        \
    VSlider<x>(const std::string& label,        \
               const ImVec2& size,              \
               x& variable,                     \
               x min,                           \
               x max,                           \
               const std::string& format,       \
               ImGuiSliderFlags flags)

    INSTANTIATE(std::int8_t);
    INSTANTIATE(std::uint8_t);
    INSTANTIATE(std::int16_t);
    INSTANTIATE(std::uint16_t);
    INSTANTIATE(std::int32_t);
    INSTANTIATE(std::uint32_t);
    INSTANTIATE(std::int64_t);
    INSTANTIATE(std::uint64_t);
    INSTANTIATE(char);
    INSTANTIATE(wchar_t);
    INSTANTIATE(char8_t);
    INSTANTIATE(char16_t);
    INSTANTIATE(char32_t);
    INSTANTIATE(float);
    INSTANTIATE(double);

#undef INSTANTIATE

} // namespace ImGui

#endif // IMGUI_DISABLE
