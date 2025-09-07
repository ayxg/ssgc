///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
// Licensed under the Apache License, Version 2.0(the "License");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file
/// @ingroup coreappfw
/// @brief [HEADER] Entity system.
///
///
/// - The value which an Ent object wraps is called the "user object", and will be refered to as such in the comments.
///   The value which the user provides to an Ent's constructor is the  "given user object". The value stored inside the
///   constructed EntModel<T> is the "stored user object". The "user type" is therefore the type of this object, which
///   will be the same for both the given and stored object.
/// - In the virtual base class EntInterface, the user-overloadable methods are prefixed with 't'. Private prototype
///   methods are prefixed with 'tp'. The user should never have to directly call the virtual base. Each base method
///   which is available to call for the user(i.e. not dispatched by the engine internally) should be called from the
///   wrapper Ent class. Each user-callable method has an equivalent in the Ent named the same as the virtual method
///   without the prefix 't'.
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @addtogroup coreappfw
/// @{

#pragma once
#include <concepts>
#include <string>
#include <type_traits>
#include <vector>
// clang-format off
#include "cxxx.hpp"
#include "CAF/PreConfig.hpp"
#include "CAF/System/EngineMeta.hpp"
#include "CAF/System/EntityCore.hpp"
// clang-format on

// namespace cxx {
// template <class T, class RegisterFnT, class UnregisterFnT>
//   requires std::same_as<void, std::invoke_result_t<RegisterFnT, T**>> &&
//            std::same_as<void, std::invoke_result_t<UnregisterFnT, T**>>
// struct strong_ptr {
//   using value_type = T;
//   using pointer_type = value_type*;
//   using guard_type = value_type**;
//
//   pointer_type mptr;
//
//   strong_ptr(pointer_type ptr) : mptr(ptr) {
//     if (ptr) RegisterFnT{}(guard_type{&mptr});
//   }
//
//   ~strong_ptr() {
//     if (mptr) UnregisterFnT{}(guard_type{&mptr});
//   }
//
//   operator bool() const noexcept { return mptr != nullptr; }
// };
// }

namespace caf {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// External Dependencies
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using TimeType = sf::Time;
using OsEventType = sf::Event;
using EventBufferType = std::vector<OsEventType>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// fwd decl
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clang-format off
class Ent;
class EntInterface;
template <class T> class EntModel;
struct EXCEntityNotCopyable;
struct EntEventHandlerChain;
// clang-format on

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @class EntInterface
/// @brief Ent's virtual base interface. An Ent stores all given types as a pointer to this
///        virtual base class. An EntInterface is always guaranteed to be a base class of some
///        EntModel<T>. Furthermore, this interface inherits from EntImpl which is the type
///        agnostic implementation for all entities.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EntInterface {
  // User Methods
  constexpr virtual void tCreate(Ent& self_ent) = 0;

  // Called right before destruction of T as an Entity. 'self_ent' argument is the owning 'Ent' of 'T'(self). It is
  // guaranteed to be cast-able to T unchecked within this method.
  constexpr virtual void tDestroy(Ent& self_ent) = 0;

  // Called when an OS event is received from a parent entity which is an OS event generator (usually a window).
  constexpr virtual eProcRes tProcOsEvent(const OsEventType& ev) = 0;

  // Called to process the context frame's event buffer for this entity. EventBufferType is an array of all caf
  // context-wide events polled/generated for this frame.
  constexpr virtual eProcRes tProcEvents(Ent& self_ent, EventBufferType& evs) = 0;

  /// Called to process the context frame's systems for this entity. The user override is expected(but not mandated) to
  /// process the entity's systems, and optionally branch nodes, by calling 'self_ent.SubProcSystems(dt)' and returning
  /// the result.
  ///
  /// - Default override forwards to 'self_ent.SubProcSystems', recursivley processing branch nodes.
  /// - If a given user object declares this method, default override is NOT called.
  ///   In this case, the user should return 'eProcResult::kContinue'.
  /// - User objects are allowed to not process branch nodes, or process them using some custom logic.
  /// - 'dt' is the time elapsed since the last frame, may be used to process time-independent logic.
  constexpr virtual eProcRes tProcSystems(Ent& self_ent, const TimeType& dt) = 0;

