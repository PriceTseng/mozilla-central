/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsAttachment.h"
#include "nsIDOMClassInfo.h"
#include "nsDOMFile.h"
#include "nsJSUtils.h"
#include "nsContentUtils.h"

DOMCI_DATA(MozMmsAttachment, mozilla::dom::mms::MmsAttachment)

namespace mozilla {
namespace dom {
namespace mms {

NS_INTERFACE_MAP_BEGIN(MmsAttachment)
  NS_INTERFACE_MAP_ENTRY(nsIDOMMozMmsAttachment)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
  NS_DOM_INTERFACE_MAP_ENTRY_CLASSINFO(MozMmsAttachment)
NS_INTERFACE_MAP_END

NS_IMPL_ADDREF(MmsAttachment)
NS_IMPL_RELEASE(MmsAttachment)

/* static */ nsresult
MmsAttachment::NewMmsAttachment(nsISupports* *aNewObject)
{
  NS_ADDREF(*aNewObject = new MmsAttachment());
  return NS_OK;
}

MmsAttachment::MmsAttachment()
{
}

MmsAttachment::MmsAttachment(const Attachment& aData)
  : mData(aData)
{
}

NS_IMETHODIMP
MmsAttachment::GetName(nsAString & aName)
{
  aName = mData.name();
  return NS_OK;
}

NS_IMETHODIMP
MmsAttachment::SetName(const nsAString &aName)
{
  mData.name() = aName;
  return NS_OK;
}

NS_IMETHODIMP
MmsAttachment::GetContentType(nsAString &aContentType)
{
  aContentType = mData.contentType();
  return NS_OK;
}

NS_IMETHODIMP
MmsAttachment::SetContentType(const nsAString &aContentType)
{
  mData.contentType() = aContentType;
  return NS_OK;
}

NS_IMETHODIMP
MmsAttachment::GetContent(JSContext* aCx, JS::Value *aContent)
{
  if (mData.isString()) {
    JSString* jsContent = JS_NewUCStringCopyN(aCx, mData.content().get(), mData.content().Length());
    if (!jsContent) {
      return NS_ERROR_FAILURE;
    }

    *aContent = STRING_TO_JSVAL(jsContent);
  }
  else {
    nsCOMPtr<nsIFile> file;
    nsresult rv = NS_NewLocalFile(mData.content(), false, getter_AddRefs(file));
    if (file) {
      nsCOMPtr<nsIDOMBlob> domBlob = new nsDOMFileFile(file);

      JSObject* scope = JS_GetGlobalForScopeChain(aCx);
      nsContentUtils::WrapNative(aCx, scope, domBlob, aContent,
                                 nsnull, true);
      NS_ENSURE_TRUE(aContent->isObject(), NS_ERROR_FAILURE);
    } else {
      return NS_ERROR_FAILURE;
    }
  }

  return NS_OK;
}

NS_IMETHODIMP
MmsAttachment::SetContent(JSContext* aCx, const JS::Value &aContent)
{
  if (aContent == JSVAL_NULL) {
    return NS_ERROR_FAILURE;
  }

  if (aContent.isString()) {
    mData.isString() = true;

    nsDependentJSString content;
    content.init(aCx, aContent.toString());
    mData.content() = content;   
  }
  else if (aContent.isObject()) {
    mData.isString() = false;

    nsCOMPtr<nsIDOMFile> file = do_QueryInterface(
      nsContentUtils::XPConnect()->GetNativeOfWrapper(aCx, &aContent.toObject()));
    if (!file) {
      return NS_ERROR_INVALID_ARG;
    }

    file->GetName(mData.content());
  } else {
    return NS_ERROR_INVALID_ARG;
  }

  return NS_OK;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
