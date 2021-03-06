// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Register.proto

#ifndef PROTOBUF_Register_2eproto__INCLUDED
#define PROTOBUF_Register_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3003001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace Hanasu {
class RegisterRequest;
class RegisterRequestDefaultTypeInternal;
extern RegisterRequestDefaultTypeInternal _RegisterRequest_default_instance_;
class RegisterResponse;
class RegisterResponseDefaultTypeInternal;
extern RegisterResponseDefaultTypeInternal _RegisterResponse_default_instance_;
}  // namespace Hanasu

namespace Hanasu {

namespace protobuf_Register_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static void InitDefaultsImpl();
  static void Shutdown();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_Register_2eproto

enum RegisterResponse_RegisterCode {
  RegisterResponse_RegisterCode_SUCCESS = 0,
  RegisterResponse_RegisterCode_USER_NAME_EXISTS = 1,
  RegisterResponse_RegisterCode_PASSWORD_TOO_SHORT = 2,
  RegisterResponse_RegisterCode_NICK_NAME_TOO_LONG = 3,
  RegisterResponse_RegisterCode_RegisterResponse_RegisterCode_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  RegisterResponse_RegisterCode_RegisterResponse_RegisterCode_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool RegisterResponse_RegisterCode_IsValid(int value);
const RegisterResponse_RegisterCode RegisterResponse_RegisterCode_RegisterCode_MIN = RegisterResponse_RegisterCode_SUCCESS;
const RegisterResponse_RegisterCode RegisterResponse_RegisterCode_RegisterCode_MAX = RegisterResponse_RegisterCode_NICK_NAME_TOO_LONG;
const int RegisterResponse_RegisterCode_RegisterCode_ARRAYSIZE = RegisterResponse_RegisterCode_RegisterCode_MAX + 1;

const ::google::protobuf::EnumDescriptor* RegisterResponse_RegisterCode_descriptor();
inline const ::std::string& RegisterResponse_RegisterCode_Name(RegisterResponse_RegisterCode value) {
  return ::google::protobuf::internal::NameOfEnum(
    RegisterResponse_RegisterCode_descriptor(), value);
}
inline bool RegisterResponse_RegisterCode_Parse(
    const ::std::string& name, RegisterResponse_RegisterCode* value) {
  return ::google::protobuf::internal::ParseNamedEnum<RegisterResponse_RegisterCode>(
    RegisterResponse_RegisterCode_descriptor(), name, value);
}
// ===================================================================

class RegisterRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Hanasu.RegisterRequest) */ {
 public:
  RegisterRequest();
  virtual ~RegisterRequest();

  RegisterRequest(const RegisterRequest& from);

