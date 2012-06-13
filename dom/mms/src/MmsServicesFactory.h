/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsServicesFactory_h
#define mozilla_dom_mms_MmsServicesFactory_h

#include "nsCOMPtr.h"

class nsIMmsService;

namespace mozilla {
namespace dom {
namespace mms {

class MmsServicesFactory
{
public:
  static already_AddRefed<nsIMmsService> CreateMmsService();
};

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsServicesFactory_h
