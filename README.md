# Sway Display Switch

A GUI for toggling Sway outputs.

## Controls

Left: `Left arrow`, `A` or `H`  
Right: `Right arrow`, `D` or`L`  
Toggle: `Enter` or `Space`  
Exit: `Escape` or `Q`

## Build

`cmake --preset release`

`cmake --build build-relase`

Best used when bound to a key in your Sway config:

`bindsym exec $mod+<key> sway_display_switch`

## Dependencies

All dependencies will be automatically installed via vcpkg.

- SDL3
- nlohmann_json
- Fontconfig

Two SVG icons from [Google Fonts](https://fonts.google.com/icons) are already embedded in `src/ui/Icons.h` as strings.
- [Monitor icon](https://fonts.google.com/icons?selected=Material+Symbols+Outlined:monitor)
- [Laptop icon](https://fonts.google.com/icons?selected=Material+Symbols+Outlined:laptop_chromebook)


## References

- https://man.archlinux.org/man/sway-ipc.7.en  