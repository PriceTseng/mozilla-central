/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsEvent_h
#define mozilla_dom_mms_MmsEvent_h

#include "nsIDOMMmsEvent.h"
#include "nsDOMEvent.h"

class nsIDOMMozMmsMessage;

namespace mozilla {
namespace dom {
namespace mms {

class MmsEvent : public nsIDOMMozMmsEvent
               , public nsDOMEvent
{
public:
  MmsEvent(nsPresContext* aPresContext, nsEvent* aEvent)
    : nsDOMEvent(aPresContext, aEvent)
    , mMessage(nsnull)
  {}

  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_NSIDOMMOZMMSEVENT

  NS_FORWARD_TO_NSDOMEVENT

  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(MmsEvent, nsDOMEvent)

  nsresult Init(const nsAString & aEventTypeArg,
                bool aCanBubbleArg,
                bool aCancelableArg,
                nsIDOMMozMmsMessage* aMessage);

private:
  nsCOMPtr<nsIDOMMozMmsMessage> mMessage;
};

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsEvent_h