  inline RegisterRequest& operator=(const RegisterRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RegisterRequest& default_instance();

  static inline const RegisterRequest* internal_default_instance() {
    return reinterpret_cast<const RegisterRequest*>(
               &_RegisterRequest_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(RegisterRequest* other);

  // implements Message ----------------------------------------------

  inline RegisterRequest* New() const PROTOBUF_FINAL { return New(NULL); }

  RegisterRequest* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const RegisterRequest& from);
  void MergeFrom(const RegisterRequest& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(RegisterRequest* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string nick = 1;
  void clear_nick();
  static const int kNickFieldNumber = 1;
  const ::std::string& nick() const;
  void set_nick(const ::std::string& value);
  #if LANG_CXX11
  void set_nick(::std::string&& value);
  #endif
  void set_nick(const char* value);
  void set_nick(const char* value, size_t size);
  ::std::string* mutable_nick();
  ::std::string* release_nick();
  void set_allocated_nick(::std::string* nick);

  // string userName = 2;
  void clear_username();
  static const int kUserNameFieldNumber = 2;
  const ::std::string& username() const;
  void set_username(const ::std::string& value);
  #if LANG_CXX11
  void set_username(::std::string&& value);
  #endif
  void set_username(const char* value);
  void set_username(const char* value, size_t size);
  ::std::string* mutable_username();
  ::std::string* release_username();
  void set_allocated_username(::std::string* username);

  // string pass = 3;
  void clear_pass();
  static const int kPassFieldNumber = 3;
  const ::std::string& pass() const;
  void set_pass(const ::std::string& value);
  #if LANG_CXX11
  void set_pass(::std::string&& value);
  #endif
  void set_pass(const char* value);
  void set_pass(const char* value, size_t size);
  ::std::string* mutable_pass();
  ::std::string* release_pass();
  void set_allocated_pass(::std::string* pass);

  // @@protoc_insertion_point(class_scope:Hanasu.RegisterRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr nick_;
  ::google::protobuf::internal::ArenaStringPtr username_;
  ::google::protobuf::internal::ArenaStringPtr pass_;
  mutable int _cached_size_;
  friend struct protobuf_Register_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class RegisterResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Hanasu.RegisterResponse) */ {
 public:
  RegisterResponse();
  virtual ~RegisterResponse();

  RegisterResponse(const RegisterResponse& from);

  inline RegisterResponse& operator=(const RegisterResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RegisterResponse& default_instance();

  static inline const RegisterResponse* internal_default_instance() {
    return reinterpret_cast<const RegisterResponse*>(
               &_RegisterResponse_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(RegisterResponse* other);

  // implements Message ----------------------------------------------

  inline RegisterResponse* New() const PROTOBUF_FINAL { return New(NULL); }

  RegisterResponse* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const RegisterResponse& from);
  void MergeFrom(const RegisterResponse& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(RegisterResponse* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  typedef RegisterResponse_RegisterCode RegisterCode;
  static const RegisterCode SUCCESS =
    RegisterResponse_RegisterCode_SUCCESS;
  static const RegisterCode USER_NAME_EXISTS =
    RegisterResponse_RegisterCode_USER_NAME_EXISTS;
  static const RegisterCode PASSWORD_TOO_SHORT =
    RegisterResponse_RegisterCode_PASSWORD_TOO_SHORT;
  static const RegisterCode NICK_NAME_TOO_LONG =
    RegisterResponse_RegisterCode_NICK_NAME_TOO_LONG;
  static inline bool RegisterCode_IsValid(int value) {
    return RegisterResponse_RegisterCode_IsValid(value);
  }
  static const RegisterCode RegisterCode_MIN =
    RegisterResponse_RegisterCode_RegisterCode_MIN;
  static const RegisterCode RegisterCode_MAX =
    RegisterResponse_RegisterCode_RegisterCode_MAX;
  static const int RegisterCode_ARRAYSIZE =
    RegisterResponse_RegisterCode_RegisterCode_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  RegisterCode_descriptor() {
    return RegisterResponse_RegisterCode_descriptor();
  }
  static inline const ::std::string& RegisterCode_Name(RegisterCode value) {
    return RegisterResponse_RegisterCode_Name(value);
  }
  static inline bool RegisterCode_Parse(const ::std::string& name,
      RegisterCode* value) {
    return RegisterResponse_RegisterCode_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // .Hanasu.RegisterResponse.RegisterCode code = 1;
  void clear_code();
  static const int kCodeFieldNumber = 1;
  ::Hanasu::RegisterResponse_RegisterCode code() const;
  void set_code(::Hanasu::RegisterResponse_RegisterCode value);

  // @@protoc_insertion_point(class_scope:Hanasu.RegisterResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  int code_;
  mutable int _cached_size_;
  friend struct protobuf_Register_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// RegisterRequest

// string nick = 1;
inline void RegisterRequest::clear_nick() {
  nick_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& RegisterRequest::nick() const {
  // @@protoc_insertion_point(field_get:Hanasu.RegisterRequest.nick)
  return nick_.GetNoArena();
}
inline void RegisterRequest::set_nick(const ::std::string& value) {
  
  nick_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Hanasu.RegisterRequest.nick)
}
#if LANG_CXX11
inline void RegisterRequest::set_nick(::std::string&& value) {
  
  nick_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Hanasu.RegisterRequest.nick)
}
#endif
inline void RegisterRequest::set_nick(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  nick_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Hanasu.RegisterRequest.nick)
}
inline void RegisterRequest::set_nick(const char* value, size_t size) {
  
  nick_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Hanasu.RegisterRequest.nick)
}
inline ::std::string* RegisterRequest::mutable_nick() {
  
  // @@protoc_insertion_point(field_mutable:Hanasu.RegisterRequest.nick)
  return nick_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* RegisterRequest::release_nick() {
  // @@protoc_insertion_point(field_release:Hanasu.RegisterRequest.nick)
  
  return nick_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void RegisterRequest::set_allocated_nick(::std::string* nick) {
  if (nick != NULL) {
    
  } else {
    
  }
  nick_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), nick);
  // @@protoc_insertion_point(field_set_allocated:Hanasu.RegisterRequest.nick)
}

// string userName = 2;
inline void RegisterRequest::clear_username() {
  username_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& RegisterRequest::username() const {
  // @@protoc_insertion_point(field_get:Hanasu.RegisterRequest.userName)
  return username_.GetNoArena();
}
inline void RegisterRequest::set_username(const ::std::string& value) {
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Hanasu.RegisterRequest.userName)
}
#if LANG_CXX11
inline void RegisterRequest::set_username(::std::string&& value) {
  
  username_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Hanasu.RegisterRequest.userName)
}
#endif
inline void RegisterRequest::set_username(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Hanasu.RegisterRequest.userName)
}
inline void RegisterRequest::set_username(const char* value, size_t size) {
  
  username_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Hanasu.RegisterRequest.userName)
}
inline ::std::string* RegisterRequest::mutable_username() {
  
  // @@protoc_insertion_point(field_mutable:Hanasu.RegisterRequest.userName)
  return username_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* RegisterRequest::release_username() {
  // @@protoc_insertion_point(field_release:Hanasu.RegisterRequest.userName)
  
  return username_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void RegisterRequest::set_allocated_username(::std::string* username) {
  if (username != NULL) {
    
  } else {
    
  }
  username_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), username);
  // @@protoc_insertion_point(field_set_allocated:Hanasu.RegisterRequest.userName)
}

// string pass = 3;
inline void RegisterRequest::clear_pass() {
  pass_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& RegisterRequest::pass() const {
  // @@protoc_insertion_point(field_get:Hanasu.RegisterRequest.pass)
  return pass_.GetNoArena();
}
inline void RegisterRequest::set_pass(const ::std::string& value) {
  
  pass_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Hanasu.RegisterRequest.pass)
}
#if LANG_CXX11
inline void RegisterRequest::set_pass(::std::string&& value) {
  
  pass_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Hanasu.RegisterRequest.pass)
}
#endif
inline void RegisterRequest::set_pass(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  pass_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Hanasu.RegisterRequest.pass)
}
inline void RegisterRequest::set_pass(const char* value, size_t size) {
  
  pass_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Hanasu.RegisterRequest.pass)
}
inline ::std::string* RegisterRequest::mutable_pass() {
  
  // @@protoc_insertion_point(field_mutable:Hanasu.RegisterRequest.pass)
  return pass_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* RegisterRequest::release_pass() {
  // @@protoc_insertion_point(field_release:Hanasu.RegisterRequest.pass)
  
  return pass_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void RegisterRequest::set_allocated_pass(::std::string* pass) {
  if (pass != NULL) {
    
  } else {
    
  }
  pass_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), pass);
  // @@protoc_insertion_point(field_set_allocated:Hanasu.RegisterRequest.pass)
}

// -------------------------------------------------------------------

// RegisterResponse

// .Hanasu.RegisterResponse.RegisterCode code = 1;
inline void RegisterResponse::clear_code() {
  code_ = 0;
}
inline ::Hanasu::RegisterResponse_RegisterCode RegisterResponse::code() const {
  // @@protoc_insertion_point(field_get:Hanasu.RegisterResponse.code)
  return static_cast< ::Hanasu::RegisterResponse_RegisterCode >(code_);
}
inline void RegisterResponse::set_code(::Hanasu::RegisterResponse_RegisterCode value) {
  
  code_ = value;
  // @@protoc_insertion_point(field_set:Hanasu.RegisterResponse.code)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


}  // namespace Hanasu

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::Hanasu::RegisterResponse_RegisterCode> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Hanasu::RegisterResponse_RegisterCode>() {
  return ::Hanasu::RegisterResponse_RegisterCode_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Register_2eproto__INCLUDED
