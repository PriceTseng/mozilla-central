/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/ContentChild.h"
#include "MmsIPCService.h"
#include "nsXULAppAPI.h"
#include "jsapi.h"
#include "mozilla/dom/mms/MmsChild.h"

namespace mozilla {
namespace dom {
namespace mms {

PMmsChild* MmsIPCService::sMmsChild = nsnull;

NS_IMPL_ISUPPORTS1(MmsIPCService, nsIMmsService)

/* static */ PMmsChild*
MmsIPCService::GetMmsChild()
{
  if (!sMmsChild) {
    sMmsChild = ContentChild::GetSingleton()->SendPMmsConstructor();
  }

  return sMmsChild;
}

/*
 * Implementation of nsIMmsService.
 */
NS_IMETHODIMP
MmsIPCService::HasSupport(bool* aHasSupport)
{
  GetMmsChild()->SendHasSupport(aHasSupport);

  return NS_OK;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
