/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsAttachmentStorage_h
#define mozilla_dom_mms_MmsAttachmentStorage_h

#include "nsIDOMMmsAttachmentStorage.h"
#include "mozilla/Attributes.h"

namespace mozilla {
namespace dom {
namespace mms {

class MmsAttachmentStorage MOZ_FINAL : public nsIDOMMozMmsAttachmentStorage
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMMOZMMSATTACHMENTSTORAGE

  MmsAttachmentStorage(int a);

private:
  MmsAttachmentStorage();
};

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsAttachmentStorage_h
