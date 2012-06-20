/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsDeliveryEventData_h
#define mozilla_dom_mms_MmsDeliveryEventData_h

#include "mozilla/dom/mms/PMms.h"
#include "nsIMmsDeliveryEventData.h"
#include "Types.h"
#include "mozilla/Attributes.h"

namespace mozilla {
namespace dom {
namespace mms {

class MmsDeliveryEventData MOZ_FINAL : public nsIMmsDeliveryEventData
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIMMSDELIVERYEVENTDATA

  MmsDeliveryEventData() {}
  MmsDeliveryEventData(const MmsMessageData& aMessageData,
                       const nsAString& aOriginator);

private:
  nsCOMPtr<nsIDOMMozMmsMessage> mMessage;
  nsString mOriginator;
};

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsDeliveryEventData_h
