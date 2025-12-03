# LVGL UI XML Files

This directory contains XML definitions for the LVGL UI screens that can be edited in the LVGL Online Editor.

## Files

- `shot_stopper_screen.xml` - Main shot stopper screen UI definition
- `home_assistant_screen.xml` - Home Assistant control screen UI definition

## Usage

1. Edit the XML files in the LVGL Online Editor (https://lvgl.io/editor)
2. After editing, copy the XML content to the corresponding strings in `ui/ui_xml_strings.h`
3. Recompile the project

## XML Format

The XML files use a simplified format compatible with the custom XML loader:

- **Tags**: `screen`, `obj`, `label`, `btn`, `container`
- **Attributes**:
  - `name` - Unique identifier for finding objects in code
  - `width`, `height` - Object dimensions
  - `x`, `y` - Position offsets
  - `align` - Alignment (e.g., "top_mid", "center", "bottom_left")
  - `bg_color` - Background color in hex format (e.g., "#000000")
  - `text_color` - Text color in hex format
  - `font` - Font name (e.g., "montserrat_24", "montserrat_48")
  - `text` - Text content for labels
  - `scrollable` - "true" or "false"
  - `hidden` - "true" or "false"
  - `checkable` - "true" or "false"
  - `clickable` - "true" or "false"
  - `flex_flow` - "row" or "column"
  - `remove_style` - "true" to remove all default styles

## Notes

- All objects that need to be accessed from code must have a unique `name` attribute
- Event handlers are attached programmatically after XML loading
- Some properties (like LVGL symbols) are set programmatically after loading