  /// Called to process the context frame's graphics for this entity.
  ///
  /// - Default override forwards to 'self_ent.SubProcGraphics', recursivley processing branch nodes.
  /// - If a given user object declares this method, default override is NOT called. In this case, the user should
  ///   return 'eProcResult::kContinue'.
  /// - User objects are allowed to not process branch nodes, or process them using some custom logic.
  constexpr virtual eProcRes tProcGraphics(Ent& self_ent) = 0;

  /// Disabled by default. Called when an entity is an event generator, returning 'false' when polling for the entity's
  /// context frame should stop. User override is expected to set the passed 'ev' arg to the generated event for each
  /// event, returning 'true'.
  /// @note This should rarely be overloaded, standard os event generator entities are provided by caf. eg.TGLWindow
  constexpr virtual bool tPollOsEvent(OsEventType& ev) = 0;

  virtual ~EntInterface() {}
  /////////////////////////////////////////////////////////////////////////////
  // Prototype methods prefixed with [tp] for "entity prototype".
  // User should NEVER override these methods.
  /////////////////////////////////////////////////////////////////////////////
  constexpr virtual std::unique_ptr<EntInterface> tpClone() const = 0;
  constexpr virtual std::type_index tpTypeIndex() const = 0;

  constexpr virtual bool tpOverridesPollOsEvent() = 0;
  constexpr virtual bool tpOverridesHandleOsEvent() = 0;

  // constexpr virtual void tpPushEventHandler() = 0;
  // constexpr virtual void tpRemoveEventHandler() = 0;
  // constexpr virtual void tpViewEventHandlers() = 0;
};

/// Exception thrown when an attempting to call Ent::Clone() on a non-copyable entity.
struct EXCEntityNotCopyable {
  std::string msg{""};
  constexpr explicit EXCEntityNotCopyable(Ent* pent = nullptr) noexcept;
  constexpr const char* what() const noexcept;
};

struct EntEventHandlerChain {
  std::unique_ptr<std::function<eProcRes(Ent&, const OsEventType&)>> phandler{nullptr};
  std::unique_ptr<EntEventHandlerChain> next{nullptr};

  constexpr void Push(const std::function<eProcRes(Ent&, const OsEventType&)>& cb) {
    if (!phandler) {
      phandler = std::make_unique<std::function<eProcRes(Ent&, const OsEventType&)>>(cb);
    } else {
      if (!next) next = std::make_unique<EntEventHandlerChain>();
      next->Push(cb);
    }
  }

  constexpr void Push(std::function<eProcRes(Ent&, const OsEventType&)>&& cb) {
    if (!phandler) {
      phandler = std::make_unique<std::function<eProcRes(Ent&, const OsEventType&)>>(std::move(cb));
    } else {
      if (!next) next = std::make_unique<EntEventHandlerChain>();
      next->Push(std::move(cb));
    }
  }

  constexpr void Pop() {
    if (next) {
      next->Pop();
      next.reset();
      phandler.reset();
    } else {
      phandler.reset();
    }
  }

