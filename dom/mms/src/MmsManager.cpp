/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsManager.h"
#include "nsIDOMClassInfo.h"
#include "nsIMmsService.h"
#include "mozilla/Services.h"
#include "nsJSUtils.h"
#include "nsContentUtils.h"
#include "nsIXPConnect.h"

DOMCI_DATA(MozMmsManager, mozilla::dom::mms::MmsManager)

namespace mozilla {
namespace dom {
namespace mms {

NS_IMPL_CYCLE_COLLECTION_CLASS(MmsManager)

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(MmsManager,
                                                  nsDOMEventTargetHelper)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(MmsManager,
                                                nsDOMEventTargetHelper)
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION_INHERITED(MmsManager)
  NS_INTERFACE_MAP_ENTRY(nsIDOMMozMmsManager)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIDOMMozMmsManager)
  NS_DOM_INTERFACE_MAP_ENTRY_CLASSINFO(MozMmsManager)
NS_INTERFACE_MAP_END_INHERITING(nsDOMEventTargetHelper)

NS_IMPL_ADDREF_INHERITED(MmsManager, nsDOMEventTargetHelper)
NS_IMPL_RELEASE_INHERITED(MmsManager, nsDOMEventTargetHelper)

void
MmsManager::Init(nsPIDOMWindow *aWindow)
{
  BindToOwner(aWindow);
}

void
MmsManager::Shutdown()
{
}

} // namespace mms
} // namespace dom
} // namespace mozilla
