/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsAttachmentStorage.h"
#include "nsIDOMClassInfo.h"
#include "nsIDOMMmsAttachment.h"

DOMCI_DATA(MozMmsAttachmentStorage, mozilla::dom::mms::MmsAttachmentStorage)

namespace mozilla {
namespace dom {
namespace mms {

NS_INTERFACE_MAP_BEGIN(MmsAttachmentStorage)
  NS_INTERFACE_MAP_ENTRY(nsIDOMMozMmsAttachmentStorage)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
  NS_DOM_INTERFACE_MAP_ENTRY_CLASSINFO(MozMmsAttachmentStorage)
NS_INTERFACE_MAP_END

NS_IMPL_ADDREF(MmsAttachmentStorage)
NS_IMPL_RELEASE(MmsAttachmentStorage)

MmsAttachmentStorage::MmsAttachmentStorage(int a)
{
}

NS_IMETHODIMP
MmsAttachmentStorage::GetAttachment(nsAString const &aName,
                                    nsIDOMMozMmsAttachment** aAttachment)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
MmsAttachmentStorage::SetAttachment(nsAString const &aName,
                                    nsIDOMMozMmsAttachment* aAttachment)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
MmsAttachmentStorage::DeleteAttachment(nsAString const &aName)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
