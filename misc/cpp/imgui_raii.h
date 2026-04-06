// -*- mode: c++ -*-
/*
 * RAII helpers for ImGui
 *
 * Copyright (C) 2026  Daniel K. O. <dkosmari>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifndef IMGUI_DISABLE

#include <imgui.h>

#include "imgui_stdlib.h"


namespace ImGui::RAII {

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



    struct Child : detail::Conditional {

        explicit
        Child(const auto& str_id,
              const ImVec2& size = {0, 0},
              ImGuiChildFlags child_flags = 0,
              ImGuiWindowFlags window_flags = 0)
            noexcept :
            detail::Conditional{BeginChild(str_id,
                                           size,
                                           child_flags,
                                           window_flags)}
        {}

        ~Child()
            noexcept
        {
            // Note: ImGui::EndChild() is unconditional.
            EndChild();
        }

    }; // struct Child


    struct ClipRect : detail::Immovable {

        explicit
        ClipRect(const ImVec2& min,
                 const ImVec2& max,
                 bool intersect)
            noexcept
        {
            PushClipRect(min, max, intersect);
        }

        ~ClipRect()
            noexcept
        {
            PopClipRect();
        }

    }; // struct ClipRect


    struct Combo : detail::Conditional {

        explicit
        Combo(const auto& label,
              const auto& preview,
              ImGuiComboFlags flags = 0)
            noexcept :
            detail::Conditional{BeginCombo(label, preview, flags)}
        {}

        ~Combo()
            noexcept
        {
            if (status)
                EndCombo();
        }

    }; // struct Combo


    struct Disabled : detail::Immovable {

        explicit
        Disabled(bool disabled)
            noexcept
        {
            BeginDisabled(disabled);
        }

        ~Disabled()
            noexcept
        {
            EndDisabled();
        }

    }; // struct Disabled


    struct DragDropSource : detail::Conditional {

        explicit
        DragDropSource(ImGuiDragDropFlags flags = 0)
            noexcept :
            detail::Conditional{BeginDragDropSource(flags)}
        {}

        ~DragDropSource()
            noexcept
        {
            if (status)
                EndDragDropSource();
        }

    }; // struct DragDropSource


    struct DragDropTarget : detail::Conditional {

        explicit
        DragDropTarget()
            noexcept :
            detail::Conditional{BeginDragDropTarget()}
        {}

        ~DragDropTarget()
            noexcept
        {
            if (status)
                EndDragDropTarget();
        }

    }; // struct DragDropTarget


    struct Font : detail::Immovable {

        explicit
        Font(ImFont* font,
             float size)
            noexcept
        {
            PushFont(font, size);
        }

        ~Font()
            noexcept
        {
            PopFont();
        }

    }; // struct Font


    struct Group : detail::Immovable {

        Group()
            noexcept
        {
            BeginGroup();
        }

        ~Group()
            noexcept
        {
            EndGroup();
        }

    }; // struct Group


    struct ID : detail::Immovable {

        explicit
        ID(const auto&... args)
            noexcept
        {
            PushID(args...);
        }

        ~ID()
            noexcept
        {
            PopID();
        }

    }; // struct ID


    class Indent : detail::Immovable {

        const float width;

    public:

        explicit
        Indent(float amount = 0)
            noexcept :
            width(amount)
        {
            ImGui::Indent(width);
        }

        ~Indent()
            noexcept
        {
            ImGui::Unindent(width);
        }

    }; // struct Indent


    struct ItemFlag : detail::Immovable {

        explicit
        ItemFlag(ImGuiItemFlags opt,
                 bool enabled)
            noexcept
        {
            PushItemFlag(opt, enabled);
        }

        ~ItemFlag()
            noexcept
        {
            PopItemFlag();
        }

    }; // struct ItemFlag


    struct ItemTooltip : detail::Conditional {

        explicit
        ItemTooltip()
            noexcept :
            detail::Conditional{BeginItemTooltip()}
        {}

        ~ItemTooltip()
            noexcept
        {
            if (status)
                EndTooltip();
        }

    }; // struct ItemTooltip


    struct ItemWidth : detail::Immovable {

        explicit
        ItemWidth(float width)
            noexcept
        {
            PushItemWidth(width);
        }

        ~ItemWidth()
            noexcept
        {
            PopItemWidth();
        }

    }; // struct ItemWidth


    struct ListBox : detail::Conditional {

        explicit
        ListBox(const auto& label,
                const ImVec2& size = {0, 0})
            noexcept :
            detail::Conditional{BeginListBox(label, size)}
        {}

        ~ListBox()
            noexcept
        {
            if (status)
                EndListBox();
        }

    }; // struct ListBox


    struct MainMenuBar : detail::Conditional {

        explicit
        MainMenuBar()
            noexcept :
             detail::Conditional{BeginMainMenuBar()}
        {}

        ~MainMenuBar()
            noexcept
        {
            if (status)
                EndMainMenuBar();
        }

    }; // struct MainMenuBar


    struct Menu : detail::Conditional {

        explicit
        Menu(const auto& label,
             bool enabled = true)
            noexcept :
            detail::Conditional{BeginMenu(label, enabled)}
        {}

        ~Menu()
            noexcept
        {
            if (status)
                EndMenu();
        }

    }; // struct Menu


    struct MenuBar : detail::Conditional {

        explicit
        MenuBar()
            noexcept :
            detail::Conditional{BeginMenuBar()}
        {}

        ~MenuBar()
            noexcept
        {
            if (status)
                EndMenuBar();
        }

    }; // struct MenuBar


    class MultiSelect : detail::Immovable {

        ImGuiMultiSelectIO* const io;

    public:

        explicit
        MultiSelect(ImGuiMultiSelectFlags flags,
                    int selection_size = -1,
                    int items_count = -1)
            noexcept :
            io{BeginMultiSelect(flags, selection_size, items_count)}
        {}

        ~MultiSelect()
            noexcept
        {
            EndMultiSelect();
        }

        ImGuiMultiSelectIO*
        operator ->()
        {
            return io;
        }

    }; // class MultiSelect


    struct Popup : detail::Conditional {

        explicit
        Popup(const auto& str_id,
              ImGuiWindowFlags flags = 0)
            noexcept :
            detail::Conditional{BeginPopup(str_id, flags)}
        {}

        ~Popup()
            noexcept
        {
            if (status)
                EndPopup();
        }

    }; // struct Popup


    struct PopupModal : detail::Conditional {

        explicit
        PopupModal(const auto& name,
                   bool* p_open = nullptr,
                   ImGuiWindowFlags flags = 0)
            noexcept :
            detail::Conditional{BeginPopupModal(name, p_open, flags)}
        {}


        ~PopupModal()
        {
            if (status)
                EndPopup();
        }

    }; // struct PopupModal


    struct StyleColor : detail::Immovable {

        explicit
        StyleColor(ImGuiCol idx,
                   const auto&... args)
            noexcept
        {
            PushStyleColor(idx, args...);
        }

        ~StyleColor()
            noexcept
        {
            PopStyleColor();
        }

    }; // struct StyleColor


    struct StyleVar : detail::Immovable {

        explicit
        StyleVar(ImGuiStyleVar idx,
                 const auto& arg)
            noexcept
        {
            PushStyleVar(idx, arg);
        }

        ~StyleVar()
            noexcept
        {
            PopStyleVar();
        }

    }; // struct StyleVar


    struct StyleVarX : detail::Immovable {

        explicit
        StyleVarX(ImGuiStyleVar idx,
                  float x)
            noexcept
        {
            PushStyleVarX(idx, x);
        }

        ~StyleVarX()
            noexcept
        {
            PopStyleVar();
        }

    }; // struct StyleVarX


    struct StyleVarY : detail::Immovable {

        explicit
        StyleVarY(ImGuiStyleVar idx,
                  float y)
            noexcept
        {
            PushStyleVarY(idx, y);
        }

        ~StyleVarY()
            noexcept
        {
            PopStyleVar();
        }

    }; // struct StyleVarY


    struct TabBar : detail::Conditional {

        explicit
        TabBar(const auto& str_id,
               ImGuiTabBarFlags flags = 0)
            noexcept :
            detail::Conditional{BeginTabBar(str_id, flags)}
        {}

        ~TabBar()
            noexcept
        {
            if (status)
                EndTabBar();
        }

    }; // struct TabBar


    struct TabItem : detail::Conditional {

        explicit
        TabItem(const auto& label,
                bool* p_open = nullptr,
                ImGuiTabItemFlags flags = 0)
            noexcept :
            detail::Conditional{BeginTabItem(label, p_open, flags)}
        {}

        ~TabItem()
            noexcept
        {
            if (status)
                EndTabItem();
        }

    }; // struct TabItem


    struct Table : detail::Conditional {

        explicit
        Table(const auto& str_id,
              int columns,
              ImGuiTableFlags flags = 0,
              const ImVec2& outer_size = {0, 0},
              float inner_width = 0)
            noexcept :
            detail::Conditional{BeginTable(str_id,
                                           columns,
                                           flags,
                                           outer_size,
                                           inner_width)}
        {}

        ~Table()
            noexcept
        {
            if (status)
                EndTable();
        }

    }; // struct Table


    struct TextWrapPos : detail::Immovable {

        explicit
        TextWrapPos(float pos = 0.0f)
            noexcept
        {
            PushTextWrapPos(pos);
        }

        ~TextWrapPos()
            noexcept
        {
            PopTextWrapPos();
        }

    }; // struct TextWrapPos

    struct Tooltip : detail::Conditional {

        explicit
        Tooltip()
            noexcept :
            detail::Conditional{BeginTooltip()}
        {}

        ~Tooltip()
            noexcept
        {
            if (status)
                EndTooltip();
        }

    }; // struct Tooltip


    struct Window : detail::Conditional {

        explicit
        Window(const auto& name,
               bool* p_open = nullptr,
               ImGuiWindowFlags flags = 0)
            noexcept :
            detail::Conditional{Begin(name, p_open, flags)}
        {}

        ~Window()
            noexcept
        {
            // Note: ImGui::End() is unconditional.
            End();
        }

    }; // struct Window

} // namespace ImGui::RAII


#endif // IMGUI_DISABLE
