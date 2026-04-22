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
        ImGuiDataType data_type_v<float> = ImGuiDataType_Float;
        template<>
        constexpr
        ImGuiDataType data_type_v<double> = ImGuiDataType_Double;



        template<typename T>
        constexpr
        const char* format_string_v;

        template<>
        constexpr
        const char* format_string_v<char> = "c";

        template<>
        constexpr
        const char* format_string_v<std::int8_t>  = PRIi8;
        template<>
        constexpr
        const char* format_string_v<std::uint8_t> = PRIu8;

        template<>
        constexpr
        const char* format_string_v<std::int16_t>  = PRIi16;
        template<>
        constexpr
        const char* format_string_v<std::uint16_t> = PRIu16;

        template<>
        constexpr
        const char* format_string_v<std::int32_t>  = PRIi32;
        template<>
        constexpr
        const char* format_string_v<std::uint32_t> = PRIu32;

        template<>
        constexpr
        const char* format_string_v<std::int64_t>  = PRIi64;
        template<>
        constexpr
        const char* format_string_v<std::uint64_t> = PRIu64;

        template<>
        constexpr
        const char* format_string_v<float>        = "f";

        template<>
        constexpr
        const char* format_string_v<double>       = "lf";

        template<>
        constexpr
        const char* format_string_v<char *>       = "s";
        template<>
        constexpr
        const char* format_string_v<const char *> = "s";

    } // namespace



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
    INSTANTIATE_N(float);
    INSTANTIATE_N(double);

#undef INSTANTIATE_N
#undef INSTANTIATE


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
    INSTANTIATE_N(float);
    INSTANTIATE_N(double);

#undef INSTANTIATE_N
#undef INSTANTIATE


    namespace {

        namespace InputTextHelper {

            struct Data {
                std::string* str;
                InputTextFunction func;
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
                } else if (user_data->func)
                    user_data->func(data);
                return 0;
            }

        } // Namespace InputTextHelper

    } // namespace


    bool
    InputText(const std::string& label,
              std::string& value,
              ImGuiInputTextFlags flags,
              InputTextFunction func)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;
        InputTextHelper::Data data;
        data.str = &value;
        data.func = std::move(func);
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
              InputTextFunction func)
    {
        return InputText(label, *value, flags, std::move(func));
    }


    bool
    InputTextMultiline(const std::string& label,
                       std::string& value,
                       const ImVec2& size,
                       ImGuiInputTextFlags flags,
                       InputTextFunction func)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;
        InputTextHelper::Data data;
        data.str = &value;
        data.func = std::move(func);
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
                       InputTextFunction func)
    {
        return InputTextMultiline(label, *value, size, flags, std::move(func));
    }


    bool
    InputTextWithHint(const std::string& label,
                      const std::string& hint,
                      std::string& value,
                      ImGuiInputTextFlags flags,
                      InputTextFunction func)
    {
        flags |= ImGuiInputTextFlags_CallbackResize;
        InputTextHelper::Data data;
        data.str = &value;
        data.func = std::move(func);
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
                      std::string& value,
                      ImGuiInputTextFlags flags,
                      InputTextFunction func)
    {
        return InputTextWithHint(label, hint, *value, flags, std::move(func));
    }


    bool
    IsPopupOpen(const std::string& str_id,
                ImGuiPopupFlags flags)
    {
        return IsPopupOpen(str_id.data(), flags);
    }


    void
    OpenPopup(const std::string& str_id,
              ImGuiPopupFlags popup_flags)
    {
        OpenPopup(str_id.data(), popup_flags);
    }


    void
    PushID(const std::string& id)
    {
        PushID(id.data(), id.data() + id.size());
    }


    void
    PushID(std::string_view id)
    {
        PushID(id.data(), id.data() + id.size());
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


    void
    SeparatorText(const std::string& label)
    {
        SeparatorText(label.data());
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
    INSTANTIATE_N(float);
    INSTANTIATE_N(double);

#undef INSTANTIATE_N
#undef INSTANTIATE


    bool
    TabItemButton(const std::string& label,
                  ImGuiTabItemFlags flags)
    {
        return TabItemButton(label.data(), flags);
    }


    void
    Text(const std::string& text)
    {
        return Text("%s", text.data());
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


    template<typename T>
    void
    Value(const std::string& prefix,
          T value)
    {
        const std::string fmt = "%s: %"s + format_string_v<T>;
        Text(fmt.data(),
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
             x)

    INSTANTIATE(std::int8_t);
    INSTANTIATE(std::uint8_t);

    INSTANTIATE(std::int16_t);
    INSTANTIATE(std::uint16_t);

    INSTANTIATE(std::int32_t);
    INSTANTIATE(std::uint32_t);

    INSTANTIATE(std::int64_t);
    INSTANTIATE(std::uint64_t);

    INSTANTIATE(char);
    INSTANTIATE(float);
    INSTANTIATE(double);

    INSTANTIATE(char*);
    INSTANTIATE(const char*);

#undef INSTANTIATE


    void
    Value(const std::string& prefix,
          const std::string& value)
    {
        Text("%s: %s", prefix.data(), value.data());
    }

} // namespace ImGui

#endif // IMGUI_DISABLE