  constexpr ~EntEventHandlerChain() {
    if (next) next->phandler.reset();
    if (phandler) phandler.reset();
  }
  constexpr EntEventHandlerChain(EntEventHandlerChain&& other) noexcept
      : phandler{std::move(other.phandler)}, next{std::move(other.next)} {}
  constexpr EntEventHandlerChain(const EntEventHandlerChain& other) {
    if (other.phandler) phandler = std::make_unique<std::function<eProcRes(Ent&, const OsEventType&)>>(*other.phandler);
    if (other.next) next = std::make_unique<EntEventHandlerChain>(*other.next);
  }
  constexpr EntEventHandlerChain() = default;
  constexpr EntEventHandlerChain& operator=(EntEventHandlerChain&& other) noexcept {
    if (this != &other) {
      phandler = std::move(other.phandler);
      next = std::move(other.next);
    }
    return *this;
  }
  constexpr EntEventHandlerChain& operator=(const EntEventHandlerChain& other) {
    if (this != &other) {
      if (other.phandler)
        phandler = std::make_unique<std::function<eProcRes(Ent&, const OsEventType&)>>(*other.phandler);
      if (other.next) next = std::make_unique<EntEventHandlerChain>(*other.next);
    }
    return *this;
  }
  constexpr EntEventHandlerChain(std::function<eProcRes(Ent&, const OsEventType&)>&& cb) {
    phandler = std::make_unique<std::function<eProcRes(Ent&, const OsEventType&)>>(std::move(cb));
  }
  constexpr EntEventHandlerChain(const std::function<eProcRes(Ent&, const OsEventType&)>& cb) {
    phandler = std::make_unique<std::function<eProcRes(Ent&, const OsEventType&)>>(cb);
  }
};

template <class T>
concept iGivenUserObject = requires(T t) { requires std::is_move_constructible_v<T>; };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declare Entity Named Requirements
//
// Functions begging with ent are for the user to optionally overload, they are only called
// by the underlying entity system itself.
//
// - tCreate
// - tDestroy(const T& self, Ent& self_ent) :
//      Called right before destruction of T as an Entity. 'self_ent' argument is the owning 'Ent' of 'T'(self). It is
//      guaranteed to be cast-able to T unchecked within this method.
// - tProcOsEvent
// - tProcEvents
// - tProcSystems
// - tProcGraphics
// - tPollOsEvent
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// [tCreate]
template <class T>
constexpr void tCreate(T& self, Ent& self_ent) = delete;
template <>
constexpr void tCreate(Ent& self, Ent& self_ent);

template <typename T>
concept EntHasMemberCreate = requires(T t, Ent& self_ent) {
  { t.tCreate(self_ent) } -> std::same_as<void>;
};

template <typename T>
concept EntHasStaticCreate = requires(T t, Ent& self_ent) {
  { tCreate(t, self_ent) } -> std::same_as<void>;
};

///////////////////////////////////////////////////////////
// [tDestroy]
template <class T>
constexpr void tDestroy(T& self, Ent& self_ent) = delete;
template <>
constexpr void tDestroy(Ent& self, Ent& self_ent);

template <typename T>
concept EntHasMemberDestroy = requires(T t, Ent& self_ent) {
  { t.tDestroy(self_ent) } -> std::same_as<void>;
};

template <typename T>
concept EntHasStaticDestroy = requires(T t, Ent& self_ent) {
  { tDestroy(t, self_ent) } -> std::same_as<void>;
};

///////////////////////////////////////////////////////////
// [tProcOsEvent]
template <class T>
constexpr eProcRes tProcOsEvent(T& self, const OsEventType& ev) = delete;

template <>
constexpr eProcRes tProcOsEvent(Ent& self, const OsEventType& ev);

template <typename T>
concept EntHasMemberHandleOsEvent = requires(T& t, const OsEventType& ev) {
  { t.tProcOsEvent(ev) } -> std::same_as<eProcRes>;
};

template <typename T>
concept EntHasStaticHandleOsEvent = requires(T& t, const OsEventType& ev) {
  { tProcOsEvent(t, ev) } -> std::same_as<eProcRes>;
};

///////////////////////////////////////////////////////////
// [tProcEvents]
template <class T>
constexpr eProcRes tProcEvents(T& self, Ent& self_ent, EventBufferType& evs) = delete;

template <>
constexpr eProcRes tProcEvents(Ent& self, Ent& self_ent, EventBufferType& evs);

template <typename T>
concept EntHasMemberProcessEvents = requires(T t, Ent& self_ent, EventBufferType& evs) {
  { t.tProcEvents(self_ent, evs) } -> std::same_as<eProcRes>;
};

template <typename T>
concept EntHasStaticProcessEvents = requires(T t, Ent& self_ent, EventBufferType& evs) {
  { tProcEvents(t, self_ent, evs) } -> std::same_as<eProcRes>;
};

