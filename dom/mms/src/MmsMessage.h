/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsMessage_h
#define mozilla_dom_mms_MmsMessage_h

#include "mozilla/dom/mms/PMms.h"
#include "nsIDOMMmsMessage.h"
#include "Types.h"
#include "mozilla/Attributes.h"

namespace mozilla {
namespace dom {
namespace mms {

class MmsMessage MOZ_FINAL : public nsIDOMMozMmsMessage
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMMOZMMSMESSAGE

  static nsresult
  NewMmsMessage(nsISupports* *aNewObject);

  MmsMessage();
  MmsMessage(const MmsMessageData& aData);

  const MmsMessageData& GetData() const;

private:
  MmsMessageData mData;
};

inline const MmsMessageData&
MmsMessage::GetData() const
{
  return mData;
}

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsMessage_h
