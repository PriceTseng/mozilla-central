/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_mms_MmsIPCService_h
#define mozilla_dom_mms_MmsIPCService_h

#include "nsIMmsService.h"

namespace mozilla {
namespace dom {
namespace mms {

class PMmsChild;

class MmsIPCService : public nsIMmsService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIMMSSERVICE

private:
  static PMmsChild* GetMmsChild();
  static PMmsChild* sMmsChild;
};

} // namespace mms
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_mms_MmsIPCService_h
