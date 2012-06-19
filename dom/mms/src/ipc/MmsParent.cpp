/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsParent.h"
#include "nsIMmsService.h"
#include "mozilla/Services.h"
#include "mozilla/unused.h"

namespace mozilla {
namespace dom {
namespace mms {

nsTArray<MmsParent*>* MmsParent::gMmsParents = nsnull;

NS_IMPL_ISUPPORTS1(MmsParent, nsIObserver)

/* static */ void
MmsParent::GetAll(nsTArray<MmsParent*>& aArray)
{
  if (!gMmsParents) {
    aArray.Clear();
    return;
  }

  aArray = *gMmsParents;
}

MmsParent::MmsParent()
{
  if (!gMmsParents) {
    gMmsParents = new nsTArray<MmsParent*>();
  }

  gMmsParents->AppendElement(this);
}

void
MmsParent::ActorDestroy(ActorDestroyReason why)
{
  NS_ASSERTION(gMmsParents, "gMmsParents can't be null at that point!");
  gMmsParents->RemoveElement(this);
  if (gMmsParents->Length() == 0) {
    delete gMmsParents;
    gMmsParents = nsnull;
  }
}

NS_IMETHODIMP
MmsParent::Observe(nsISupports*     aSubject,
                   const char*      aTopic,
                   const PRUnichar* aData)
{
  return NS_OK;
}

bool
MmsParent::RecvHasSupport(bool* aHasSupport)
{
  *aHasSupport = false;

  nsCOMPtr<nsIMmsService> mmsService = do_GetService(MMS_SERVICE_CONTRACTID);
  NS_ENSURE_TRUE(mmsService, true);

  mmsService->HasSupport(aHasSupport);
  return true;
}

bool
MmsParent::RecvSendDummy(const MmsMessageData &aData)
{
  return true;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
