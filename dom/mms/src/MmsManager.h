/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsManager_h
#define mozilla_dom_mms_MmsManager_h

#include "nsIDOMMmsManager.h"
#include "nsDOMEventTargetHelper.h"

class nsIDOMMozMmsMessage;

namespace mozilla {
namespace dom {
namespace mms {

class MmsManager : public nsDOMEventTargetHelper
                 , public nsIDOMMozMmsManager
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMMOZMMSMANAGER

  NS_FORWARD_NSIDOMEVENTTARGET(nsDOMEventTargetHelper::)

  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(MmsManager,
                                           nsDOMEventTargetHelper)

  void Init(nsPIDOMWindow *aWindow);
  void Shutdown();
};

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsManager_h
