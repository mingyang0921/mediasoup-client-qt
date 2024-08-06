/*
 * Copyright (C) 2014 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_SVG_SVG_NUMBER_OPTIONAL_NUMBER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_SVG_SVG_NUMBER_OPTIONAL_NUMBER_H_

#include "third_party/blink/renderer/core/svg/svg_number.h"
#include "third_party/blink/renderer/core/svg/svg_parsing_error.h"
#include "third_party/blink/renderer/platform/heap/garbage_collected.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace blink {

class SVGNumberOptionalNumber final : public SVGPropertyBase {
 public:
  // Tearoff of SVGNumberOptionalNumber is never created.
  typedef void TearOffType;
  typedef void PrimitiveType;

  SVGNumberOptionalNumber(SVGNumber* first_number, SVGNumber* second_number);

  SVGNumberOptionalNumber* Clone() const;
  SVGPropertyBase* CloneForAnimation(const String&) const override;

  String ValueAsString() const override;
  SVGParsingError SetValueAsString(const String&);
  void SetInitial(unsigned);
  static constexpr int kInitialValueBits = SVGNumber::kInitialValueBits;

  void Add(const SVGPropertyBase*, const SVGElement*) override;
  void CalculateAnimatedValue(
      const SMILAnimationEffectParameters&,
      float percentage,
      unsigned repeat_count,
      const SVGPropertyBase* from,
      const SVGPropertyBase* to,
      const SVGPropertyBase* to_at_end_of_duration_value,
      const SVGElement* context_element) override;
  float CalculateDistance(const SVGPropertyBase* to,
                          const SVGElement* context_element) const override;

  static AnimatedPropertyType ClassType() {
    return kAnimatedNumberOptionalNumber;
  }
  AnimatedPropertyType GetType() const override { return ClassType(); }

  SVGNumber* FirstNumber() const { return first_number_; }
  SVGNumber* SecondNumber() const { return second_number_; }

  void Trace(Visitor*) const override;

 protected:
  Member<SVGNumber> first_number_;
  Member<SVGNumber> second_number_;
};

template <>
struct DowncastTraits<SVGNumberOptionalNumber> {
  static bool AllowFrom(const SVGPropertyBase& value) {
    return value.GetType() == SVGNumberOptionalNumber::ClassType();
  }
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_SVG_SVG_NUMBER_OPTIONAL_NUMBER_H_
