//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
// Licensed under the GNU Affero General Public License, Version 3.
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: meta-template-archive
// File: require.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CALE_META_TEMPLATE_ARCHIVE_REQUIRE_H
#define HEADER_GUARD_CALE_META_TEMPLATE_ARCHIVE_REQUIRE_H
//---------------------------------------------------------------------------//
// Brief: Common concept definitions under namespace mta::req
//---------------------------------------------------------------------------//
// Includes:
#include <concepts>
#include <ranges>
#include <utility>
//---------------------------------------------------------------------------//

namespace mta{
namespace req {

/* Core Language Concepts */
template <typename T, typename U>
concept iSameAs =
    std::same_as<T, U>;  

template <typename T, typename U>
concept iDerivedFrom = std::derived_from<T, U>;  

template <typename T, typename U>
concept iConvertibleTo =
    std::convertible_to<T, U>;  
                                

template <typename T, typename U>
concept iCommonRefWith =
    std::common_reference_with<T, U>;  

template <typename T, typename U>
concept iCommonWith =
    std::common_with<T,
                     U>;  

template <typename T>
concept iIntegral =
    std::integral<T>;  

template <typename T>
concept iSignedIntegral =
    std::signed_integral<T>;  
                              

template <typename T>
concept iUnsignedIntegral =
    std::unsigned_integral<T>;  
                                

template <typename T>
concept iFloatingPoint = std::floating_point<T>;  

template <typename LhsT, typename RhsT>
concept iAssignableFrom =
    std::assignable_from<LhsT, RhsT>;  

template <typename T>
concept iSwappable =
    std::swappable<T>;  

template <typename T, typename U>
concept iSwappableWith =
    std::swappable_with<T, U>;  
                                

template <typename T>
concept iDestructible = std::destructible<T>;  

template <typename T, typename U>
concept iConstructibleFrom =
    std::constructible_from<T, U>;  

template <typename T>
concept iDefaultInitializable =
    std::default_initializable<T>;  

template <typename T>
concept iMoveConstructible =
    std::move_constructible<T>;  

// note: Types without a move constructor, but with a copy constructor that
// accepts const T& arguments,
//       satisfy std::is_move_constructible.
template <typename T>
concept iCopyAndMoveConstructible =
    std::move_constructible<T>;  

/* Comparison concepts */
template <typename T>
concept iEqualityComparable =
    std::equality_comparable<T>;  

template <typename T, typename U>
concept iEqualityComparableWith =
    std::equality_comparable_with<T, U>; 
template <typename T>
concept iTotallyOrdered =
    std::totally_ordered<T>;  
                            
template <typename T, typename U>
concept iTotallyOrderedWith =
    std::totally_ordered_with<T, U>;  

template <typename T>
concept iMoveAndSwappable = std::movable<T>; 

template <typename T>
concept iCopyMoveAndSwappable =
    std::copyable<T>; 

template <typename T>
concept iSemiregular = std::semiregular<T>;  

template <typename T>
concept iRegular = std::regular<T>; 

template <typename T>
concept iInvocable = std::invocable<T>; 

template <typename T>
concept iRegularInvocable =
    std::regular_invocable<T>;  

template <typename T>
concept iPredicate = std::predicate<T>; 

template <typename R, typename T, typename U>
concept iEquivalenceRelation =
    std::equivalence_relation<R, T, U>;  

template <typename R, typename T, typename U>
concept iStrictWeakOrder =
    std::strict_weak_order<R, T, U>;  

/// Range concepts

template <typename T>
concept iRange = std::ranges::range<T>; 

template <typename T>
concept iBorrowedRange =
    std::ranges::borrowed_range<T>;  

template <typename T>
concept iSizedRange =
    std::ranges::sized_range<T>; 

template <typename T>
concept iRangeView = std::ranges::view<T>; 

template <typename T>
concept iInputRange =
    std::ranges::input_range<T>;  

template <typename T, typename U>
concept iOutputRange =
    std::ranges::output_range<T, U>;  

template <typename T>
concept iForwardRange =
    std::ranges::forward_range<T>;  

template <typename T>
concept iBidirectionalRange =
    std::ranges::bidirectional_range<T>;  

template <typename T>
concept iRandomAccessRange =
    std::ranges::random_access_range<T>;  

template <typename T>
concept iContiguousRange =
    std::ranges::contiguous_range<T>;  

template <typename T>
concept iCommonRange =
    std::ranges::common_range<T>;  

template <typename T>
concept iViewableRange =
    std::ranges::viewable_range<T>;  

/* Iterator concepts */

template <typename T>
concept iIndirectlyReadable =
    std::indirectly_readable<T>;  

template <typename T, typename U>
concept iIndirectlyWritable =
    std::indirectly_writable<T, U>;  

template <typename T>
concept iWeaklyIncrementableIter =
    std::weakly_incrementable<T>;  

template <typename T>
concept iIncrementableIter =
    std::incrementable<T>;  

template <typename T>
concept iInputOrOutputIter =
    std::input_or_output_iterator<T>;  

template <typename SentinelT, typename IterT>
concept iSentinelForIter =
    std::sentinel_for<SentinelT, IterT>;  

template <typename SentinelT, typename IterT>
concept iSizedSentinelForIter =
    std::sized_sentinel_for<SentinelT,
                            IterT>;  
template <typename T>
concept iInputIter = std::input_iterator<T>;  

template <typename T>
concept iOutputIter = std::input_iterator<T>;  

template <typename T>
concept iForwardIter =
    std::forward_iterator<T>;  

template <typename T>
concept iBidirectionalIter =
    std::bidirectional_iterator<T>;  

template <typename T>
concept iRandomAccessIter =
    std::random_access_iterator<T>;  

template <typename T>
concept iContiguousIter =
    std::contiguous_iterator<T>;  

/* Indirect callable concepts */

/* <@concept:IndirectlyUnaryInvocable_>
 * <@brief:Specifies that a callable type can be invoked with the result of
 * dereferencing an indirectly_readable type.>
 * */
template <typename T, typename U>
concept iIndirectlyUnaryInvocable = std::indirectly_unary_invocable<T, U>;

/* <@concept:IndirectlyRegularUnaryInvocable_>
 * <@brief:Specifies that a callable type can be invoked with the result of
 * dereferencing an indirectly_readable regular type.>
 * */
template <typename T, typename U>
concept iIndirectlyRegularUnaryInvocable =
    std::indirectly_regular_unary_invocable<T, U>;

/* <@concept:IndirectUnaryPredicate_>
 * <@brief:specifies that a callable type, when invoked with the result of
 * dereferencing an indirectly_readable type, satisfies predicate.>
 * */
template <typename T, typename U>
concept iIndirectUnaryPredicate = std::indirect_unary_predicate<T, U>;

/* <@concept:IndirectBinaryPredicate_>
 * <@brief:	Specifies that a callable type, when invoked with the result of
 * dereferencing two indirectly_readable types, satisfies predicate.>
 * */
template <typename T, typename U, typename V>
concept iIndirectBinaryPredicate = std::indirect_binary_predicate<T, U, V>;

/* <@concept:IndirectEquivalenceRelation_>
 * <@brief:	Specifies that a callable type, when invoked with the result of
 * dereferencing two indirectly_readable types, satisfies equivalence_relation.>
 * */
template <typename T, typename U, typename V = U>
concept iIndirectEquivalenceRelation =
    std::indirect_equivalence_relation<T, U, V>;

/* <@concept:IndirectStrictWeakOrder_>
 * <@brief:	specifies that a callable type, when invoked with the result of
 * dereferencing two indirectly_readable types, satisfies strict_weak_order.>
 * */
template <typename T, typename U, typename V = U>
concept iIndirectStrictWeakOrder = std::indirect_strict_weak_order<T, U, V>;

/* specifies that values may be moved from an indirectly_readable type to an
 * indirectly_writable type */
template <typename T, typename U>
concept iIndirectlyMovable = std::indirectly_movable<T, U>;

/* specifies that values may be moved from an indirectly_readable type to an
 * indirectly_writable type and that the move may be performed via an
 * intermediate object */
template <typename T, typename U>
concept iIndirectlyMovableStorage = std::indirectly_movable_storable<T, U>;

/* specifies that values may be copied from an indirectly_readable type to an
 * indirectly_writable type */
template <typename T, typename U>
concept iIndirectlyCopyable = std::indirectly_copyable<T, U>;

/* specifies that values may be copied from an indirectly_readable type to an
 * indirectly_writable type and that the copy may be performed via an
 * intermediate object */
template <typename T, typename U>
concept iIndirectlyCopyableStorage = std::indirectly_copyable_storable<T, U>;

/* specifies that the values referenced by two indirectly_readable types can be
 * swapped */
template <typename T, typename U>
concept iIndirectlySwappable = std::indirectly_swappable<T, U>;

/* specifies that the values referenced by two indirectly_readable types can be
 * compared */
template <typename T, typename U, typename V, typename X = std::identity,
          typename Y = std::identity>
concept iIndirectlyComparable = std::indirectly_comparable<T, U, V, X, Y>;

/* <@concept: Mergeable_>
 * <@brief: Specifies the common requirements of algorithms that reorder
 * elements in place.>
 * */
template <typename T>
concept iPermutable = std::permutable<T>;

/* <@concept: Mergeable_>
 * <@brief: Specifies the requirements for algorithms that merge two input
 * ranges into a single output range according to the strict weak ordering
 * imposed by X.>
 * */
template <typename T, typename U, typename V, typename X = std::ranges::less,
          typename Y = std::identity, typename Z = std::identity>
concept iMergeable = std::mergeable<T, U, V, X, Y, Z>;

/* <@concept: Mergeable_>
 * <@brief: Specifies the requirements for algorithms that permute a range into
 * an ordered range according to R..>
 * */
template <typename T, typename U = std::ranges::less,
          typename V = std::identity>
concept iSortable = std::sortable<T, U, V>;

/// <@concept:Arithmetic_>
/// <@brief: Specifies an integral type, a floating-point type; or a
/// cv-qualified version thereof.>
template <typename T>
concept iArithmetic = std::is_arithmetic_v<T>;

/// <@concept:Boolean_>
/// <@brief: Specifies a type which is a bool, or any cv ref qualified version
/// thereof.>
template <typename T>
concept iBoolean = iSameAs<std::remove_cvref_t<T>, bool>;

/// <@concept:IntegralNumeral_>
/// <@brief: Specifies a type which is an integral excluding bool; or any cv ref
/// qualified version thereof.>
template <typename T>
concept iIntegralNumeral = iIntegral<T> && not iBoolean<T>;

/// <@concept:IntegralNumeral_>
/// <@brief: Specifies a type which is an unsigned integral excluding bool; or
/// any cv ref qualified version thereof.>
template <typename T>
concept iUnsignedIntegralNumeral = iIntegralNumeral<T> && iUnsignedIntegral<T>;

/// <@concept:IntegralNumeral_>
/// <@brief: Specifies a type which is an signed integral excluding bool; or any
/// cv ref qualified version thereof.>
template <typename T>
concept iSignedIntegralNumeral = iIntegralNumeral<T> && iSignedIntegral<T>;

/// <@concept:Numeral_>
/// <@brief: Specifies an integral numeral type, a floating-point type; or a
/// cv-qualified version thereof.>
template <typename T>
concept iNumeral = iIntegralNumeral<T> || iFloatingPoint<T>;

/// <@concept:SpecializedArithmetic_>
/// <@brief: Specifies an arithmetic type that is specialized in
/// std::numeric_limits.>
template <typename T>
concept iSpecializedArithmetic =
    iArithmetic<T> && std::numeric_limits<T>::is_specialized;

/// <@concept:SpecializedArithmetic_>
/// <@brief: Specifies an arithmetic type that is specialized and has at least
/// one value that,
///          if used as an argument to an arithmetic operation, will generate a
///          trap.>
template <typename T>
concept iTrappingArithmetic =
    iSpecializedArithmetic<T> && std::numeric_limits<T>::traps;

/// <@concept:Iec559Floating_>
/// <@brief: Specifies floating-point type which fulfill the requirements of IEC
/// 559 (IEEE 754) standard. This means that
/// std::numeric_limits<T>::has_infinity, std::numeric_limits<T>::has_quiet_NaN,
/// and std::numeric_limits<T>::has_signaling_NaN are also true.>
template <typename T>
concept iIec559Floating =
    iFloatingPoint<T> && std::numeric_limits<T>::is_iec559;

/// <@concept: TrappingFloating_>
/// <@brief: Specifies an floating type that is specialized and has at least one
/// value that,
///          if used as an argument to an arithmetic operation, will generate a
///          trap.>
template <typename T>
concept iTrappingFloating = iFloatingPoint<T> && iTrappingArithmetic<T>;

/// <@concept: FloatingHasInf_>
/// <@brief: Specifies an floating type that is specialized and capable of
/// representing
///          the positive infinity as a distinct special value.>
template <typename T>
concept iFloatingHasInf =
    iFloatingPoint<T> && std::numeric_limits<T>::has_infinity;

/// <@concept: FloatingHasQuietNan_>
/// <@brief: Specifies an floating type that is capable of representing the
/// special value
///          "Quiet Not-A-Number".>
template <typename T>
concept iFloatingHasQuietNan =
    iFloatingPoint<T> && std::numeric_limits<T>::has_quiet_NaN;

/// <@concept: FloatingHasSignalingNan_>
/// <@brief: Specifies an floating type that can represent the special value
/// "signaling not-a-number" (NaN).>
template <typename T>
concept iFloatingHasSignalingNan =
    iFloatingPoint<T> && std::numeric_limits<T>::has_signaling_NaN;

/// <@concept:SafeFloating_>
/// <@brief: Iec559 compliant, non-trapping floating point types.>
template <typename T>
concept iSafeFloating = iIec559Floating<T> && not iTrappingFloating<T>;

///< @concept FloatingUnsafe_
///< Possibly trapping floating point types
template <typename T>
concept iFloatingUnsafe =
    std::is_arithmetic_v<T> && iFloatingPoint<T> &&
    std::numeric_limits<T>::is_specialized && std::numeric_limits<T>::traps;

//@concept
// non-arithmetic, trivial, non-abstract class or struct types
template <typename T>
concept iTrivialObject =
    !std::is_arithmetic_v<T> && std::is_trivial_v<T> && !std::is_abstract_v<T>;

template <typename T>
concept iCopyConstructible = std::is_copy_constructible_v<T>;

template <typename T>
concept iCopyAssignable = std::is_copy_assignable_v<T>;

template <typename T>
concept iCopyable = iCopyConstructible<T> && iCopyAssignable<T>;

template <typename T>
concept iNonCopyable = (not iCopyConstructible<T>)&&(not iCopyAssignable<T>);

template <typename T>
concept iMoveAssignable = std::is_move_assignable_v<T>;

template <typename T>
concept iMovable = iMoveConstructible<T> && iMoveAssignable<T>;

template <typename T>
concept iNonMovable = (not iMoveConstructible<T>)&&(not iMoveAssignable<T>);

template <typename T>
concept iMoveOnly = iMovable<T> && (not iCopyable<T>);

template <typename T>
concept iNoThrowMoveConstructible = std::is_nothrow_move_constructible_v<T>;

template <typename T>
concept iNoThrowMoveAssignable = std::is_nothrow_move_assignable_v<T>;

template <typename T>
concept iMemberObjectPointer = std::is_member_object_pointer_v<T>;

template <typename T>
concept iMemberFunctionPointer = std::is_member_function_pointer_v<T>;

/// Cannot be moved copied or swapped
template <typename T>
concept iSingular = iNonCopyable<T> && iNonMovable<T> && (not iSwappable<T>);

template <class T>
concept iEnumeration = requires() { std::is_enum_v<T>; };

}  // namespace req
}

//---------------------------------------------------------------------------//
// Copyright 2024 Anton Yashchenko
//
// Licensed under the GNU Affero General Public License, Version 3.
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
//---------------------------------------------------------------------------//
// Author(s): Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.acpp.dev
//---------------------------------------------------------------------------//
// Project: C& Programming Language Environment
// Directory: meta-template-archive
// File: require.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CALE_META_TEMPLATE_ARCHIVE_REQUIRE_H
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//