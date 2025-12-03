/*
 * LVGL XML Loader implementation.
 * Simple XML parser for LVGL UI definitions compatible with LVGL Online Editor format.
 */

#include "lvgl_xml_loader.h"
#include <Arduino.h>
#include <string.h>
#include <stdlib.h>

// Simple XML attribute structure
typedef struct {
    char name[32];
    char value[128];
} xml_attr_t;

// Simple XML element structure
typedef struct {
    char tag[32];
    xml_attr_t attrs[16];
    int attr_count;
    const char* text_content;
} xml_element_t;

// Helper function to skip whitespace
static const char* skip_whitespace(const char* s) {
    while (*s && (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r')) {
        s++;
    }
    return s;
}

// Helper function to parse XML attribute
static const char* parse_attribute(const char* s, xml_attr_t* attr) {
    s = skip_whitespace(s);
    if (*s == '\0' || *s == '>' || *s == '/') return s;
    
    const char* name_start = s;
    while (*s && *s != '=' && *s != ' ' && *s != '\t' && *s != '>' && *s != '/') s++;
    int name_len = s - name_start;
    if (name_len >= sizeof(attr->name)) name_len = sizeof(attr->name) - 1;
    strncpy(attr->name, name_start, name_len);
    attr->name[name_len] = '\0';
    
    s = skip_whitespace(s);
    if (*s != '=') return s;
    s++;
    s = skip_whitespace(s);
    
    char quote = *s;
    if (quote != '"' && quote != '\'') return s;
    s++;
    
    const char* value_start = s;
    while (*s && *s != quote) s++;
    int value_len = s - value_start;
    if (value_len >= sizeof(attr->value)) value_len = sizeof(attr->value) - 1;
    strncpy(attr->value, value_start, value_len);
    attr->value[value_len] = '\0';
    
    if (*s == quote) s++;
    return s;
}

// Helper function to get attribute value
static const char* get_attr_value(xml_element_t* elem, const char* attr_name) {
    for (int i = 0; i < elem->attr_count; i++) {
        if (strcmp(elem->attrs[i].name, attr_name) == 0) {
            return elem->attrs[i].value;
        }
    }
    return NULL;
}

// Helper function to parse integer
static int parse_int(const char* s, int default_val) {
    if (!s) return default_val;
    return atoi(s);
}

// Helper function to parse color from hex string
static lv_color_t parse_color(const char* hex_str) {
    if (!hex_str || hex_str[0] != '#') {
        return lv_color_black();
    }
    
    uint32_t hex = strtoul(hex_str + 1, NULL, 16);
    if (strlen(hex_str) == 7) { // #RRGGBB
        return lv_color_hex(hex);
    } else if (strlen(hex_str) == 4) { // #RGB
        uint8_t r = ((hex >> 8) & 0xF) * 17;
        uint8_t g = ((hex >> 4) & 0xF) * 17;
        uint8_t b = (hex & 0xF) * 17;
        return lv_color_make(r, g, b);
    }
    return lv_color_black();
}

// Helper function to find font by name
static const lv_font_t* get_font_by_name(const char* font_name) {
    if (!font_name) return NULL;
    
    if (strcmp(font_name, "montserrat_16") == 0) return &lv_font_montserrat_16;
    if (strcmp(font_name, "montserrat_24") == 0) return &lv_font_montserrat_24;
    if (strcmp(font_name, "montserrat_48") == 0) {
        #if LV_FONT_MONTSERRAT_48
        return &lv_font_montserrat_48;
        #else
        return &lv_font_montserrat_24;
        #endif
    }
    return NULL;
}

// Create LVGL object from XML element
static lv_obj_t* create_object_from_xml(xml_element_t* elem, lv_obj_t* parent) {
    lv_obj_t* obj = NULL;
    
    if (strcmp(elem->tag, "obj") == 0 || strcmp(elem->tag, "screen") == 0) {
        obj = lv_obj_create(parent);
    } else if (strcmp(elem->tag, "label") == 0) {
        obj = lv_label_create(parent);
    } else if (strcmp(elem->tag, "btn") == 0) {
        obj = lv_btn_create(parent);
    } else if (strcmp(elem->tag, "container") == 0) {
        obj = lv_obj_create(parent);
    } else {
        // Unknown tag, create as obj
        obj = lv_obj_create(parent);
    }
    
    if (!obj) return NULL;
    
    // Apply common attributes
    const char* width = get_attr_value(elem, "width");
    const char* height = get_attr_value(elem, "height");
    if (width && height) {
        lv_obj_set_size(obj, parse_int(width, 0), parse_int(height, 0));
    }
    
    const char* x = get_attr_value(elem, "x");
    const char* y = get_attr_value(elem, "y");
    const char* align = get_attr_value(elem, "align");
    if (align) {
        // Parse alignment (e.g., "top_mid", "center", "bottom_left")
        lv_align_t align_val = LV_ALIGN_CENTER;
        if (strcmp(align, "top_mid") == 0) align_val = LV_ALIGN_TOP_MID;
        else if (strcmp(align, "top_left") == 0) align_val = LV_ALIGN_TOP_LEFT;
        else if (strcmp(align, "top_right") == 0) align_val = LV_ALIGN_TOP_RIGHT;
        else if (strcmp(align, "center") == 0) align_val = LV_ALIGN_CENTER;
        else if (strcmp(align, "bottom_mid") == 0) align_val = LV_ALIGN_BOTTOM_MID;
        else if (strcmp(align, "bottom_left") == 0) align_val = LV_ALIGN_BOTTOM_LEFT;
        else if (strcmp(align, "bottom_right") == 0) align_val = LV_ALIGN_BOTTOM_RIGHT;
        
        int x_offset = x ? parse_int(x, 0) : 0;
        int y_offset = y ? parse_int(y, 0) : 0;
        lv_obj_align(obj, align_val, x_offset, y_offset);
    }
    
    // Apply styles
    const char* bg_color = get_attr_value(elem, "bg_color");
    if (bg_color) {
        lv_obj_set_style_bg_color(obj, parse_color(bg_color), 0);
    }
    
    const char* text_color = get_attr_value(elem, "text_color");
    if (text_color) {
        lv_obj_set_style_text_color(obj, parse_color(text_color), 0);
    }
    
    const char* font = get_attr_value(elem, "font");
    if (font) {
        const lv_font_t* font_obj = get_font_by_name(font);
        if (font_obj) {
            lv_obj_set_style_text_font(obj, font_obj, 0);
        }
    }
    
    const char* text = get_attr_value(elem, "text");
    if (text && lv_obj_check_type(obj, &lv_label_class)) {
        lv_label_set_text((lv_obj_t*)obj, text);
        // Center label in parent if it's a child
        if (parent) {
            lv_obj_center(obj);
        }
    }
    
    const char* scrollable = get_attr_value(elem, "scrollable");
    if (scrollable && strcmp(scrollable, "false") == 0) {
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    }
    
    const char* remove_style = get_attr_value(elem, "remove_style");
    if (remove_style && strcmp(remove_style, "true") == 0) {
        lv_obj_remove_style_all(obj);
    }
    
    const char* hidden = get_attr_value(elem, "hidden");
    if (hidden && strcmp(hidden, "true") == 0) {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
    
    const char* checkable = get_attr_value(elem, "checkable");
    if (checkable && strcmp(checkable, "true") == 0) {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
    }
    
    const char* clickable = get_attr_value(elem, "clickable");
    if (clickable && strcmp(clickable, "true") == 0) {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    }
    
    // Flex layout attributes
    const char* flex_flow = get_attr_value(elem, "flex_flow");
    if (flex_flow) {
        if (strcmp(flex_flow, "row") == 0) {
            lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
        } else if (strcmp(flex_flow, "column") == 0) {
            lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
        }
    }
    
    const char* flex_align = get_attr_value(elem, "flex_align");
    if (flex_align) {
        // Parse flex_align (e.g., "space_evenly,center,center")
        // For simplicity, we'll use a common alignment
        lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    }
    
    return obj;
}

// Simple recursive XML parser (handles basic nested structures)
static lv_obj_t* parse_xml_recursive(const char** xml_ptr, lv_obj_t* parent, 
                                     lvgl_xml_obj_map_t* obj_map, uint16_t* obj_map_idx, uint16_t obj_map_size) {
    const char* s = *xml_ptr;
    s = skip_whitespace(s);
    
    if (*s == '\0' || *s == '<' && s[1] == '/') {
        return NULL;
    }
    
    if (*s != '<') return NULL;
    s++;
    
    xml_element_t elem;
    memset(&elem, 0, sizeof(elem));
    elem.attr_count = 0;
    
    // Parse tag name
    const char* tag_start = s;
    while (*s && *s != ' ' && *s != '\t' && *s != '>' && *s != '/') s++;
    int tag_len = s - tag_start;
    if (tag_len >= sizeof(elem.tag)) tag_len = sizeof(elem.tag) - 1;
    strncpy(elem.tag, tag_start, tag_len);
    elem.tag[tag_len] = '\0';
    
    // Parse attributes
    s = skip_whitespace(s);
    while (*s && *s != '>' && *s != '/' && elem.attr_count < 16) {
        s = parse_attribute(s, &elem.attrs[elem.attr_count]);
        elem.attr_count++;
        s = skip_whitespace(s);
    }
    
    // Check for self-closing tag
    bool self_closing = (*s == '/');
    if (self_closing) s++;
    if (*s == '>') s++;
    
    // Create object
    lv_obj_t* obj = create_object_from_xml(&elem, parent);
    if (!obj) {
        *xml_ptr = s;
        return NULL;
    }
    
    // Store in object map if name is provided
    const char* name = get_attr_value(&elem, "name");
    if (name && obj_map && *obj_map_idx < obj_map_size) {
        obj_map[*obj_map_idx].name = name;
        obj_map[*obj_map_idx].obj = obj;
        (*obj_map_idx)++;
    }
    
    if (self_closing) {
        *xml_ptr = s;
        return obj;
    }
    
    // Parse children and text content
    s = skip_whitespace(s);
    
    // Check if there's text content before the first child tag
    const char* text_start = s;
    while (*s && *s != '<') s++;
    if (s > text_start) {
        // There's text content - for labels, set it
        int text_len = s - text_start;
        if (text_len > 0 && lv_obj_check_type(obj, &lv_label_class)) {
            char text_buf[256];
            if (text_len >= sizeof(text_buf)) text_len = sizeof(text_buf) - 1;
            strncpy(text_buf, text_start, text_len);
            text_buf[text_len] = '\0';
            // Trim whitespace
            while (text_len > 0 && (text_buf[text_len-1] == ' ' || text_buf[text_len-1] == '\n' || text_buf[text_len-1] == '\t')) {
                text_buf[--text_len] = '\0';
            }
            if (text_len > 0) {
                lv_label_set_text((lv_obj_t*)obj, text_buf);
            }
        }
    }
    
    // Parse child elements
    while (*s == '<' && s[1] != '/') {
        lv_obj_t* child = parse_xml_recursive(&s, obj, obj_map, obj_map_idx, obj_map_size);
        if (child) {
            // Child already added to parent
        }
        s = skip_whitespace(s);
    }
    
    // Find closing tag
    if (*s == '<' && s[1] == '/') {
        s += 2;
        while (*s && *s != '>') s++;
        if (*s == '>') s++;
    }
    
    *xml_ptr = s;
    return obj;
}

lv_obj_t* lvgl_xml_load_from_string(const char* xml_string, lv_obj_t* parent, 
                                     lvgl_xml_obj_map_t* obj_map, uint16_t obj_map_size) {
    if (!xml_string) return NULL;
    
    // Initialize object map
    if (obj_map) {
        for (uint16_t i = 0; i < obj_map_size; i++) {
            obj_map[i].name = NULL;
            obj_map[i].obj = NULL;
        }
    }
    
    uint16_t obj_map_idx = 0;
    const char* xml_ptr = xml_string;
    
    // Skip XML declaration if present
    if (strncmp(xml_ptr, "<?xml", 5) == 0) {
        while (*xml_ptr && *xml_ptr != '>') xml_ptr++;
        if (*xml_ptr == '>') xml_ptr++;
    }
    
    return parse_xml_recursive(&xml_ptr, parent, obj_map, &obj_map_idx, obj_map_size);
}

lv_obj_t* lvgl_xml_find_object(lvgl_xml_obj_map_t* obj_map, uint16_t obj_map_size, const char* name) {
    if (!obj_map || !name) return NULL;
    
    for (uint16_t i = 0; i < obj_map_size; i++) {
        if (obj_map[i].name && strcmp(obj_map[i].name, name) == 0) {
            return obj_map[i].obj;
        }
    }
    return NULL;
}

