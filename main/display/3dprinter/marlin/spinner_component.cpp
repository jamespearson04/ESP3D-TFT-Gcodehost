/*
  esp3d_tft

  Copyright (c) 2022 Luc Lebosse. All rights reserved.

  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "spinner_component.h"

#include <string>

#include "esp3d_hal.h"
#include "esp3d_log.h"
#include "esp3d_styles.h"
#include "esp3d_tft_ui.h"
#include "translations/esp3d_translation_service.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/
namespace spinnerScreen {
lv_obj_t* spinnerObj = NULL;

void show_spinner() {
  // to avoid multiple call to show spinner
  if (spinnerObj != NULL) {
    return;
  }
  spinnerObj = lv_obj_create(lv_scr_act());
  apply_style(spinnerObj, ESP3DStyleType::spinnerScreen);
  lv_obj_move_foreground(spinnerObj);
  lv_obj_set_size(spinnerObj, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_pos(spinnerObj, 0, 0);
  lv_obj_t* spinner = lv_spinner_create(spinnerObj, 1000, 60);
  lv_obj_set_size(spinner, CURRENT_SPINNER_SIZE, CURRENT_SPINNER_SIZE);
  lv_obj_center(spinner);

  std::string text = esp3dTranslationService.translate(ESP3DLabel::please_wait);
  lv_obj_t* lbl = lv_label_create(spinnerObj);
  apply_style(lbl, ESP3DStyleType::spinnerText);
  lv_label_set_text(lbl, text.c_str());
  lv_obj_align_to(lbl, spinner, LV_ALIGN_OUT_BOTTOM_MID, 0,
                  CURRENT_BUTTON_PRESSED_OUTLINE);
}
void hide_spinner() {
  if (spinnerObj != NULL) {
    lv_obj_del(spinnerObj);
    spinnerObj = NULL;
  }
}

}  // namespace spinnerScreen