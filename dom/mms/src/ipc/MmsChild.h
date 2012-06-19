/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsChild_h
#define mozilla_dom_mms_MmsChild_h

#include "mozilla/dom/mms/PMmsChild.h"

namespace mozilla {
namespace dom {
namespace mms {

class MmsChild : public PMmsChild
{
public:
  NS_OVERRIDE virtual bool RecvNotifyDummy(const MmsMessageData &aData);
};

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsChild_h
