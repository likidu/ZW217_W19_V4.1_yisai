/**
 * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
 * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
 *
 * Copyright 2022-present Unisoc (Shanghai) Technologies Co., Ltd.
 * Licensed under the Unisoc General Software License V1;
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 * See the Unisoc General Software License v1 for more details.
 *
 **/

 /**
  *****************************************************************************
  * @file     js_dashboard.c
  * @brief    -
  * @author   qingjun.yu@unisoc.com
  * @version  V1.0.0
  * @date
  * @history
  * @note
  * detailed description:
  *****************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2022 unisoc </center></h2>
  **/

#include "sci_types.h"
#include "js_dashboard.h"

DECLARE_PUBLIC_HANDLER(js_dashboard_create)
{
    JERRY_CHECK_ARGS(6, object, number, number, number, number, number);
    int btn_handle = NULL;
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    int page_handle = jerry_get_number_value(args_p[1]);
    int x = jerry_get_number_value(args_p[2]);
    int y = jerry_get_number_value(args_p[3]);
    int width = jerry_get_number_value(args_p[4]);
    int height = jerry_get_number_value(args_p[5]);

    btn_handle =
        js_native_dashboard_create(obj, page_handle, x, y, width, height);
    return jerry_create_number(btn_handle);
}

DECLARE_PUBLIC_HANDLER(js_dashboard_delete)
{
    JERRY_CHECK_ARGS(1, number);
    int btn_handle = jerry_get_number_value(args_p[0]);
    js_native_dashboard_destory(btn_handle);

    return jerry_create_null();
}