///////////////////////////////////////////////////////////
// [tProcSystems]
template <class T>
constexpr eProcRes tProcSystems(T& self, Ent& self_ent, const caf::TimeType& dt) = delete;

template <>
constexpr eProcRes tProcSystems(Ent& self, Ent& self_ent, const caf::TimeType& dt);

template <typename T>
concept EntHasMemberProcessSystems = requires(T t, Ent& self_ent, const caf::TimeType& dt) {
  { t.tProcSystems(self_ent, dt) } -> std::same_as<eProcRes>;
};

template <typename T>
concept EntHasStaticProcessSystems = requires(T t, Ent& self_ent, const caf::TimeType& dt) {
  { tProcSystems(t, self_ent, dt) } -> std::same_as<eProcRes>;
};

///////////////////////////////////////////////////////////
// [tProcGraphics]
template <class T>
constexpr eProcRes tProcGraphics(T& self, Ent& self_ent) = delete;

template <>
constexpr eProcRes tProcGraphics(Ent& self, Ent& self_ent);

template <typename T>
concept EntHasMemberProcessGraphics = requires(T t, Ent& self_ent) {
  { t.tProcGraphics(self_ent) } -> std::same_as<eProcRes>;
};

template <typename T>
concept EntHasStaticProcessGraphics = requires(T t, Ent& self_ent) {
  { tProcGraphics(t, self_ent) } -> std::same_as<eProcRes>;
};

///////////////////////////////////////////////////////////
// [tPollOsEvent]
template <class T>
constexpr bool tPollOsEvent(T& self, OsEventType& ev) = delete;

template <>
constexpr bool tPollOsEvent(Ent& self, OsEventType& ev);

template <typename T>
concept EntHasMemberPollOsEvent = requires(T t, OsEventType& ev) {
  { t.tPollOsEvent(ev) } -> std::same_as<bool>;
};

