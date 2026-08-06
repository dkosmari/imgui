// -*- mode: c++ -*-
/*
 * RAII helpers for ImGui
 *
 * Copyright (C) 2026  Daniel K. O. <dkosmari>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <utility>

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

        template<typename... Args>
        explicit
        Child(Args&&... args)
            noexcept :
            detail::Conditional{BeginChild(std::forward<Args>(args)...)}
        {}

        ~Child()
            noexcept
        {
            // NOTE: ImGui::EndChild() is unconditional.
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

        template<typename... Args>
        explicit
        Combo(Args&&... args)
            noexcept :
            detail::Conditional{BeginCombo(std::forward<Args>(args)...)}
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

        explicit
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

        template<typename... Args>
        explicit
        ID(Args&&... args)
            noexcept
        {
            PushID(std::forward<Args>(args)...);
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

        template<typename... Args>
        explicit
        ListBox(Args&&... args)
            noexcept :
            detail::Conditional{BeginListBox(std::forward<Args>(args)...)}
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

        template<typename... Args>
        explicit
        Menu(Args&&... args)
            noexcept :
            detail::Conditional{BeginMenu(std::forward<Args>(args)...)}
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

        template<typename... Args>
        explicit
        Popup(Args&&... args)
            noexcept :
            detail::Conditional{BeginPopup(std::forward<Args>(args)...)}
        {}

        ~Popup()
            noexcept
        {
            if (status)
                EndPopup();
        }

    }; // struct Popup


    struct PopupContextItem : detail::Conditional {

        template<typename... Args>
        explicit
        PopupContextItem(Args&&... args)
            noexcept :
            detail::Conditional{BeginPopupContextItem(std::forward<Args>(args)...)}
        {}

        ~PopupContextItem()
            noexcept
        {
            if (status)
                EndPopup();
        }

    }; // struct PopupContextItem


    struct PopupContextWindow : detail::Conditional {

        template<typename... Args>
        explicit
        PopupContextWindow(Args&&... args)
            noexcept :
            detail::Conditional{BeginPopupContextWindow(std::forward<Args>(args)...)}
        {}

        ~PopupContextWindow()
            noexcept
        {
            if (status)
                EndPopup();
        }

    }; // struct PopupContextWindow


    struct PopupContextVoid : detail::Conditional {

        template<typename... Args>
        explicit
        PopupContextVoid(Args&&... args)
            noexcept :
            detail::Conditional{BeginPopupContextVoid(std::forward<Args>(args)...)}
        {}

        ~PopupContextVoid()
            noexcept
        {
            if (status)
                EndPopup();
        }

    }; // struct PopupContextVoid


    struct PopupModal : detail::Conditional {

        template<typename... Args>
        explicit
        PopupModal(Args&&... args)
            noexcept :
            detail::Conditional{BeginPopupModal(std::forward<Args>(args)...)}
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
                   ImU32 col)
            noexcept
        {
            PushStyleColor(idx, col);
        }

        explicit
        StyleColor(ImGuiCol idx,
                   const ImVec4& col)
            noexcept
        {
            PushStyleColor(idx, col);
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
                 float val)
            noexcept
        {
            PushStyleVar(idx, val);
        }

        explicit
        StyleVar(ImGuiStyleVar idx,
                 const ImVec2& val)
            noexcept
        {
            PushStyleVar(idx, val);
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

        template<typename... Args>
        explicit
        TabBar(Args&&... args)
            noexcept :
            detail::Conditional{BeginTabBar(std::forward<Args>(args)...)}
        {}

        ~TabBar()
            noexcept
        {
            if (status)
                EndTabBar();
        }

    }; // struct TabBar


    struct TabItem : detail::Conditional {

        template<typename... Args>
        explicit
        TabItem(Args&&... args)
            noexcept :
            detail::Conditional{BeginTabItem(std::forward<Args>(args)...)}
        {}

        ~TabItem()
            noexcept
        {
            if (status)
                EndTabItem();
        }

    }; // struct TabItem


    struct Table : detail::Conditional {

        template<typename... Args>
        explicit
        Table(Args&&... args)
            noexcept :
            detail::Conditional{BeginTable(std::forward<Args>(args)...)}
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

        template<typename... Args>
        explicit
        Window(Args&&... args)
            noexcept :
            detail::Conditional{Begin(std::forward<Args>(args)...)}
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
