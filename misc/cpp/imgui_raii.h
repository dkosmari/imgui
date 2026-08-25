// -*- mode: c++ -*-
/*
 * RAII helpers for ImGui
 *
 * Copyright (C) 2026  Daniel K. O. <dkosmari>
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <concepts>
#include <format>
#include <string_view>
#include <type_traits>
#include <utility>

#ifndef IMGUI_DISABLE

#include <imgui.h>

#include "imgui_stdlib.h"


namespace ImGui::RAII {

    namespace detail {

        namespace concepts {

            template<typename T>
            concept string = std::convertible_to<std::decay_t<T>, std::string_view>;

        } // namespace concepts

        struct Immovable {

            constexpr
            Immovable() noexcept = default;

            Immovable(Immovable&&) = delete;

        }; // struct Immovable


        class Conditional : Immovable {

        protected:

            const bool status;

            explicit
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

        template<typename T>
        Child(T&& str_id,
              const ImVec2& size = {0, 0},
              ImGuiChildFlags child_flags = 0,
              ImGuiWindowFlags window_flags = 0)
            noexcept :
            detail::Conditional{BeginChild(std::forward<T>(str_id),
                                           size,
                                           child_flags,
                                           window_flags)}
        {}

        ~Child()
            noexcept
        {
            // NOTE: ImGui::EndChild() is unconditional.
            EndChild();
        }

    }; // struct Child


    struct ClipRect : detail::Immovable {

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

        template<typename T,
                 typename U>
        Combo(T&& label,
              U&& preview,
              ImGuiComboFlags flags = 0)
            noexcept :
            detail::Conditional{BeginCombo(std::forward<T>(label),
                                           std::forward<U>(preview),
                                           flags)}
        {}

        ~Combo()
            noexcept
        {
            if (status)
                EndCombo();
        }

    }; // struct Combo


    struct Disabled : detail::Immovable {

        Disabled(bool disabled = true)
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

        Font(ImFont* font,
             float size = 0.0f,
             float scale = 0.0f)
            noexcept
        {
            PushFont(font, size, scale);
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

        template<typename... Args>
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

        template<typename T>
        ListBox(T&& label,
                const ImVec2& size = {0, 0})
            noexcept :
            detail::Conditional{BeginListBox(std::forward<T>(label),
                                             size)}
        {}

        ~ListBox()
            noexcept
        {
            if (status)
                EndListBox();
        }

    }; // struct ListBox


    struct MainMenuBar : detail::Conditional {

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

        template<typename T>
        Menu(T&& label,
             bool enabled = true)
            noexcept :
            detail::Conditional{BeginMenu(std::forward<T>(label),
                                          enabled)}
        {}

        ~Menu()
            noexcept
        {
            if (status)
                EndMenu();
        }

    }; // struct Menu


    struct MenuBar : detail::Conditional {

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

        template<typename T>
        Popup(T&& str_id,
              ImGuiWindowFlags flags = 0)
            noexcept :
            detail::Conditional{BeginPopup(std::forward<T>(str_id),
                                           flags)}
        {}

        ~Popup()
            noexcept
        {
            if (status)
                EndPopup();
        }

    }; // struct Popup


    struct PopupContextItem : detail::Conditional {

        PopupContextItem()
            noexcept :
            detail::Conditional{BeginPopupContextItem()}
        {}

        template<typename T>
        PopupContextItem(T&& str_id,
                         ImGuiPopupFlags popup_flags = 0)
            noexcept :
            detail::Conditional{BeginPopupContextItem(std::forward<T>(str_id),
                                                      popup_flags)}
        {}

        ~PopupContextItem()
            noexcept
        {
            if (status)
                EndPopup();
        }

    }; // struct PopupContextItem


    struct PopupContextWindow : detail::Conditional {

        PopupContextWindow()
            noexcept :
            detail::Conditional{BeginPopupContextWindow()}
        {}

        template<typename T>
        PopupContextWindow(T&& str_id,
                           ImGuiPopupFlags popup_flags = 0)
            noexcept :
            detail::Conditional{BeginPopupContextWindow(std::forward<T>(str_id),
                                                        popup_flags)}
        {}

        ~PopupContextWindow()
            noexcept
        {
            if (status)
                EndPopup();
        }

    }; // struct PopupContextWindow


    struct PopupContextVoid : detail::Conditional {

        PopupContextVoid()
            noexcept :
            detail::Conditional{BeginPopupContextVoid()}
        {}

        template<typename T>
        PopupContextVoid(T&& str_id,
                         ImGuiPopupFlags popup_flags = 0)
            noexcept :
            detail::Conditional{BeginPopupContextVoid(std::forward<T>(str_id),
                                                      popup_flags)}
        {}

        ~PopupContextVoid()
            noexcept
        {
            if (status)
                EndPopup();
        }

    }; // struct PopupContextVoid


    struct PopupModal : detail::Conditional {

        template<typename T>
        PopupModal(T&& name,
                   bool* p_open = nullptr,
                   ImGuiWindowFlags flags = 0)
            noexcept :
            detail::Conditional{BeginPopupModal(std::forward<T>(name),
                                                p_open,
                                                flags)}
        {}

        ~PopupModal()
        {
            if (status)
                EndPopup();
        }

    }; // struct PopupModal


    struct StyleColor : detail::Immovable {

        StyleColor(ImGuiCol idx,
                   ImU32 col)
            noexcept
        {
            PushStyleColor(idx, col);
        }

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

        StyleVar(ImGuiStyleVar idx,
                 float val)
            noexcept
        {
            PushStyleVar(idx, val);
        }

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

        template<typename T>
        TabBar(T&& str_id,
               ImGuiTabBarFlags flags = 0)
            noexcept :
            detail::Conditional{BeginTabBar(std::forward<T>(str_id),
                                            flags)}
        {}

        ~TabBar()
            noexcept
        {
            if (status)
                EndTabBar();
        }

    }; // struct TabBar


    struct TabItem : detail::Conditional {

        template<typename T>
        TabItem(T&& label,
                bool* p_open = nullptr,
                ImGuiTabItemFlags flags = 0)
            noexcept :
            detail::Conditional{BeginTabItem(std::forward<T>(label),
                                             p_open,
                                             flags)}
        {}

        template<typename T>
        TabItem(T&& label,
                bool& open,
                ImGuiTabItemFlags flags = 0)
            noexcept :
            detail::Conditional{BeginTabItem(std::forward<T>(label),
                                             open,
                                             flags)}
        {}

        ~TabItem()
            noexcept
        {
            if (status)
                EndTabItem();
        }

    }; // struct TabItem


    struct Table : detail::Conditional {

        template<typename T>
        Table(T&& str_id,
              int columns,
              ImGuiTableFlags flags = 0,
              const ImVec2& outer_size = {0, 0},
              float inner_width = 0)
            noexcept :
            detail::Conditional{BeginTable(std::forward<T>(str_id),
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


    struct TreeNode : detail::Conditional {

        template<detail::concepts::string Str>
        TreeNode(Str&& label)
            noexcept :
            detail::Conditional{ImGui::TreeNode(std::forward<Str>(label))}
        {}

        template<detail::concepts::string Str1,
                 detail::concepts::string Str2>
        TreeNode(Str1&& str_id,
                 Str2&& label)
            noexcept :
            detail::Conditional{
                ImGui::TreeNode(std::forward<Str1>(str_id),
                                std::forward<Str2>(label))
            }
        {}

        template<detail::concepts::string Str>
        TreeNode(const void* ptr_id,
                 Str&& label)
            noexcept :
            detail::Conditional{
                ImGui::TreeNode(ptr_id,
                                std::forward<Str>(label))
            }
        {}

        template<detail::concepts::string Str>
        TreeNode(Str&& label,
                 ImGuiTreeNodeFlags flags)
            noexcept :
            detail::Conditional{
                ImGui::TreeNodeEx(std::forward<Str>(label),
                                  flags)
            }
        {}

        template<detail::concepts::string Str1,
                 detail::concepts::string Str2>
        TreeNode(Str1&& str_id,
                 ImGuiTreeNodeFlags flags,
                 Str2&& label)
            noexcept :
            detail::Conditional{
                ImGui::TreeNodeEx(std::forward<Str1>(str_id),
                                  flags,
                                  std::forward<Str2>(label))
            }
        {}

        template<detail::concepts::string Str>
        TreeNode(const void* ptr_id,
                 ImGuiTreeNodeFlags flags,
                 Str&& label)
            noexcept :
            detail::Conditional{
                ImGui::TreeNodeEx(ptr_id,
                                  flags,
                                  std::forward<Str>(label))
            }
        {}

        ~TreeNode()
            noexcept
        {
            if (status)
                TreePop();
        }

        // Named constructors for formatting

        template<detail::concepts::string Str,
                 typename... Args>
        static
        TreeNode
        format(Str&& str_id,
               std::format_string<Args...> fmt,
               Args&&... args)
        {
            return TreeNode{
                ImGui::FormatTreeNode(std::forward<Str>(str_id),
                                      std::move(fmt),
                                      std::forward<Args>(args)...)
            };
        }

        template<typename... Args>
        static
        TreeNode
        format(const void* ptr_id,
               std::format_string<Args...> fmt,
               Args&&... args)
        {
            return TreeNode{
                ImGui::FormatTreeNode(ptr_id,
                                      std::move(fmt),
                                      std::forward<Args>(args)...)
            };
        }

        template<detail::concepts::string Str,
                 typename... Args>
        static
        TreeNode
        format(Str&& str_id,
               ImGuiTreeNodeFlags flags,
               std::format_string<Args...> fmt,
               Args&&... args)
        {
            return TreeNode{
                ImGui::FormatTreeNodeEx(std::forward<Str>(str_id),
                                        flags,
                                        std::move(fmt),
                                        std::forward<Args>(args)...)
            };
        }

        template<typename... Args>
        static
        TreeNode
        format(const void* ptr_id,
               ImGuiTreeNodeFlags flags,
               std::format_string<Args...> fmt,
               Args&&... args)
        {
            return TreeNode{
                ImGui::FormatTreeNodeEx(ptr_id,
                                        flags,
                                        std::move(fmt),
                                        std::forward<Args>(args)...)
            };
        }

    private:

        explicit
        TreeNode(bool v) :
            detail::Conditional{v}
        {}

    }; // struct TreeNode


    struct Window : detail::Conditional {

        template<typename T>
        Window(T&& name,
               bool* p_open = nullptr,
               ImGuiWindowFlags flags = 0)
            noexcept :
            detail::Conditional{Begin(std::forward<T>(name),
                                      p_open,
                                      flags)}
        {}

        template<typename T>
        Window(T&& name,
               bool& open,
               ImGuiWindowFlags flags = 0)
            noexcept :
            detail::Conditional{Begin(std::forward<T>(name),
                                      open,
                                      flags)}
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
