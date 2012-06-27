/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsAttachment_h
#define mozilla_dom_mms_MmsAttachment_h

#include "mozilla/dom/mms/PMms.h"
#include "nsIDOMMmsAttachment.h"

namespace mozilla {
namespace dom {
namespace mms {

class MmsAttachment : public nsIDOMMozMmsAttachment
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMMOZMMSATTACHMENT

  static nsresult
  NewMmsAttachment(nsISupports* *aNewObject);

  MmsAttachment();
  MmsAttachment(const Attachment& aData);

  const Attachment& GetData() const;

private:
  Attachment mData;
};

inline const Attachment&
MmsAttachment::GetData() const
{
  return mData;
}

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsAttachment_h
