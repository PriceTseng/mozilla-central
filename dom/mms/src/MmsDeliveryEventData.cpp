/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsDeliveryEventData.h"
#include "nsIDOMClassInfo.h"
#include "MmsMessage.h"

namespace mozilla {
namespace dom {
namespace mms {

NS_IMPL_ISUPPORTS1(MmsDeliveryEventData, nsIMmsDeliveryEventData)

MmsDeliveryEventData::MmsDeliveryEventData(const MmsMessageData& aMessageData,
                                           const nsAString& aOriginator)
  : mMessage(new MmsMessage(aMessageData))
  , mOriginator(aOriginator)
{
}

NS_IMETHODIMP
MmsDeliveryEventData::GetMessageMoz(nsIDOMMozMmsMessage** aMessage)
{
  NS_IF_ADDREF(*aMessage = mMessage);
  return NS_OK;
}

NS_IMETHODIMP
MmsDeliveryEventData::SetMessageMoz(nsIDOMMozMmsMessage* aMessage)
{
  mMessage = aMessage;
  return NS_OK;
}

NS_IMETHODIMP
MmsDeliveryEventData::GetOriginator(nsAString &aOriginator)
{
  aOriginator = mOriginator;
  return NS_OK;
}

NS_IMETHODIMP
MmsDeliveryEventData::SetOriginator(nsAString const &aOriginator)
{
  mOriginator = aOriginator;
  return NS_OK;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
