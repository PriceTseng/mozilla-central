/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MmsMessage.h"
#include "nsIDOMClassInfo.h"
#include "Constants.h"
#include "nsJSUtils.h"

DOMCI_DATA(MozMmsMessage, mozilla::dom::mms::MmsMessage)

namespace mozilla {
namespace dom {
namespace mms {

NS_INTERFACE_MAP_BEGIN(MmsMessage)
  NS_INTERFACE_MAP_ENTRY(nsIDOMMozMmsMessage)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
  NS_DOM_INTERFACE_MAP_ENTRY_CLASSINFO(MozMmsMessage)
NS_INTERFACE_MAP_END

NS_IMPL_ADDREF(MmsMessage)
NS_IMPL_RELEASE(MmsMessage)

/* static */ nsresult
MmsMessage::NewMmsMessage(nsISupports* *aNewObject)
{
  NS_ADDREF(*aNewObject = new MmsMessage());
  return NS_OK;
}

MmsMessage::MmsMessage()
{
  mData.delivery() = eDeliveryState_Unknown;
}

MmsMessage::MmsMessage(const MmsMessageData& aData)
  : mData(aData)
{
}

NS_IMETHODIMP
MmsMessage::GetId(PRInt32* aId)
{
  *aId = mData.id();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetDelivery(nsAString& aDelivery)
{
  switch (mData.delivery()) {
    case eDeliveryState_Received:
      aDelivery = DELIVERY_RECEIVED;
      break;
    case eDeliveryState_Sent:
      aDelivery = DELIVERY_SENT;
      break;
    case eDeliveryState_Sending:
      aDelivery = DELIVERY_SENDING;
      break;
    case eDeliveryState_Draft:
      aDelivery = DELIVERY_DRAFT;
      break;
    case eDeliveryState_Unknown:
    case eDeliveryState_EndGuard:
    default:
      NS_ASSERTION(true, "We shouldn't get any other delivery state!");
      return NS_ERROR_UNEXPECTED;
  }

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetFrom(nsAString& aFrom)
{
  aFrom = mData.from();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetRecipients(JSContext* aCx,
                          JS::Value* aRecipients)
{
  PRUint32 toLength = mData.to().Length();
  PRUint32 ccLength = mData.cc().Length();
  PRUint32 bccLength = mData.bcc().Length();
  PRUint32 length = toLength + ccLength + bccLength;

  if (length == 0) {
    *aRecipients = JSVAL_NULL;
    return NS_OK;
  }

  jsval* recipients = new jsval[length];
  PRUint32 r = 0;
  for (PRUint32 i = 0; i < toLength; ++i, ++r) {
    recipients[r].setString(JS_NewUCStringCopyN(aCx, mData.to()[i].get(),
                                                mData.to()[i].Length()));
  }
  for (PRUint32 j = 0; j < ccLength; ++j, ++r) {
    recipients[r].setString(JS_NewUCStringCopyN(aCx, mData.cc()[j].get(),
                                                mData.cc()[j].Length()));
  }
  for (PRUint32 k = 0; k < bccLength; ++k, ++r) {
    recipients[r].setString(JS_NewUCStringCopyN(aCx, mData.bcc()[k].get(),
                                                mData.bcc()[k].Length()));
  }

  aRecipients->setObjectOrNull(JS_NewArrayObject(aCx, length, recipients));
  NS_ENSURE_TRUE(aRecipients->isObject(), NS_ERROR_FAILURE);

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetTo(JSContext* aCx,
                  JS::Value* aTo)
{
  PRUint32 length = mData.to().Length();

  if (length == 0) {
    *aTo = JSVAL_NULL;
    return NS_OK;
  }

  jsval* to = new jsval[length];

  for (PRUint32 i = 0; i < length; ++i) {
    to[i].setString(JS_NewUCStringCopyN(aCx, mData.to()[i].get(),
                                        mData.to()[i].Length()));
  }

  aTo->setObjectOrNull(JS_NewArrayObject(aCx, length, to));
  NS_ENSURE_TRUE(aTo->isObject(), NS_ERROR_FAILURE);

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetTo(JSContext* aCx,
                  JS::Value const &aTo)
{
  if (aTo == JSVAL_NULL) {
    mData.to().Clear();
    return NS_OK;
  }

  nsTArray<nsString> to;

  if (aTo.isString()) {
    nsDependentJSString number;
    number.init(aCx, aTo.toString());

    to.AppendElement(number);
  } else if (aTo.isObject()) {
    JSObject& obj = aTo.toObject();
    if (!JS_IsArrayObject(aCx, &obj)) {
      return NS_ERROR_INVALID_ARG;
    }

    uint32_t size;
    JS_ALWAYS_TRUE(JS_GetArrayLength(aCx, &obj, &size));

    for (uint32_t i = 0; i < size; ++i) {
      jsval jsNumber;
      if (!JS_GetElement(aCx, &obj, i, &jsNumber)) {
        return NS_ERROR_INVALID_ARG;
      }

      if (!jsNumber.isString()) {
        return NS_ERROR_INVALID_ARG;
      }

      nsDependentJSString number;
      number.init(aCx, jsNumber.toString());

      to.AppendElement(number);
    }
  } else {
    return NS_ERROR_INVALID_ARG;
  }

  mData.to().Clear();
  mData.to().AppendElements(to);

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetCc(JSContext* aCx,
                  JS::Value* aCc)
{
  PRUint32 length = mData.cc().Length();

  if (length == 0) {
    *aCc = JSVAL_NULL;
    return NS_OK;
  }

  jsval* cc = new jsval[length];

  for (PRUint32 i = 0; i < length; ++i) {
    cc[i].setString(JS_NewUCStringCopyN(aCx, mData.cc()[i].get(),
                                        mData.cc()[i].Length()));
  }

  aCc->setObjectOrNull(JS_NewArrayObject(aCx, length, cc));
  NS_ENSURE_TRUE(aCc->isObject(), NS_ERROR_FAILURE);

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetCc(JSContext* aCx,
                  JS::Value const &aCc)
{
  if (aCc == JSVAL_NULL) {
    mData.cc().Clear();
    return NS_OK;
  }

  nsTArray<nsString> cc;

  if (aCc.isString()) {
    nsDependentJSString number;
    number.init(aCx, aCc.toString());

    cc.AppendElement(number);
  } else if (aCc.isObject()) {
    JSObject& obj = aCc.toObject();
    if (!JS_IsArrayObject(aCx, &obj)) {
      return NS_ERROR_INVALID_ARG;
    }

    uint32_t size;
    JS_ALWAYS_TRUE(JS_GetArrayLength(aCx, &obj, &size));

    for (uint32_t i = 0; i < size; ++i) {
      jsval jsNumber;
      if (!JS_GetElement(aCx, &obj, i, &jsNumber)) {
        return NS_ERROR_INVALID_ARG;
      }

      if (!jsNumber.isString()) {
        return NS_ERROR_INVALID_ARG;
      }

      nsDependentJSString number;
      number.init(aCx, jsNumber.toString());

      cc.AppendElement(number);
    }
  } else {
    return NS_ERROR_INVALID_ARG;
  }

  mData.cc().Clear();
  mData.cc().AppendElements(cc);

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetBcc(JSContext* aCx,
                   JS::Value* aBcc)
{
  PRUint32 length = mData.bcc().Length();

  if (length == 0) {
    *aBcc = JSVAL_NULL;
    return NS_OK;
  }

  jsval* bcc = new jsval[length];

  for (PRUint32 i = 0; i < length; ++i) {
    bcc[i].setString(JS_NewUCStringCopyN(aCx, mData.bcc()[i].get(),
                                        mData.bcc()[i].Length()));
  }

  aBcc->setObjectOrNull(JS_NewArrayObject(aCx, length, bcc));
  NS_ENSURE_TRUE(aBcc->isObject(), NS_ERROR_FAILURE);

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetBcc(JSContext* aCx,
                   JS::Value const &aBcc)
{
  if (aBcc == JSVAL_NULL) {
    mData.bcc().Clear();
    return NS_OK;
  }

  nsTArray<nsString> bcc;

  if (aBcc.isString()) {
    nsDependentJSString number;
    number.init(aCx, aBcc.toString());

    bcc.AppendElement(number);
  } else if (aBcc.isObject()) {
    JSObject& obj = aBcc.toObject();
    if (!JS_IsArrayObject(aCx, &obj)) {
      return NS_ERROR_INVALID_ARG;
    }

    uint32_t size;
    JS_ALWAYS_TRUE(JS_GetArrayLength(aCx, &obj, &size));

    for (uint32_t i = 0; i < size; ++i) {
      jsval jsNumber;
      if (!JS_GetElement(aCx, &obj, i, &jsNumber)) {
        return NS_ERROR_INVALID_ARG;
      }

      if (!jsNumber.isString()) {
        return NS_ERROR_INVALID_ARG;
      }

      nsDependentJSString number;
      number.init(aCx, jsNumber.toString());

      bcc.AppendElement(number);
    }
  } else {
    return NS_ERROR_INVALID_ARG;
  }

  mData.bcc().Clear();
  mData.bcc().AppendElements(bcc);

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetDate(JSContext* aCx,
                    JS::Value* aDate)
{
  if (mData.date() == 0) {
    *aDate = JSVAL_NULL;
    return NS_OK;
  }

  *aDate = OBJECT_TO_JSVAL(JS_NewDateObjectMsec(aCx, mData.date()));
  NS_ENSURE_TRUE(aDate->isObject(), NS_ERROR_FAILURE);

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetExpiry(JSContext* aCx,
                      JS::Value* aExpiry)
{
  const ExpiryValue &ev = mData.expiry();

  if (ev.isAbsolute()) {
    *aExpiry = OBJECT_TO_JSVAL(JS_NewDateObjectMsec(aCx, ev.timestamp()));
    NS_ENSURE_TRUE(aExpiry->isObject(), NS_ERROR_FAILURE);
  } else {
    if (!JS_NewNumberValue(aCx, ev.timestamp(), aExpiry)) {
      return NS_ERROR_FAILURE;
    }
  }

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetExpiry(JSContext* aCx,
                      JS::Value const &aExpiry)
{
  ExpiryValue &ev = mData.expiry();

  if (aExpiry == JSVAL_NULL) {
    ev.isAbsolute() = true;
    ev.timestamp() = 0;
    return NS_OK;
  }

  if (aExpiry.isObject()) {
    JSObject& obj = aExpiry.toObject();
    if (!JS_ObjectIsDate(aCx, &obj)) {
      return NS_ERROR_INVALID_ARG;
    }

    ev.isAbsolute() = true;
    ev.timestamp() = js_DateGetMsecSinceEpoch(aCx, &obj);
    return NS_OK;
  }

  if (aExpiry.isNumber()) {
    ev.isAbsolute() = false;
    ev.timestamp() = aExpiry.toNumber();
    return NS_OK;
  }

  return NS_ERROR_INVALID_ARG;
}

NS_IMETHODIMP
MmsMessage::GetDeliveryTime(JSContext* aCx,
                            JS::Value* aDeliveryTime)
{
  const ExpiryValue &ev = mData.deliveryTime();

  if (ev.isAbsolute()) {
    *aDeliveryTime = OBJECT_TO_JSVAL(JS_NewDateObjectMsec(aCx, ev.timestamp()));
    NS_ENSURE_TRUE(aDeliveryTime->isObject(), NS_ERROR_FAILURE);
  } else {
    if (!JS_NewNumberValue(aCx, ev.timestamp(), aDeliveryTime)) {
      return NS_ERROR_FAILURE;
    }
  }

  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetDeliveryTime(JSContext* aCx,
                            JS::Value const &aDeliveryTime)
{
  ExpiryValue &ev = mData.deliveryTime();

  if (aDeliveryTime == JSVAL_NULL) {
    ev.isAbsolute() = true;
    ev.timestamp() = 0;
    return NS_OK;
  }

  if (aDeliveryTime.isObject()) {
    JSObject& obj = aDeliveryTime.toObject();
    if (!JS_ObjectIsDate(aCx, &obj)) {
      return NS_ERROR_INVALID_ARG;
    }

    ev.isAbsolute() = true;
    ev.timestamp() = js_DateGetMsecSinceEpoch(aCx, &obj);
    return NS_OK;
  }

  if (aDeliveryTime.isNumber()) {
    ev.isAbsolute() = false;
    ev.timestamp() = aDeliveryTime.toNumber();
    return NS_OK;
  }

  return NS_ERROR_INVALID_ARG;
}

NS_IMETHODIMP
MmsMessage::GetSubject(nsAString& aSubject)
{
  aSubject = mData.subject();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetSubject(nsAString const &aSubject)
{
  mData.subject() = aSubject;
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetMessageClass(nsAString& aMessageClass)
{
  aMessageClass = mData.messageClass();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetMessageClass(nsAString const &aMessageClass)
{
  mData.messageClass() = aMessageClass;
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetPriority(unsigned short *aPriority)
{
  *aPriority = mData.priority();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetPriority(unsigned short aPriority)
{
  if (aPriority > PRIORITY_HIGH) {
    return NS_ERROR_INVALID_ARG;
  }

  mData.priority() = aPriority;
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetDeliveryReport(bool *aDeliveryReport)
{
  *aDeliveryReport = mData.deliveryReport();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetDeliveryReport(bool aDeliveryReport)
{
  mData.deliveryReport() = aDeliveryReport;
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetReadReport(bool *aReadReport)
{
  *aReadReport = mData.readReport();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetReadReport(bool aReadReport)
{
  mData.readReport() = aReadReport;
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetAdaptationAllowed(bool *aAdaptationAllowed)
{
  *aAdaptationAllowed = mData.adaptationAllowed();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetAdaptationAllowed(bool aAdaptationAllowed)
{
  mData.adaptationAllowed() = aAdaptationAllowed;
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetDrmContent(bool *aDrmContent)
{
  *aDrmContent = mData.drmContent();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::SetDrmContent(bool aDrmContent)
{
  mData.drmContent() = aDrmContent;
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetRead(bool *aRead)
{
  *aRead = mData.read();
  return NS_OK;
}

NS_IMETHODIMP
MmsMessage::GetContentDocument(nsIDOMDocument **aContentDocument)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
MmsMessage::SetContentDocument(nsIDOMDocument *aContentDocument)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
MmsMessage::GetAttachments(nsIDOMMozMmsAttachmentStorage **aAttachments)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

} // namespace mms
} // namespace dom
} // namespace mozilla
