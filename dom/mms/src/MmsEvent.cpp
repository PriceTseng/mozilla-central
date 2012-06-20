/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsEvent.h"
#include "nsIDOMClassInfo.h"
#include "nsIDOMMmsMessage.h"

DOMCI_DATA(MozMmsEvent, mozilla::dom::mms::MmsEvent)

namespace mozilla {
namespace dom {
namespace mms {

NS_IMPL_CYCLE_COLLECTION_CLASS(MmsEvent)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(MmsEvent, nsDOMEvent)
  NS_IMPL_CYCLE_COLLECTION_UNLINK_NSCOMPTR(mMessage)
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(MmsEvent, nsDOMEvent)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE_NSCOMPTR(mMessage)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_ADDREF_INHERITED(MmsEvent, nsDOMEvent)
NS_IMPL_RELEASE_INHERITED(MmsEvent, nsDOMEvent)

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION_INHERITED(MmsEvent)
  NS_INTERFACE_MAP_ENTRY(nsIDOMMozMmsEvent)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIDOMMozMmsEvent)
  NS_DOM_INTERFACE_MAP_ENTRY_CLASSINFO(MozMmsEvent)
NS_INTERFACE_MAP_END_INHERITING(nsDOMEvent)

nsresult
MmsEvent::Init(const nsAString& aEventTypeArg,
               bool aCanBubbleArg,
               bool aCancelableArg,
               nsIDOMMozMmsMessage* aMessage)
{
  nsresult rv = nsDOMEvent::InitEvent(aEventTypeArg, aCanBubbleArg,
                                      aCancelableArg);
  NS_ENSURE_SUCCESS(rv, rv);

  mMessage = aMessage;
  return NS_OK;
}

NS_IMETHODIMP
MmsEvent::GetMessageMoz(nsIDOMMozMmsMessage** aMessage)
{
  NS_IF_ADDREF(*aMessage = mMessage);
  return NS_OK;
}

} // namespace mms
} // namespace dom
} // namespace mozilla

nsresult
NS_NewDOMMmsEvent(nsIDOMEvent** aInstancePtrResult,
                  nsPresContext* aPresContext,
                  nsEvent* aEvent)
{
  return CallQueryInterface(new mozilla::dom::mms::MmsEvent(aPresContext, aEvent),
                            aInstancePtrResult);
}
