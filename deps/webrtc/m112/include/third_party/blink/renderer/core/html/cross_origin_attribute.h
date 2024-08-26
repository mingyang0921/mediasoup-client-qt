// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_HTML_CROSS_ORIGIN_ATTRIBUTE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_HTML_CROSS_ORIGIN_ATTRIBUTE_H_

#include "third_party/blink/renderer/platform/loader/fetch/cross_origin_attribute_value.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

CrossOriginAttributeValue GetCrossOriginAttributeValue(const String& value);

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_HTML_CROSS_ORIGIN_ATTRIBUTE_H_