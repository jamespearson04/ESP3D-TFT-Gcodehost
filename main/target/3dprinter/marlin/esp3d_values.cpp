
/*
  esp3d_values.cpp -  values esp3d functions class

  Copyright (c) 2014 Luc Lebosse. All rights reserved.

  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with This code; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "esp3d_values.h"

#include "esp3d_log.h"
#include "esp3d_string.h"
#if ESP3D_DISPLAY_FEATURE
bool status_bar_cb(ESP3DValuesIndex index, const char *value,
                   ESP3DValuesCbAction action);
#endif  // ESP3D_DISPLAY_FEATURE

bool ESP3DValues::intialize() {
  clear();
#if ESP3D_DISPLAY_FEATURE
  // status bar label
  _values.push_back({ESP3DValuesIndex::status_bar_label,
                     ESP3DValuesType::string_t, 200, "", status_bar_cb});
#endif  // ESP3D_DISPLAY_FEATURE
  //  current ip
  _values.push_back({
      ESP3DValuesIndex::current_ip,
      ESP3DValuesType::string_t,
      16,
      "",
      nullptr,
  });
  return _values.size();
}