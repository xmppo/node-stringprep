#include <nan.h>
#include <unicode/unistr.h>
#include <unicode/usprep.h>
#include <unicode/uidna.h>
#include <cstring>
#include <exception>

using namespace v8;
using namespace node;

/* supports return of just enum */
class UnknownProfileException : public std::exception {
};

// protect constructor from GC
static Nan::Persistent<FunctionTemplate> stringprep_constructor;

class StringPrep : public Nan::ObjectWrap {
public:
  static void Initialize(Handle<Object> target)
  {
    Nan::HandleScope scope;
    Local<FunctionTemplate> t = Nan::New<FunctionTemplate>(New);
    stringprep_constructor.Reset(t);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    Nan::SetPrototypeMethod(t, "prepare", Prepare);

    target->Set(Nan::New<String>("StringPrep").ToLocalChecked(), t->GetFunction());
  }

  bool good() const
  {
    return U_SUCCESS(error);
  }

  const char *errorName() const
  {
    return u_errorName(error);
  }

protected:
  /*** Constructor ***/

  static NAN_METHOD(New)
  {
    if (info.Length() >= 1 && info[0]->IsString())
      {
        Nan::Utf8String arg0(info[0]->ToString());
        UStringPrepProfileType profileType;
        try
          {
            profileType = parseProfileType(arg0);
          }
        catch (UnknownProfileException &)
          {
            Nan::ThrowTypeError("Unknown StringPrep profile");
            return;
          }

        StringPrep *self = new StringPrep(profileType);
        if (self->good())
          {
            self->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
            return;
          }
        else
          {
            const char* err = self->errorName();
            delete self;
            Nan::ThrowError(err);
            return;
          }
      }
    else {
      Nan::ThrowTypeError("Bad argument.");
      return;
    }
  }

  StringPrep(const UStringPrepProfileType profileType)
    : error(U_ZERO_ERROR)
  {
    profile = usprep_openByType(profileType, &error);
  }

  /*** Destructor ***/

  ~StringPrep()
  {
    if (profile)
      usprep_close(profile);
  }

  /*** Prepare ***/

  static NAN_METHOD(Prepare)
  {
    if (info.Length() >= 1 && info[0]->IsString())
      {
        StringPrep *self = Nan::ObjectWrap::Unwrap<StringPrep>(info.This());
        String::Value arg0(info[0]->ToString());
        info.GetReturnValue().Set(self->prepare(arg0));
        return;
      }
    else {
      Nan::ThrowTypeError("Bad argument.");
      return;
    }
  }

  Local<Value> prepare(String::Value &str)
  {
    Nan::EscapableHandleScope scope;
    size_t destLen = str.length() + 1;
    UChar *dest = NULL;
    while(!dest)
      {
        error = U_ZERO_ERROR;
        dest = new UChar[destLen];
        size_t w = usprep_prepare(profile,
                                  *str, str.length(),
                                  dest, destLen,
                                  USPREP_DEFAULT, NULL, &error);

        if (error == U_BUFFER_OVERFLOW_ERROR)
          {
            // retry with a dest buffer twice as large
            destLen *= 2;
            delete[] dest;
            dest = NULL;
          }
        else if (!good())
          {
            // other error, just bail out
            delete[] dest;
            Nan::ThrowError(errorName());
            return scope.Escape(Nan::Undefined());
          }
        else
          destLen = w;
      }

    Local<Value> result = Nan::New<String>(dest, destLen).ToLocalChecked();
    delete[] dest;
    return scope.Escape(result);
  }

private:
  UStringPrepProfile *profile;
  UErrorCode error;

  static enum UStringPrepProfileType parseProfileType(Nan::Utf8String &profile)
    throw(UnknownProfileException)
  {
    if (strcasecmp(*profile, "nameprep") == 0)
      return USPREP_RFC3491_NAMEPREP;
    if (strcasecmp(*profile, "nfs4_cs_prep") == 0)
      return USPREP_RFC3530_NFS4_CS_PREP;
    if (strcasecmp(*profile, "nfs4_cs_prep") == 0)
      return USPREP_RFC3530_NFS4_CS_PREP_CI;
    if (strcasecmp(*profile, "nfs4_cis_prep") == 0)
      return USPREP_RFC3530_NFS4_CIS_PREP;
    if (strcasecmp(*profile, "nfs4_mixed_prep prefix") == 0)
      return USPREP_RFC3530_NFS4_MIXED_PREP_PREFIX;
    if (strcasecmp(*profile, "nfs4_mixed_prep suffix") == 0)
      return USPREP_RFC3530_NFS4_MIXED_PREP_SUFFIX;
    if (strcasecmp(*profile, "iscsi") == 0)
      return USPREP_RFC3722_ISCSI;
    if (strcasecmp(*profile, "nodeprep") == 0)
      return USPREP_RFC3920_NODEPREP;
    if (strcasecmp(*profile, "resourceprep") == 0)
      return USPREP_RFC3920_RESOURCEPREP;
    if (strcasecmp(*profile, "mib") == 0)
      return USPREP_RFC4011_MIB;
    if (strcasecmp(*profile, "saslprep") == 0)
      return USPREP_RFC4013_SASLPREP;
    if (strcasecmp(*profile, "trace") == 0)
      return USPREP_RFC4505_TRACE;
    if (strcasecmp(*profile, "ldap") == 0)
      return USPREP_RFC4518_LDAP;
    if (strcasecmp(*profile, "ldapci") == 0)
      return USPREP_RFC4518_LDAP_CI;

    throw UnknownProfileException();
  }
};


