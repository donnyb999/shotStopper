/*
 * Embedded XML UI definitions for LVGL screens.
 * These strings can be edited in the LVGL Online Editor and then embedded here.
 */

#ifndef UI_XML_STRINGS_H
#define UI_XML_STRINGS_H

#include <Arduino.h>

// Shot Stopper Screen XML
// Note: On ESP32, PROGMEM is not needed as strings are already in flash
const char shot_stopper_screen_xml[] = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<screen name=\"screen_shot_stopper\" width=\"360\" height=\"360\" bg_color=\"#000000\" scrollable=\"false\">\n"
"  <label name=\"ble_status_label\" text=\"B\" align=\"top_mid\" y=\"10\" font=\"montserrat_24\" text_color=\"#808080\"/>\n"
"  <label name=\"title_label\" text=\"Target Weight (g)\" align=\"top_mid\" y=\"50\" font=\"montserrat_24\" text_color=\"#FFFFFF\"/>\n"
"  <label name=\"weight_label\" text=\"...\" align=\"center\" y=\"-30\" font=\"montserrat_48\" text_color=\"#FFFFFF\"/>\n"
"  <label name=\"checkmark_label\" text=\"OK\" align=\"center\" y=\"10\" font=\"montserrat_24\" text_color=\"#00FF00\" hidden=\"true\"/>\n"
"  <obj name=\"preset_container\" width=\"320\" height=\"80\" align=\"bottom_mid\" y=\"-60\" flex_flow=\"row\" flex_align=\"space_evenly,center,center\" remove_style=\"true\">\n"
"    <btn name=\"preset_btn_0\" width=\"90\" height=\"60\" bg_color=\"#808080\" clickable=\"true\">\n"
"      <label name=\"preset_label_0\" text=\"36 g\" font=\"montserrat_24\" text_color=\"#FFFFFF\"/>\n"
"    </btn>\n"
"    <btn name=\"preset_btn_1\" width=\"90\" height=\"60\" bg_color=\"#808080\" clickable=\"true\">\n"
"      <label name=\"preset_label_1\" text=\"40 g\" font=\"montserrat_24\" text_color=\"#FFFFFF\"/>\n"
"    </btn>\n"
"    <btn name=\"preset_btn_2\" width=\"90\" height=\"60\" bg_color=\"#808080\" clickable=\"true\">\n"
"      <label name=\"preset_label_2\" text=\"45 g\" font=\"montserrat_24\" text_color=\"#FFFFFF\"/>\n"
"    </btn>\n"
"  </obj>\n"
"  <label name=\"battery_label\" text=\"Batt: --%\" align=\"bottom_mid\" y=\"-20\" font=\"montserrat_16\" text_color=\"#FFFFFF\"/>\n"
"</screen>\n";

// Home Assistant Screen XML
const char home_assistant_screen_xml[] = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<screen name=\"screen_ha\" width=\"360\" height=\"360\" bg_color=\"#343a40\" scrollable=\"false\">\n"
"  <btn name=\"ha_on_off_btn\" width=\"180\" height=\"50\" align=\"top_mid\" y=\"15\" checkable=\"true\" clickable=\"true\">\n"
"    <label text=\"P ON/OFF\"/>\n"
"  </btn>\n"
"  <obj name=\"ha_mode_cont\" width=\"150\" height=\"60\" align=\"top_left\" x=\"20\" y=\"80\" clickable=\"true\">\n"
"    <label name=\"ha_mode_label\" text=\"Pre-brew\"/>\n"
"  </obj>\n"
"  <obj name=\"ha_preinf_time_cont\" width=\"150\" height=\"80\" align=\"top_right\" x=\"-20\" y=\"80\" clickable=\"true\">\n"
"    <label name=\"ha_preinf_time_label\" text=\"0.8s\"/>\n"
"  </obj>\n"
"  <obj name=\"ha_temp_cont\" width=\"150\" height=\"80\" align=\"center\" x=\"-85\" y=\"30\" clickable=\"true\">\n"
"    <label name=\"ha_temp_label\" text=\"93.0 C\"/>\n"
"  </obj>\n"
"  <obj name=\"ha_steam_cont\" width=\"120\" height=\"80\" align=\"center\" x=\"85\" y=\"30\" clickable=\"true\">\n"
"    <label name=\"ha_steam_label\" text=\"Pwr: 3\"/>\n"
"  </obj>\n"
"  <btn name=\"ha_backflush_cont\" width=\"180\" height=\"50\" align=\"bottom_right\" x=\"-20\" y=\"-15\" clickable=\"true\">\n"
"    <label text=\"R BACKFLUSH\"/>\n"
"  </btn>\n"
"  <obj name=\"last_shot_cont\" width=\"120\" height=\"50\" align=\"bottom_left\" x=\"20\" y=\"-15\">\n"
"    <label name=\"ha_last_shot_label\" text=\"Last: 0.0s\"/>\n"
"  </obj>\n"
"</screen>\n";

#endif // UI_XML_STRINGS_H

