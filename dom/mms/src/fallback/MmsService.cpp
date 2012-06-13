/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsService.h"
#include "jsapi.h"

namespace mozilla {
namespace dom {
namespace mms {

NS_IMPL_ISUPPORTS1(MmsService, nsIMmsService)

NS_IMETHODIMP
MmsService::HasSupport(bool* aHasSupport)
{
  *aHasSupport = false;
  return NS_OK;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
