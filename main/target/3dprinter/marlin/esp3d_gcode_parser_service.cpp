/*
  esp3d_gcode_parser_service
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

#include "esp3d_gcode_parser_service.h"

#include "esp3d_hal.h"
#include "esp3d_log.h"
#include "esp3d_string.h"

ESP3DGCodeParserService esp3dGcodeParser;

ESP3DGCodeParserService::ESP3DGCodeParserService() {
  _isMultiLineReportOnGoing = false;
}

ESP3DGCodeParserService::~ESP3DGCodeParserService() {
  _isMultiLineReportOnGoing = false;
}

ESP3DDataType ESP3DGCodeParserService::getType(const char* data) {
  return ESP3DDataType::unknown;
}
bool ESP3DGCodeParserService::needAck(const char* data) { return false; }