/*** IDN support ***/

NAN_METHOD(ToUnicode)
{
  if (info.Length() >= 2 && info[0]->IsString() && info[1]->IsInt32())
  {
    String::Value str(info[0]->ToString());
    int32_t options = info[1]->ToInt32()->Value();
    UErrorCode error = U_ZERO_ERROR;
    UIDNA *uidna = uidna_openUTS46(options, &error);
    if (U_FAILURE(error))
      {
        Nan::ThrowError(u_errorName(error));
        return;
      }

    // ASCII encoding (xn--*--*) should be longer than Unicode
    size_t destLen = str.length() + 1;
    UChar *dest = NULL;
    while(!dest)
      {
        dest = new UChar[destLen];
        UIDNAInfo uinfo = UIDNA_INFO_INITIALIZER;
        size_t w = uidna_nameToUnicode(
          uidna,
          *str, str.length(),
          dest, destLen,
          &uinfo, &error);
        
        if (error == U_BUFFER_OVERFLOW_ERROR)
          {
            // retry with a dest buffer twice as large
            destLen *= 2;
            delete[] dest;
            dest = NULL;
          }
        else if (U_FAILURE(error))
          {
            // other error, just bail out
            delete[] dest;
            uidna_close(uidna);
            Nan::ThrowError(u_errorName(error));
            return;
          }
        else
          destLen = w;
      }

    Local<String> result = Nan::New<String>(dest, destLen).ToLocalChecked();
    delete[] dest;
    uidna_close(uidna);
    info.GetReturnValue().Set(result);
    return;
  }
  else {
    Nan::ThrowTypeError("Bad argument.");
    return;
  }
}

NAN_METHOD(ToASCII)
{
  if (info.Length() >= 2 && info[0]->IsString() && info[1]->IsInt32())
  {
    String::Value str(info[0]->ToString());
    int32_t options = info[1]->ToInt32()->Value();
    UErrorCode error = U_ZERO_ERROR;
    UIDNA *uidna = uidna_openUTS46(options, &error);
    if (U_FAILURE(error))
      {
        Nan::ThrowError(u_errorName(error));
        return;
      }

    // find out length first
    size_t strLen = str.length();
    UIDNAInfo uinfo1 = UIDNA_INFO_INITIALIZER;
    size_t destLen = uidna_nameToASCII(
      uidna,
      *str, strLen,
      NULL, 0,
      &uinfo1, &error);
    UChar *dest = NULL;
    if (error == U_BUFFER_OVERFLOW_ERROR)
      {
        // now try with dest buffer
        error = U_ZERO_ERROR;
        dest = new UChar[destLen];
        UIDNAInfo uinfo2 = UIDNA_INFO_INITIALIZER;
        uidna_nameToASCII(
          uidna,
          *str, strLen,
          dest, destLen,
          &uinfo2, &error);
        if (U_SUCCESS(error) && uinfo2.errors > 0)
          {
            error = U_INVALID_CHAR_FOUND;
          }
      }
    if (U_FAILURE(error))
      {
        // other error, just bail out
        delete[] dest;
        uidna_close(uidna);
        Nan::ThrowError(u_errorName(error));
        return;
      }

    Local<String> result = Nan::New<String>(dest, destLen).ToLocalChecked();
    delete[] dest;
    uidna_close(uidna);
    info.GetReturnValue().Set(result);
    return;
  }
  else {
    Nan::ThrowTypeError("Bad argument.");
    return;
  }
}

/*** Initialization ***/

NAN_MODULE_INIT(init)
{
  StringPrep::Initialize(target);
  Nan::SetMethod(target, "toUnicode", ToUnicode);
  Nan::SetMethod(target, "toASCII", ToASCII);
}

NODE_MODULE(node_stringprep, init)
