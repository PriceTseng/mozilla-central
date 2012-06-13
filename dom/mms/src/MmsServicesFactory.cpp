/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsServicesFactory.h"
#include "nsXULAppAPI.h"
#ifndef MOZ_B2G_RIL
# include "MmsService.h"
#endif
#include "MmsIPCService.h"
#include "nsServiceManagerUtils.h"

#define RIL_MMS_SERVICE_CONTRACTID "@mozilla.org/mms/rilmmsservice;1"

namespace mozilla {
namespace dom {
namespace mms {

/* static */ already_AddRefed<nsIMmsService>
MmsServicesFactory::CreateMmsService()
{
  nsCOMPtr<nsIMmsService> mmsService;

  if (XRE_GetProcessType() == GeckoProcessType_Content) {
    mmsService = new MmsIPCService();
  } else {
#ifdef MOZ_B2G_RIL
    mmsService = do_GetService(RIL_MMS_SERVICE_CONTRACTID);
#else
    mmsService = new MmsService();
#endif
  }
  return mmsService.forget();
}

} // namespace mms
} // namespace dom
} // namespace mozilla
