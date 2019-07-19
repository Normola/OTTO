#pragma once

#ifdef _MSC_VER
#pragma warning(disable : 4396) // silly VS warning about inline friend stuff...
#endif

#include <tuple>
#include <type_traits>
#include <utility>

#include "util/string_ref.hpp"

namespace otto::reflect {
  /// @addtogroup reflection
  /// \{

  /// type_list is array of types
  template<typename... Args>
  struct type_list {
    template<std::size_t N>
    using type = std::tuple_element_t<N, std::tuple<Args...>>;
    using indices = std::index_sequence_for<Args...>;
    static const size_t size = sizeof...(Args);
  };


  template<typename... Args>
  constexpr auto members(Args&&... args);

  template<typename T>
  struct _has_reflect_member {
    template<typename C>
    static std::true_type test(decltype(&C::reflect_members));
    template<typename C>
    static std::false_type test(...);

    static constexpr bool value = decltype(test<T>(nullptr))::value;
  };

  /// function used for registration of classes by user
  template<typename Class, typename = std::enable_if_t<_has_reflect_member<Class>::value>>
  constexpr auto register_members()
  {
    return Class::reflect_members();
  }

  /// function used for registration of class name by user
  template<typename Class, typename = std::enable_if_t<_has_reflect_member<Class>::value>>
  constexpr util::string_ref register_name()
  {
    return Class::reflect_name();
  }

  /// returns set name for class
  template<typename Class>
  constexpr util::string_ref get_name();


  /// returns the number of registered members of the class
  template<typename Class>
  constexpr std::size_t get_member_count();

  /// returns std::tuple of Members
  template<typename Class>
  constexpr const auto& get_members();

  /// Check if class has registerMembers<T> specialization (has been registered)
  template<typename Class>
  constexpr bool is_registered();

  /// Check if Class has non-default ctor registered
  template<typename Class>
  constexpr bool ctor_registered();

  template<typename T>
  struct constructor_args {
    using types = type_list<>;
  };

  template<typename T>
  using constructor_arguments = typename constructor_args<T>::types;

  /// Check if user registered non default constructor
  template<typename Class>
  constexpr bool ctor_registered();

  /// Check if class T has member
  template<typename Class>
  bool has_member(const char* name);


  template<typename Class, typename F, typename = std::enable_if_t<is_registered<Class>()>>
  void for_all_members(F&& f);

  /// version for non-registered classes (to generate less template stuff)
  template<typename Class,
           typename F,
           typename = std::enable_if_t<!is_registered<Class>()>,
           typename = void>
  void for_all_members(F&& f);

  /// Do F for member named 'name' with type T. It's important to pass correct type of the member
  template<typename Class, typename T, typename F>
  void for_member(const char* name, F&& f);

  /// Get value of the member named 'name'
  template<typename T, typename Class>
  T get_member_value(Class& obj, const char* name);

  /// Set value of the member named 'name'
  template<typename T,
           typename Class,
           typename V,
           typename = std::enable_if_t<std::is_constructible<T, V>::value>>
  void set_member_value(Class& obj, const char* name, V&& value);

  ///\}
} // namespace otto::reflect

#include "reflect.inl"
