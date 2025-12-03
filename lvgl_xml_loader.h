/*
 * LVGL XML Loader for loading UI from XML files.
 * Supports loading UI definitions created in LVGL Online Editor.
 */

#ifndef LVGL_XML_LOADER_H
#define LVGL_XML_LOADER_H

#include <lvgl.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Structure to store object name to pointer mapping
typedef struct {
    const char* name;
    lv_obj_t* obj;
} lvgl_xml_obj_map_t;

/**
 * Load an LVGL screen from XML string
 * @param xml_string The XML string containing the UI definition
 * @param parent Parent object (NULL for screen)
 * @param obj_map Array to store name-to-object mappings
 * @param obj_map_size Size of obj_map array
 * @return Created screen/object, or NULL on error
 */
lv_obj_t* lvgl_xml_load_from_string(const char* xml_string, lv_obj_t* parent, 
                                     lvgl_xml_obj_map_t* obj_map, uint16_t obj_map_size);

/**
 * Find an object by name from the object map
 * @param obj_map Object map array
 * @param obj_map_size Size of obj_map array
 * @param name Name of the object to find
 * @return Object pointer or NULL if not found
 */
lv_obj_t* lvgl_xml_find_object(lvgl_xml_obj_map_t* obj_map, uint16_t obj_map_size, const char* name);

#ifdef __cplusplus
}
#endif

#endif // LVGL_XML_LOADER_H

