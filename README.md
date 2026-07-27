# About this ImGui fork

This fork is kept up-to-date with ImGui releases, with a number of patches and additions:


## Changes from upstream

- Modified ImGui functions:
  - `ImGui::ArrowButton()`: has an optional `size` argument.
  - `ImGui::TextAligned()`: moved from `imgui_private.h` to the public API.
  - `ImGuiStorage`: has templated `Get()` and `Set()` methods to be used for enums.

- Changes to optimize ImGui for touch screen interaction:
  ```cpp
      ImGuiIO& io = ImGui::GetIO();
      io.ConfigDragScroll = true;
      io.ConfigWindowsMoveFromTitleBarOnly = true;
      io.MouseDragThreshold = 25; // needs tweaking for each environment
      io.DragFlickThreshold = 300; // needs tweaking for each environment
  ```
  Extra functions related to drag scroll interaction:
    - `ImGui::GetDragScrollFlick()`
    - `ImGui::GetDragScrollVelocity()`
    - `ImGui::IsDragScrollGliding()`
    - `ImGui::IsDragScrolling()`
    - `ImGui::IsWindowHorizontalScrollbarActive()`
    - `ImGui::IsWindowVerticalScrollbarActive()`
    - `ImGui::SetDragScrollVelocity()`

- Also a Carousel widget in `misc/carousel`.

- Wii U backend, by @GaryOderNichts.

- C++ additions in `misc/cpp`:
  - `imgui_raii.h`: RAII types for `Begin*`/`End*` pairs.
  - `imgui_stdlib.h`, `imgui_stdlib.cpp`: Lots of overloads, so `std::string` can be used
    seamlessly. Also has extra convenience functions:
    - `ImGui::Drag<T>()`: wrapper for `ImGui::DragScalar()` and `ImGui::DragScalarN()`.
    - `ImGui::Slider<T>()`: wrapper for `ImGui::SliderScalar()` and `ImGui::SliderScalarN()`.
    - `ImGui::VSlider<T>()`: wrapper for `ImGui::VSliderScalar()` and `ImGui::VSliderScalarN()`.
    - `ImGui::Input<T>()`: wrapper for `ImGui::InputScalar()` and `ImGui::InputScalarN()`.
    - `ImGui::Format*()`: expose formatting functions using `std::format()` formatting, instead
      of `printf`:
       ```cpp
       ImGui::Text("The answer is %d", 42);
       ImGui::FormatText("The answer is {}", 42);
       ```
    - `ImGui::to_string(ImGuiCol_ color)': wrapper for ImGui::GetStyleColorName(color)`.
    - `ToVec2()`: convert from any type with `.x` and `.y` into `ImVec2`.
    - `ToVec4()`: convert from any type with `.x`, `.y`, `.z`, `.w`, or `.r`, `.g`, `.b`, .a`,
      into `ImVec4`.

## Documentation

See [docs/README.md](docs/README.md).
