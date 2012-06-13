/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsParent_h
#define mozilla_dom_mms_MmsParent_h

#include "mozilla/dom/mms/PMmsParent.h"
#include "nsIObserver.h"

namespace mozilla {
namespace dom {
namespace mms {

class MmsParent : public PMmsParent
                , public nsIObserver
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIOBSERVER

  static void GetAll(nsTArray<MmsParent*>& aArray);

  MmsParent();

  NS_OVERRIDE virtual bool RecvHasSupport(bool* aHasSupport);

protected:
  virtual void ActorDestroy(ActorDestroyReason why);

private:
  static nsTArray<MmsParent*>* gMmsParents;
};

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsParent_h
