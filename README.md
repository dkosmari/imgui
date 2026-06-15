# About this ImGui fork

This fork is kept up-to-date with ImGui releases, with a number of patches and additions:


## Changes from upstream

- Modified ImGui functions:
  - `ImGui::ArrowButton()`: has an optional `size` argument.
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
  Also a Carousel widget in `misc/carousel`.

- Wii U backend, by @GaryOderNichts.

- C++ additions in `misc/cpp`:
  - `imgui_raii.h`: RAII types for `Begin*`/`End*` pairs.
  - `imgui_stdlib.h`: Lots of overloads, so `std::string` can be used seamlessly.
  - `imgui_stdlib.cpp`: Implementation for the overloads.


## Documentation

See [docs/README.md](docs/README.md).