template <typename T>
concept EntHasStaticPollOsEvent = requires(T t, OsEventType& ev) {
  { tPollOsEvent(t, ev) } -> std::same_as<bool>;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @class EntBase
/// @brief
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class EntBase {
  friend Ent;  // Ent will dispatch directly to EntBase<T> methods if not
               // overriden.
               // friend EntBase<EntBlueprint<T>>;  // All EntBlueprint<T>s are derived from
               //  EntBase<EntBlueprint<T>> and must have
               //  access to EntBase's internal methods.

 protected:
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @class EntModel<T>
/// @brief EntModel's responsibility is to deduce the type-dependent entity methods of the given type T, and make them
/// available through the EntInterface or Ent. EntModel will accept any type, but will only access 'entXXXX' methods if
/// they are defined for type T, or a static overload for a given method for type T is provided in the 'caf' namespace.
/// If neither is defined, a default implementation is provided which usually does nothing. To get 'default'
/// functionality for an external T, you may inherit from EntBase<T>, or use EntBlueprint<T> wrapper to pass an object
/// whose code you are not able to modify.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class EntModel : public EntInterface {
  friend Ent;  // Allow Ent to access 'object_' directly.
 public:
  constexpr void tCreate(Ent& self_ent) override {
    if constexpr (std::is_base_of_v<EntBase<T>, T>) {
      return static_cast<const EntBase<T>&>(object_).tCreate(self_ent);
    } else if constexpr (EntHasMemberCreate<T>) {
      return object_.tCreate(self_ent);
    } else if constexpr (EntHasStaticCreate<T>) {
      using ::caf::tCreate;
      return caf::tCreate(object_, self_ent);
    } else {
      /* No custom 'Create' set for this entity.*/
    }
  }

  constexpr void tDestroy(Ent& self_ent) override {
    if constexpr (std::is_base_of_v<EntBase<T>, T>) {
      return static_cast<const EntBase<T>&>(object_).tDestroy(self_ent);
    } else if constexpr (EntHasMemberDestroy<T>) {
      return object_.tDestroy(self_ent);
    } else if constexpr (EntHasStaticDestroy<T>) {
      using ::caf::tDestroy;
      return caf::tDestroy(object_, self_ent);
    } else {
      /* No custom 'Destroy' set for this entity.*/
    }
  }

  constexpr eProcRes tProcOsEvent(const OsEventType& ev) override {
    if constexpr (std::is_base_of_v<EntBase<T>, T>) {
      static_cast<const EntBase<T>&>(object_).tProcOsEvent(ev);
    } else if constexpr (EntHasMemberHandleOsEvent<T>) {
      return object_.tProcOsEvent(ev);
    } else if constexpr (EntHasStaticHandleOsEvent<T>) {
      using ::caf::tProcOsEvent;
      return caf::tProcOsEvent(object_, ev);
    } else {
      /* No custom 'HandleOsEvent' set for this entity.*/
      return eProcRes::kContinue;
    }
  };

  constexpr eProcRes tProcEvents(Ent& self_ent, EventBufferType& evs) override {
    if constexpr (std::is_base_of_v<EntBase<T>, T>) {
      return static_cast<const EntBase<T>&>(object_).tProcEvents(self_ent, evs);
    } else if constexpr (EntHasMemberProcessEvents<T>) {
      return object_.tProcEvents(self_ent, evs);
    } else if constexpr (EntHasStaticProcessEvents<T>) {
      using ::caf::tProcEvents;
      return caf::tProcEvents(object_, self_ent, evs);
    } else {
      /* No custom 'ProcessEvents' set for this entity.*/
      return eProcRes::kContinue;
    }
  };

  constexpr eProcRes tProcSystems(Ent& self_ent, const TimeType& dt) override {
    if constexpr (std::is_base_of_v<EntBase<T>, T>) {
      return static_cast<const EntBase<T>&>(object_).tProcSystems(self_ent, dt);
    } else if constexpr (EntHasMemberProcessSystems<T>) {
      return object_.tProcSystems(self_ent, dt);
    } else if constexpr (EntHasStaticProcessSystems<T>) {
      using ::caf::tProcSystems;
      return caf::tProcSystems(object_, self_ent, dt);
    } else {
      /* No custom 'ProcessSystems' set for this entity.*/
      return eProcRes::kContinue;
    }
  };

  constexpr eProcRes tProcGraphics(Ent& self_ent) override {
    if constexpr (std::is_base_of_v<EntBase<T>, T>) {
      return static_cast<const EntBase<T>&>(object_).tProcGraphics(self_ent);
    } else if constexpr (EntHasMemberProcessGraphics<T>) {
      return object_.tProcGraphics(self_ent);
    } else if constexpr (EntHasStaticProcessGraphics<T>) {
      using ::caf::tProcGraphics;
      return caf::tProcGraphics(object_, self_ent);
    } else {
      /* No custom 'ProcessGraphics' set for this entity.*/
      return eProcRes::kContinue;
    }
  };

  constexpr bool tPollOsEvent(OsEventType& ev) override {
    if constexpr (std::is_base_of_v<EntBase<T>, T>) {
      return static_cast<const EntBase<T>&>(object_).tPollOsEvent(ev);
    } else if constexpr (EntHasMemberPollOsEvent<T>) {
      return object_.tPollOsEvent(ev);
    } else if constexpr (EntHasStaticPollOsEvent<T>) {
      using ::caf::tPollOsEvent;
      return caf::tPollOsEvent(object_, ev);
    } else {
      /* No custom 'PollOsEvent' set for this entity.*/
      return false;
    }
  };

 public:
  constexpr EntModel(T&& value) : EntInterface(), object_{std::forward<T>(value)} {}
  // constexpr EntModel(const EntModel& other) : object_{other.object_} {}
  // constexpr EntModel(EntModel&& other) : object_{std::move(other.object_)} {}

  // constexpr EntModel(const T& value) : EntInterface(), object_{value} {}

  // Called when an already initialized concrete Ent is being copied. Since Ent must abstract storage both copyable and
  // non-copyable objects at runtime - throws EXCEntityNotCopyable if stored user object is not copyable.
  constexpr std::unique_ptr<EntInterface> tpClone() const override {
    if constexpr (std::copy_constructible<T>) {
      return std::make_unique<EntModel>(*this);
    } else {
      throw EXCEntityNotCopyable();
    }
  }

  constexpr std::type_index tpTypeIndex() const override { return typeid(T); }

  constexpr bool tpOverridesPollOsEvent() override {
    if constexpr (std::is_base_of_v<EntBase<T>, T>)
      return true;
    else if constexpr (EntHasMemberPollOsEvent<T>)
      return true;
    else if constexpr (EntHasStaticPollOsEvent<T>)
      return true;
    else
      return false;
  };

  constexpr bool tpOverridesHandleOsEvent() override {
    if constexpr (std::is_base_of_v<EntBase<T>, T>)
      return true;
    else if constexpr (EntHasMemberHandleOsEvent<T>)
      return true;
    else if constexpr (EntHasStaticHandleOsEvent<T>)
      return true;
    else
      return false;
  };

 private:
  T object_;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @class Ent
/// @brief A generic tree node object. The main type erased class which all entities are stored as. Any passed T is
/// coalesced into a EntInterface pointer which is holds a doubly linked list to any other entities.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Ent : public entity_core::EntCore<Ent> {
  // friend class EntImpl<Ent>;  // Allow EntImpl to access '.pimpl_' class member of Ent.

  // Allow global static overload methods to access '.impl_' class member.
  friend constexpr void tCreate<>(Ent& self, Ent& self_ent);
  friend constexpr void tDestroy<>(Ent& self, Ent& self_ent);
  friend constexpr eProcRes tProcOsEvent<>(Ent& self, const OsEventType& ev);
  friend constexpr eProcRes tProcEvents<>(Ent& self, Ent& self_ent, EventBufferType& evs);
  friend constexpr eProcRes tProcSystems<>(Ent& self, Ent& self_ent, const TimeType& evs);
  friend constexpr eProcRes tProcGraphics<>(Ent& self, Ent& self_ent);
  friend constexpr bool tPollOsEvent<>(Ent& self, OsEventType& ev);

 public:
  template <class T>
  constexpr Ent(const T& x) : entity_core::EntCore<Ent>(), pimpl_{new EntModel<T>(x)} {
    pimpl_->tCreate(*this);
  }

  // This constructor packs all the logic for creating an Ent. It is interpreted by the compiler as a move constructor
  // **BUT** due to the fact that it is a templated constructor, the "T&& user_obj" argument is a universal reference,
  // and will be deduced as either an lvalue or rvalue. Each case is handled differently:
  //
  // - Given a lvalue of type Ent -> copies virtual interface pointer by use of clone() member method. Attempting to
  //   clone an entity wrapping a non-moveable user type will result in a caf::EXCEntityNotCopyable exception.
  //
  // - Given a rvalue of type Ent, moves virtual interface pointer.
  //
  // - Given a lvalue of type T which is not an Ent, interprets the argument as a given user object.  Creates a new
  //   EntModel<T> passing the given user object to the stored user object. Then, calls tCreate(self), if a user
  //   object overload is provided.
  //
  // - Given a rvalue of type T which is not an Ent, interprets the argument as a given user object. Creates a new
  //   EntModel<T> perfect forwarding the given user object to the stored user object. Then, calls tCreate(self), if
  //   a user object overload is provided.
  template <class T>
  constexpr Ent(T&& user_obj) noexcept : entity_core::EntCore<Ent>{}, pimpl_{nullptr} {
    // existing entity
    if constexpr (std::same_as<Ent, std::decay_t<T>>) {
      if constexpr (std::is_lvalue_reference_v<decltype(user_obj)>)
        pimpl_ = user_obj.pimpl_->clone();  // attempt manual copy
      else
        pimpl_ = std::move(user_obj.pimpl_);  // move virtual pointer
    }
    // user object
    else {
      if constexpr (std::is_lvalue_reference_v<decltype(user_obj)>) {
        // init stored user object using copy ctor
        pimpl_ = std::unique_ptr<EntInterface>(new EntModel<T>(user_obj));
        pimpl_->tCreate(*this);
      } else {
        // forward to stored user object
        pimpl_ = std::unique_ptr<EntInterface>(new EntModel<T>(std::forward<T>(user_obj)));
        pimpl_->tCreate(*this);
      }
    }
  }

  // Existing entity copy/move constructor specializations.
  //
  // A copy ctor must exist even for non-copyable entities as the Ent class may wrap either or. Whether the object is
  // copyable will only be known at runtime.
  //
  // **This explicit template specialization MUST exist. Otherwise, it will be implicitly deleted (even if the
  // templated constructor above accounts for a 'const Ent &') - the compiler interprets it as a move constructor. This
  // means when you attempt to store or emplace an Ent in a standard container, you will get a seemingly untraceable
  // error:
  //     "[MSVC:C2672] std::construct_at no matching overloaded function found.
  //      could be '_Ty *std::construct_at(_Ty *const ,_Types ...) noexcept(<expr>)'"
  // Even if this ctor is never called by std::construct_at(which it wont be), it must be unambiguous for the
  // container's template.
  constexpr Ent(const Ent& other) : pimpl_{other.pimpl_->tpClone()} {}

  constexpr Ent(Ent&& other) noexcept : pimpl_{std::move(other.pimpl_)} {}

  constexpr Ent& operator=(const Ent& s) {
    pimpl_ = s.pimpl_->tpClone();
    return *this;
  }

  constexpr Ent& operator=(Ent&& s) noexcept {
    pimpl_ = std::move(s.pimpl_);
    return *this;
  }

  // template <class T>
  // constexpr Ent& operator=(const T& user_obj) {
  //   pimpl_ = new EntModel<T>(user_obj);
  //   pimpl_->tCreate(*this);
  //   return *this;
  // }

  // template <class T>
  // constexpr Ent& operator=(T&& user_obj) noexcept {
  //   pimpl_ = new EntModel<T>(std::forward<T>(user_obj));
  //   pimpl_->tCreate(*this);
  //   return *this;
  // }

  constexpr ~Ent() { pimpl_->tDestroy(*this); }

  constexpr std::type_index TypeIndex() const { return pimpl_->tpTypeIndex(); }

  template <class T>
  constexpr T& As() {
    return static_cast<EntModel<T>&>(*pimpl_).object_;
  }

  template <class T>
  constexpr const T& As() const {
    return static_cast<const EntModel<T>&>(*pimpl_).object_;
  }

  template <class T>
  constexpr bool Is() const {
    return pimpl_->tpTypeIndex() == typeid(T);
  }

  using EntCore<Ent>::IsDescendantOf; // Required, or additional Ent impl will hide base method.
  template <class T>
  constexpr bool IsDescendantOf() const {
    const Ent* desc_found = Stem();
    while (desc_found) {
      if (desc_found->Is<T>()) return true;
      desc_found = desc_found->Stem();
    }
    return false; 
  }

  template <class T>
  constexpr bool IsChildOf() const {
    if (Stem()) return Stem()->Is<T>();
    return false;
  }

  template<class T>
  constexpr Ent* FindAncestor() const {
    const Ent* desc_found = Stem();
    while (desc_found) {
      if (desc_found->Is<T>()) return desc_found;
      desc_found = desc_found->Stem();
    }
    return nullptr;
  }

  constexpr eProcRes ProcEvents(EventBufferType& evs) {
    // Handle operating system events separatley. If this is an event generator, create a temporary event in the buffer
    // to store os event while it's being polled.
    if (pimpl_->tpOverridesPollOsEvent()) {
      evs.push_back(OsEventType{});
      auto& ev = evs.back();
      // Poll events and forward event to branches with os event handlers.
      while (pimpl_->tPollOsEvent(ev)) {
        this->Apply([&ev](Ent& br) {
          if (br.event_handlers_.phandler == nullptr) {
            // Dispatch live event handler chain.
            auto* next_handler = &br.event_handlers_;
            while (next_handler != nullptr) {
              if ((*next_handler->phandler)(br, ev) == eProcRes::kStop) return;
              next_handler = next_handler->next ? next_handler->next.get() : nullptr;
            }
          }
          // Call entity class event handler override, if any.
          if (br.pimpl_->tpOverridesHandleOsEvent()) br.pimpl_->tProcOsEvent(ev);
        });
      }
      evs.pop_back();
    }

    return pimpl_->tProcEvents(*this, evs);  // Process caf events.
  }

  constexpr eProcRes ProcSystems(const TimeType& dt) { return pimpl_->tProcSystems(*this, dt); }

  constexpr eProcRes ProcGraphics() { return pimpl_->tProcGraphics(*this); }

  constexpr eProcRes SubProcEvents(EventBufferType& evs) {
    for (auto& branch : brs_) {
      if (branch->ProcEvents(evs) == eProcRes::kStop) return eProcRes::kStop;
    }
    return eProcRes::kContinue;
  }


  constexpr eProcRes SubProcSystems(const TimeType& dt) {
    for (auto& branch : brs_) {
      if (branch->ProcSystems(dt) == eProcRes::kStop) return eProcRes::kStop;
    }
    return eProcRes::kContinue;
  }

  constexpr eProcRes SubProcGraphics() {
    for (auto& branch : brs_) {
      if (branch->ProcGraphics() == eProcRes::kStop) return eProcRes::kStop;
    }
    return eProcRes::kContinue;
  }
  constexpr eProcRes SubProcOsEvents(const OsEventType& ev) {
    for (auto& branch : brs_) {
      if (branch->ProcGraphics() == eProcRes::kStop) return eProcRes::kStop;
    }
    return eProcRes::kContinue;
  }

  constexpr void PushEventHandler(const std::function<eProcRes(Ent&, const OsEventType&)>& cb) {
    event_handlers_.Push(cb);
  }

  constexpr void PushEventHandler(std::function<eProcRes(Ent&, const OsEventType&)>&& cb) { event_handlers_.Push(cb); }

 private:
  std::unique_ptr<EntInterface> pimpl_;
  EntEventHandlerChain event_handlers_;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define Entity Named Requriements
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <>
constexpr void tCreate(Ent& self, Ent& self_ent) {
  self.pimpl_->tCreate(self_ent);
}

template <>
constexpr void tDestroy(Ent& self, Ent& self_ent) {
  self.pimpl_->tDestroy(self_ent);
}

template <>
constexpr eProcRes tProcOsEvent(Ent& self, const OsEventType& ev) {
  return self.pimpl_->tProcOsEvent(ev);
}

template <>
constexpr eProcRes tProcEvents(Ent& self, Ent& self_ent, EventBufferType& evs) {
  return self.pimpl_->tProcEvents(self_ent, evs);
}

template <>
constexpr eProcRes tProcSystems(Ent& self, Ent& self_ent, const caf::TimeType& dt) {
  return self.pimpl_->tProcSystems(self_ent, dt);
}

template <>
constexpr eProcRes tProcGraphics(Ent& self, Ent& self_ent) {
  return self.pimpl_->tProcGraphics(self_ent);
}

template <>
constexpr bool tPollOsEvent(Ent& self, OsEventType& ev) {
  return self.pimpl_->tPollOsEvent(ev);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Source
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr EXCEntityNotCopyable::EXCEntityNotCopyable(Ent* pent) noexcept {
  if (!pent)
    msg = "Entity is not copyable.";
  else
    msg += std::string{"Entity '"} + pent->TypeIndex().name() + "' is not copyable.";
}

constexpr const char* EXCEntityNotCopyable::what() const noexcept { return msg.c_str(); }

}  // namespace caf

/// @} // end of coreappfw

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @project: [CAF] Core Application Framework
// @author(s): Anton Yashchenko
// @website: https://www.acpp.dev
// @created: 2025/07/21
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2025 Anton Yashchenko
//
// Licensed under the Apache License, Version 2.0(the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////