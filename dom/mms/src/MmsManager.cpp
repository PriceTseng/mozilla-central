/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsManager.h"
#include "nsIDOMClassInfo.h"
#include "nsIMmsService.h"
#include "nsIObserverService.h"
#include "mozilla/Services.h"
#include "Constants.h"
#include "MmsEvent.h"
#include "MmsDeliveryEvent.h"
#include "nsIDOMMmsMessage.h"
#include "nsIMmsDeliveryEventData.h"
#include "nsJSUtils.h"
#include "nsContentUtils.h"
#include "nsIXPConnect.h"

/**
 * We have to use macros here because our leak analysis tool thinks we are
 * leaking strings when we have |static const nsString|. Sad :(
 */
#define RECEIVED_EVENT_NAME  NS_LITERAL_STRING("received")
#define SENT_EVENT_NAME      NS_LITERAL_STRING("sent")
#define DELIVERED_EVENT_NAME NS_LITERAL_STRING("delivered")
#define READ_EVENT_NAME      NS_LITERAL_STRING("read")
#define CANCELLED_EVENT_NAME NS_LITERAL_STRING("cancelled")

DOMCI_DATA(MozMmsManager, mozilla::dom::mms::MmsManager)

namespace mozilla {
namespace dom {
namespace mms {

NS_IMPL_CYCLE_COLLECTION_CLASS(MmsManager)

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(MmsManager,
                                                  nsDOMEventTargetHelper)
  NS_CYCLE_COLLECTION_TRAVERSE_EVENT_HANDLER(received)
  NS_CYCLE_COLLECTION_TRAVERSE_EVENT_HANDLER(sent)
  NS_CYCLE_COLLECTION_TRAVERSE_EVENT_HANDLER(delivered)
  NS_CYCLE_COLLECTION_TRAVERSE_EVENT_HANDLER(read)
  NS_CYCLE_COLLECTION_TRAVERSE_EVENT_HANDLER(cancelled)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(MmsManager,
                                                nsDOMEventTargetHelper)
  NS_CYCLE_COLLECTION_UNLINK_EVENT_HANDLER(received)
  NS_CYCLE_COLLECTION_UNLINK_EVENT_HANDLER(sent)
  NS_CYCLE_COLLECTION_UNLINK_EVENT_HANDLER(delivered)
  NS_CYCLE_COLLECTION_UNLINK_EVENT_HANDLER(read)
  NS_CYCLE_COLLECTION_UNLINK_EVENT_HANDLER(cancelled)
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION_INHERITED(MmsManager)
  NS_INTERFACE_MAP_ENTRY(nsIDOMMozMmsManager)
  NS_INTERFACE_MAP_ENTRY(nsIObserver)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIDOMMozMmsManager)
  NS_DOM_INTERFACE_MAP_ENTRY_CLASSINFO(MozMmsManager)
NS_INTERFACE_MAP_END_INHERITING(nsDOMEventTargetHelper)

NS_IMPL_ADDREF_INHERITED(MmsManager, nsDOMEventTargetHelper)
NS_IMPL_RELEASE_INHERITED(MmsManager, nsDOMEventTargetHelper)

void
MmsManager::Init(nsPIDOMWindow *aWindow)
{
  BindToOwner(aWindow);

  nsCOMPtr<nsIObserverService> obs = services::GetObserverService();
  // GetObserverService() can return null is some situations like shutdown.
  if (!obs) {
    return;
  }

  obs->AddObserver(this, kMmsReceivedObserverTopic, false);
  obs->AddObserver(this, kMmsSentObserverTopic, false);
  obs->AddObserver(this, kMmsDeliveredObserverTopic, false);
  obs->AddObserver(this, kMmsReadObserverTopic, false);
  obs->AddObserver(this, kMmsCancelledObserverTopic, false);
}

void
MmsManager::Shutdown()
{
  nsCOMPtr<nsIObserverService> obs = services::GetObserverService();
  // GetObserverService() can return null is some situations like shutdown.
  if (!obs) {
    return;
  }

  obs->RemoveObserver(this, kMmsReceivedObserverTopic);
  obs->RemoveObserver(this, kMmsSentObserverTopic);
  obs->RemoveObserver(this, kMmsDeliveredObserverTopic);
  obs->RemoveObserver(this, kMmsReadObserverTopic);
  obs->RemoveObserver(this, kMmsCancelledObserverTopic);
}

NS_IMPL_EVENT_HANDLER(MmsManager, received)
NS_IMPL_EVENT_HANDLER(MmsManager, sent)
NS_IMPL_EVENT_HANDLER(MmsManager, delivered)
NS_IMPL_EVENT_HANDLER(MmsManager, read)
NS_IMPL_EVENT_HANDLER(MmsManager, cancelled)

nsresult
MmsManager::DispatchTrustedMmsEventToSelf(const nsAString& aEventName,
                                          nsIDOMMozMmsMessage* aMessage)
{
  nsRefPtr<nsDOMEvent> event = new MmsEvent(nsnull, nsnull);
  nsresult rv = static_cast<MmsEvent*>(event.get())->Init(aEventName, false,
                                                          false, aMessage);
  NS_ENSURE_SUCCESS(rv, rv);

  return DispatchTrustedEventToSelf(aEventName, event);
}

nsresult
MmsManager::DispatchTrustedMmsDeliveryEventToSelf(const nsAString& aEventName,
                                                  nsIMmsDeliveryEventData* aData)
{
  nsIDOMMozMmsMessage* message;
  nsString originator;

  nsresult rv = aData->GetMessageMoz(&message);
  NS_ENSURE_SUCCESS(rv, rv);
  rv = aData->GetOriginator(originator);
  NS_ENSURE_SUCCESS(rv, rv);

  nsRefPtr<nsDOMEvent> event = new MmsDeliveryEvent(nsnull, nsnull);
  rv = static_cast<MmsDeliveryEvent*>(event.get())->Init(aEventName, false,
                                                         false, message,
                                                         originator);
  NS_ENSURE_SUCCESS(rv, rv);

  return DispatchTrustedEventToSelf(aEventName, event);
}

nsresult
MmsManager::DispatchTrustedEventToSelf(const nsAString& aEventName,
                                       nsRefPtr<nsDOMEvent>& aEvent)
{
  nsresult rv = aEvent->SetTrusted(true);
  NS_ENSURE_SUCCESS(rv, rv);

  bool dummy;
  rv = DispatchEvent(aEvent, &dummy);
  NS_ENSURE_SUCCESS(rv, rv);

  return NS_OK;
}

NS_IMETHODIMP
MmsManager::Observe(nsISupports* aSubject,
                    const char* aTopic,
                    const PRUnichar* aData)
{
  if (!strcmp(aTopic, kMmsReceivedObserverTopic)) {
    nsCOMPtr<nsIDOMMozMmsMessage> message = do_QueryInterface(aSubject);
    if (!message) {
      NS_ERROR("Got a 'mms-received' topic without a valid message!");
      return NS_OK;
    }

    DispatchTrustedMmsEventToSelf(RECEIVED_EVENT_NAME, message);
    return NS_OK;
  }

  if (!strcmp(aTopic, kMmsSentObserverTopic)) {
    nsCOMPtr<nsIDOMMozMmsMessage> message = do_QueryInterface(aSubject);
    if (!message) {
      NS_ERROR("Got a 'mms-sent' topic without a valid message!");
      return NS_OK;
    }

    DispatchTrustedMmsEventToSelf(SENT_EVENT_NAME, message);
    return NS_OK;
  }

  if (!strcmp(aTopic, kMmsDeliveredObserverTopic)) {
    nsCOMPtr<nsIMmsDeliveryEventData> data = do_QueryInterface(aSubject);
    if (!data) {
      NS_ERROR("Got a 'mms-delivered' topic without a valid data!");
      return NS_OK;
    }

    DispatchTrustedMmsDeliveryEventToSelf(DELIVERED_EVENT_NAME, data);
    return NS_OK;
  }

  if (!strcmp(aTopic, kMmsReadObserverTopic)) {
    nsCOMPtr<nsIMmsDeliveryEventData> data = do_QueryInterface(aSubject);
    if (!data) {
      NS_ERROR("Got a 'mms-read' topic without a valid data!");
      return NS_OK;
    }

    DispatchTrustedMmsDeliveryEventToSelf(READ_EVENT_NAME, data);
    return NS_OK;
  }

  if (!strcmp(aTopic, kMmsCancelledObserverTopic)) {
    nsCOMPtr<nsIDOMMozMmsMessage> message = do_QueryInterface(aSubject);
    if (!message) {
      NS_ERROR("Got a 'mms-cancelled' topic without a valid message!");
      return NS_OK;
    }

    DispatchTrustedMmsEventToSelf(CANCELLED_EVENT_NAME, message);
    return NS_OK;
  }

  return NS_OK;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
