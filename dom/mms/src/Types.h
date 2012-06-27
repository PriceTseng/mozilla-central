/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_Types_h
#define mozilla_dom_mms_Types_h

#include "IPCMessageUtils.h"

namespace mozilla {
namespace dom {
namespace mms {

enum DeliveryState {
  eDeliveryState_Draft = 0,
  eDeliveryState_Sending,
  eDeliveryState_Sent,
  eDeliveryState_Received,
  eDeliveryState_Unknown,
  // This state should stay at the end.
  eDeliveryState_EndGuard
};

} // namespace mms
} // namespace dom
} // namespace mozilla

namespace IPC {

/**
 * Delivery state serializer.
 */
template <>
struct ParamTraits<mozilla::dom::mms::DeliveryState>
  : public EnumSerializer<mozilla::dom::mms::DeliveryState,
                          mozilla::dom::mms::eDeliveryState_Draft,
                          mozilla::dom::mms::eDeliveryState_EndGuard>
{};
/*
template <>
struct ParamTraits<mozilla::dom::mms::Attachment> {
  typedef mozilla::dom::mms::Attachment param_type;
  static void Write(Message* m, const param_type& p) {
    WriteParam(m, p.name);
    WriteParam(m, p.contentType);
    WriteParam(m, p.isString);
    WriteParam(m, p.content);
  }
  static bool Read(const Message* m, void** iter, param_type* r) {
    bool result =
      ReadParam(m, iter, &r->name) &&
      ReadParam(m, iter, &r->contentType) &&
      ReadParam(m, iter, &r->isString) &&
      ReadParam(m, iter, &r->content);
    return result;
  }
  static void Log(const param_type& p, std::wstring* l) {
  }
};
*/
} // namespace IPC

#endif // mozilla_dom_mms_Types_h
