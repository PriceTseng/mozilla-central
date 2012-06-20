/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsDeliveryEvent.h"
#include "nsIDOMClassInfo.h"
#include "nsIDOMMmsMessage.h"

DOMCI_DATA(MozMmsDeliveryEvent, mozilla::dom::mms::MmsDeliveryEvent)

namespace mozilla {
namespace dom {
namespace mms {

NS_IMPL_CYCLE_COLLECTION_CLASS(MmsDeliveryEvent)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(MmsDeliveryEvent, nsDOMEvent)
  NS_IMPL_CYCLE_COLLECTION_UNLINK_NSCOMPTR(mMessage)
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(MmsDeliveryEvent, nsDOMEvent)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE_NSCOMPTR(mMessage)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_ADDREF_INHERITED(MmsDeliveryEvent, nsDOMEvent)
NS_IMPL_RELEASE_INHERITED(MmsDeliveryEvent, nsDOMEvent)

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION_INHERITED(MmsDeliveryEvent)
  NS_INTERFACE_MAP_ENTRY(nsIDOMMozMmsDeliveryEvent)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIDOMMozMmsDeliveryEvent)
  NS_DOM_INTERFACE_MAP_ENTRY_CLASSINFO(MozMmsDeliveryEvent)
NS_INTERFACE_MAP_END_INHERITING(nsDOMEvent)

nsresult
MmsDeliveryEvent::Init(const nsAString& aEventTypeArg,
                       bool aCanBubbleArg,
                       bool aCancelableArg,
                       nsIDOMMozMmsMessage* aMessage,
                       const nsAString& aOriginator)
{
  nsresult rv = nsDOMEvent::InitEvent(aEventTypeArg, aCanBubbleArg,
                                      aCancelableArg);
  NS_ENSURE_SUCCESS(rv, rv);

  mMessage = aMessage;
  mOriginator = aOriginator;
  return NS_OK;
}

NS_IMETHODIMP
MmsDeliveryEvent::GetMessageMoz(nsIDOMMozMmsMessage** aMessage)
{
  NS_IF_ADDREF(*aMessage = mMessage);
  return NS_OK;
}

NS_IMETHODIMP
MmsDeliveryEvent::GetOriginator(nsAString& aOriginator)
{
  aOriginator = mOriginator;
  return NS_OK;
}

} // namespace mms
} // namespace dom
} // namespace mozilla

nsresult
NS_NewDOMMmsDeliveryEvent(nsIDOMEvent** aInstancePtrResult,
                          nsPresContext* aPresContext,
                          nsEvent* aEvent)
{
  return CallQueryInterface(new mozilla::dom::mms::MmsDeliveryEvent(aPresContext, aEvent),
                            aInstancePtrResult);
}
