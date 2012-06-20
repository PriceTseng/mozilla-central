/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsChild.h"
#include "MmsMessage.h"
#include "MmsDeliveryEventData.h"
#include "Constants.h"
#include "nsIObserverService.h"
#include "mozilla/Services.h"
#include "mozilla/dom/ContentChild.h"

namespace mozilla {
namespace dom {
namespace mms {

bool
MmsChild::RecvNotifyReceivedMessage(const MmsMessageData &aData)
{
  nsCOMPtr<nsIObserverService> obs = services::GetObserverService();
  if (!obs) {
    return true;
  }

  nsCOMPtr<MmsMessage> message = new MmsMessage(aData);
  obs->NotifyObservers(message, kMmsReceivedObserverTopic, nsnull);

  return true;
}

bool
MmsChild::RecvNotifySentMessage(const MmsMessageData &aData)
{
  nsCOMPtr<nsIObserverService> obs = services::GetObserverService();
  if (!obs) {
    return true;
  }

  nsCOMPtr<MmsMessage> message = new MmsMessage(aData);
  obs->NotifyObservers(message, kMmsSentObserverTopic, nsnull);

  return true;
}

bool
MmsChild::RecvNotifyDeliveredMessage(const MmsMessageData &aData,
                                     const nsString &aOriginator)
{
  nsCOMPtr<nsIObserverService> obs = services::GetObserverService();
  if (!obs) {
    return true;
  }

  nsCOMPtr<MmsDeliveryEventData> eventData = new MmsDeliveryEventData(aData, aOriginator);
  obs->NotifyObservers(eventData, kMmsDeliveredObserverTopic, nsnull);

  return true;
}

bool
MmsChild::RecvNotifyReadMessage(const MmsMessageData &aData,
                                const nsString &aOriginator)
{
  nsCOMPtr<nsIObserverService> obs = services::GetObserverService();
  if (!obs) {
    return true;
  }

  nsCOMPtr<MmsDeliveryEventData> eventData = new MmsDeliveryEventData(aData, aOriginator);
  obs->NotifyObservers(eventData, kMmsReadObserverTopic, nsnull);

  return true;
}

bool
MmsChild::RecvNotifyCancelledMessage(const MmsMessageData &aData)
{
  nsCOMPtr<nsIObserverService> obs = services::GetObserverService();
  if (!obs) {
    return true;
  }

  nsCOMPtr<MmsMessage> message = new MmsMessage(aData);
  obs->NotifyObservers(message, kMmsCancelledObserverTopic, nsnull);

  return true;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
