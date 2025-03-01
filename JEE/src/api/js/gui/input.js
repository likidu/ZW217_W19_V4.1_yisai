
/*
  * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
  * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
  * Copyright 2021-present Unisoc (Shanghai) Technologies Co., Ltd.
  * Licensed under the Unisoc General Software License V1;
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  * Software distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
  * See the Unisoc General Software License v1 for more details.
  *
  * Created on: 2022/06/01
  * Author: jie.yang@unisoc.com
  */

import { Component } from 'component';
import { Editbox } from 'editbox';
export function Input() {
  var _type = 'text';
  if (_type == 'text') {
    return new Editbox();
  }
}

console.log("input.js ok");