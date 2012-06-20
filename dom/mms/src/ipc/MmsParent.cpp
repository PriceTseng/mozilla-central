/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsParent.h"
#include "nsIMmsService.h"
#include "nsIObserverService.h"
#include "mozilla/Services.h"
#include "Constants.h"
#include "nsIDOMMmsMessage.h"
#include "nsIMmsDeliveryEventData.h"
#include "mozilla/unused.h"
#include "MmsMessage.h"
#include "MmsDeliveryEventData.h"

namespace mozilla {
namespace dom {
namespace mms {

nsTArray<MmsParent*>* MmsParent::gMmsParents = nsnull;

NS_IMPL_ISUPPORTS1(MmsParent, nsIObserver)

/* static */ void
MmsParent::GetAll(nsTArray<MmsParent*>& aArray)
{
  if (!gMmsParents) {
    aArray.Clear();
    return;
  }

  aArray = *gMmsParents;
}

MmsParent::MmsParent()
{
  if (!gMmsParents) {
    gMmsParents = new nsTArray<MmsParent*>();
  }

  gMmsParents->AppendElement(this);

  nsCOMPtr<nsIObserverService> obs = services::GetObserverService();
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
MmsParent::ActorDestroy(ActorDestroyReason why)
{
  nsCOMPtr<nsIObserverService> obs = services::GetObserverService();
  if (!obs) {
    return;
  }

  obs->RemoveObserver(this, kMmsReceivedObserverTopic);
  obs->RemoveObserver(this, kMmsSentObserverTopic);
  obs->RemoveObserver(this, kMmsDeliveredObserverTopic);
  obs->RemoveObserver(this, kMmsReadObserverTopic);
  obs->RemoveObserver(this, kMmsCancelledObserverTopic);

  NS_ASSERTION(gMmsParents, "gMmsParents can't be null at that point!");
  gMmsParents->RemoveElement(this);
  if (gMmsParents->Length() == 0) {
    delete gMmsParents;
    gMmsParents = nsnull;
  }
}

NS_IMETHODIMP
MmsParent::Observe(nsISupports*     aSubject,
                   const char*      aTopic,
                   const PRUnichar* aData)
{
  if (!strcmp(aTopic, kMmsReceivedObserverTopic)) {
    nsCOMPtr<nsIDOMMozMmsMessage> message = do_QueryInterface(aSubject);
    if (!message) {
      NS_ERROR("Got a 'mms-received' topic without a valid message!");
      return NS_OK;
    }

    unused << SendNotifyReceivedMessage(static_cast<MmsMessage*>(message.get())->GetData());
    return NS_OK;
  }

  if (!strcmp(aTopic, kMmsSentObserverTopic)) {
    nsCOMPtr<nsIDOMMozMmsMessage> message = do_QueryInterface(aSubject);
    if (!message) {
      NS_ERROR("Got a 'mms-sent' topic without a valid message!");
      return NS_OK;
    }

    unused << SendNotifySentMessage(static_cast<MmsMessage*>(message.get())->GetData());
    return NS_OK;
  }

  if (!strcmp(aTopic, kMmsDeliveredObserverTopic)) {
    nsCOMPtr<nsIMmsDeliveryEventData> data = do_QueryInterface(aSubject);
    if (!data) {
      NS_ERROR("Got a 'mms-delivered' topic without a valid data!");
      return NS_OK;
    }

    nsIDOMMozMmsMessage *message;
    nsString originator;

    NS_ENSURE_SUCCESS(data->GetMessageMoz(&message), NS_OK);
    NS_ENSURE_SUCCESS(data->GetOriginator(originator), NS_OK);

    unused << SendNotifyDeliveredMessage(static_cast<MmsMessage*>(message)->GetData(), originator);
    return NS_OK;
  }

  if (!strcmp(aTopic, kMmsReadObserverTopic)) {
    nsCOMPtr<nsIMmsDeliveryEventData> data = do_QueryInterface(aSubject);
    if (!data) {
      NS_ERROR("Got a 'mms-read' topic without a valid data!");
      return NS_OK;
    }

    nsIDOMMozMmsMessage *message;
    nsString originator;

    NS_ENSURE_SUCCESS(data->GetMessageMoz(&message), NS_OK);
    NS_ENSURE_SUCCESS(data->GetOriginator(originator), NS_OK);

    unused << SendNotifyReadMessage(static_cast<MmsMessage*>(message)->GetData(), originator);
    return NS_OK;
  }

  if (!strcmp(aTopic, kMmsCancelledObserverTopic)) {
    nsCOMPtr<nsIDOMMozMmsMessage> message = do_QueryInterface(aSubject);
    if (!message) {
      NS_ERROR("Got a 'mms-cancelled' topic without a valid message!");
      return NS_OK;
    }

    unused << SendNotifyCancelledMessage(static_cast<MmsMessage*>(message.get())->GetData());
    return NS_OK;
  }

  return NS_OK;
}

bool
MmsParent::RecvHasSupport(bool* aHasSupport)
{
  *aHasSupport = false;

  nsCOMPtr<nsIMmsService> mmsService = do_GetService(MMS_SERVICE_CONTRACTID);
  NS_ENSURE_TRUE(mmsService, true);

  mmsService->HasSupport(aHasSupport);
  return true;
}

bool
MmsParent::RecvSendDummy(const MmsMessageData &aData)
{
  return true;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
