/*
  esp3d_commands member
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

#include "esp3d_commands.h"
#include "esp3d_client.h"
#include "esp3d_version.h"
#include "esp3d_string.h"
#include <stdio.h>
#include <string>
#include <cstring>
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_spi_flash.h"
#include "authentication/esp3d_authentication.h"
#include "filesystem/esp3d_flash.h"
#include "network/esp3d_network.h"
#define COMMAND_ID 410
#define MAX_SCAN_LIST_SIZE 15
//Get available AP list
//output is JSON or plain text according parameter
//[ESP410]json=<no>
void Esp3DCommands::ESP410(int cmd_params_pos,esp3d_msg_t * msg)
{
    esp3d_clients_t target = msg->origin;
    esp3d_request_t requestId = msg->requestId;
    msg->target = target;
    msg->origin = ESP3D_COMMAND;
    bool json = hasTag (msg,cmd_params_pos,"json");
    std::string tmpstr;

#if ESP3D_AUTHENTICATION_FEATURE
    if (msg->authentication_level == ESP3D_LEVEL_GUEST) {
        dispatchAuthenticationError(msg, COMMAND_ID, json);
        return;
    }
#endif //ESP3D_AUTHENTICATION_FEATURE

    if (esp3dNetworkService.getMode() == esp3d_radio_off || esp3dNetworkService.getMode() == esp3d_bluetooth_serial) {
        tmpstr= "Network not enabled";
        dispatchAnswer(msg, COMMAND_ID, json,true,tmpstr.c_str());
        return;
    }

    if (json) {
        tmpstr = "{\"cmd\":\"410\",\"status\":\"ok\",\"data\":[";

    } else {
        tmpstr = "Start Scan\n";
    }

    if(!dispatch(msg,tmpstr.c_str())) {
        esp3d_log_e("Error sending response to clients");
        return;
    }

    if (esp3dNetworkService.getMode() !=esp3d_wifi_sta) {
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    }
    esp_err_t res = esp_wifi_scan_start(NULL, true);

    if (res== ESP_OK) {
        uint16_t number = MAX_SCAN_LIST_SIZE;
        wifi_ap_record_t ap_info[MAX_SCAN_LIST_SIZE];
        uint16_t ap_count = 0;
        memset(ap_info, 0, sizeof(ap_info));
        uint16_t real_count = 0;
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
        esp3d_log("Total : %d", ap_count);

        for (int i = 0; (i < MAX_SCAN_LIST_SIZE) && (i < ap_count); i++) {
            esp3d_log ("%s %d %d", ap_info[i].ssid, ap_info[i].rssi, ap_info[i].authmode);
            tmpstr="";
            int32_t signal = esp3dNetworkService.getSignal (ap_info[i].rssi, false);
            if (signal !=0) {
                if (json) {
                    if (real_count>0) {
                        tmpstr+=",";
                        real_count++;
                    }
                    tmpstr+="{\"SSID\":\"";
                }
                tmpstr += (char *)(ap_info[i].ssid);
                if (json) {
                    tmpstr+="\",\"SIGNAL\":\"";
                } else {
                    tmpstr+="\t";
                }
                tmpstr  += std::to_string(signal);
                if (json) {
                    tmpstr+="\",\"IS_PROTECTED\":\"";
                    if (ap_info[i].authmode!=WIFI_AUTH_OPEN) {
                        tmpstr+="1\"}";
                    } else {
                        tmpstr+="0\"}";
                    }
                } else {
                    tmpstr+="%\t";
                    if (ap_info[i].authmode!=WIFI_AUTH_OPEN) {
                        tmpstr+="Secure\n";
                    } else {
                        tmpstr+="Open\n";
                    }
                }
                if(!dispatch(tmpstr.c_str(),target,requestId)) {
                    esp3d_log_e("Error sending answer to clients");
                }
            }

        }
        //FixMe: esp_wifi_clear_ap_list(void);
        //Not supported in 4.4.2 but seems present in newest versions
        // esp_wifi_clear_ap_list(void);
    }

    if (esp3dNetworkService.getMode() !=esp3d_wifi_sta) {
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    }
    //end of list
    if (json) {
        tmpstr = "]}";
    } else {
        tmpstr = "End Scan\n";
    }
    if(!dispatch(tmpstr.c_str(),target,requestId)) {
        esp3d_log_e("Error sending answer to clients");
    }

}