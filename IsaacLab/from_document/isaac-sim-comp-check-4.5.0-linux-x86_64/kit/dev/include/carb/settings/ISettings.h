// Copyright (c) 2019-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! \file
//! \brief carb.settings interface definition
#pragma once

#include "../Defines.h"

#include "../InterfaceUtils.h"
#include "../dictionary/IDictionary.h"

#include <cstddef>
#include <cstdint>

namespace carb
{

//! Namespace for \ref carb::settings::ISettings interface and utilities.
namespace settings
{

//! An opaque type representing a settings transaction.
//! @see ISettings::createTransaction
struct Transaction DOXYGEN_EMPTY_CLASS;

/**
 * The Carbonite Settings interface.
 *
 * Carbonite settings are built on top of the \ref carb::dictionary::IDictionary interface. Many dictionary functions
 * are replicated in settings, but explicitly use the settings database instead of a generic
 * \ref carb::dictionary::Item.
 *
 * ISettings uses keys (or paths) that start with an optional forward-slash and are forward-slash separated (example:
 * "/log/level"). The settings database exists as a root-level \ref carb::dictionary::Item (of type 'dictionary') that
 * is created and maintained by the \c ISettings interface (typically through the *carb.settings.plugin* plugin). The
 * root level settings \ref carb::dictionary::Item is accessible through `getSettingsDictionary("/")`.
 *
 * @thread_safety
 * All functions in \c ISettings are thread-safe unless otherwise specified. By "thread-safe," this means
 *   that individual call to a *carb.settings* API function will fully complete in a thread-safe manner; \b however this
 *   does not mean that multiple calls together will be threadsafe as another thread may act on the settings database
 *   between the API calls. In order to ensure thread-safety across multiple calls, use the \ref ScopedRead and
 *   \ref ScopedWrite RAII objects to ensure locking. The settings database uses a global recursive read/write lock to
 *   ensure thread safety across the entire settings database.
 *
 * @par Subscriptions
 * Portions of the settings database hierarchy can be subscribed to with \ref ISettings::subscribeToTreeChangeEvents, or
 * individual keys may be subscribed to with \ref ISettings::subscribeToNodeChangeEvents. Subscriptions are called in
 * the context of the thread that triggered the change, and only once that thread has released all settings database
 * locks. Subscription callbacks also follow the principles of Basic Callback Hygiene:
 * 1. \ref ISettings::unsubscribeToChangeEvents may be called from within the callback to unregister the called
 *    subscription or any other subscription.
 * 2. Unregistering the subscription ensures that it will never be called again, and any calls in process on another
 *    thread will complete before \ref ISettings::unsubscribeToChangeEvents returns.
 * 3. The settings database lock is not held while the callback is called, but may be temporarily taken for API calls
 *    within the callback.
 *
 * @see carb::dictionary::IDictionary
 */
struct ISettings
{
    CARB_PLUGIN_INTERFACE("carb::settings::ISettings", 1, 0)

    /**
     * Returns dictionary item type of the key at the given path.
     *
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return \ref carb::dictionary::ItemType of value at \p path; if \p path is `nullptr` or does not exist,
     *   \ref carb::dictionary::ItemType::eCount is returned.
     */
    dictionary::ItemType(CARB_ABI* getItemType)(const char* path);

    /**
     * Checks if the item could be accessible as the provided type, either directly, or via conversion.
     *
     * @param itemType Item type to check for.
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return True if accessible, or false otherwise.
     */
    bool(CARB_ABI* isAccessibleAs)(dictionary::ItemType itemType, const char* path);

    /**
     * Creates a dictionary (or changes an existing value to a dictionary) at the specified path.
     *
     * If the setting value previously existed as a dictionary, it is not modified. If the setting value exists as
     * another type, it is destroyed and a new setting value is created as an empty dictionary. If the setting value did
     * not exist, it is created as an empty dictionary. If any levels of the path hierarchy did not exist, they are
     * created as dictionary items.
     *
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`. Providing the root level ("/"
     * or "") is undefined behavior.
     */
    void(CARB_ABI* setDictionary)(const char* path);

    /**
     * Attempts to get the supplied item as integer, either directly or via conversion.
     *
     * @see carb::dictionary::IDictionary::getAsInt64
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return The 64-bit integer value of the value at \p path. If \p path is invalid, non-existent, or cannot be
     *   converted to an integer, `0` is returned.
     */
    int64_t(CARB_ABI* getAsInt64)(const char* path);

    /**
     * Sets the integer value at the supplied path.
     *
     * If an item was already present, changes its original type to 'integer'. If the present item is a dictionary with
     * children, all children are destroyed. If \p path doesn't exist, creates integer item, and all the required items
     * along the path if necessary.
     *
     * @see carb::dictionary::IDictionary::setInt64
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param value Integer value that will be stored to the supplied path.
     */
    void(CARB_ABI* setInt64)(const char* path, int64_t value);


    /**
     * Attempts to get the supplied item as `int32_t`, either directly or via conversion.
     *
     * @warning The value is truncated by casting to 32-bits. In debug builds, an assert occurs if the value read from
     *   the item would be truncated.
     * @see carb::dictionary::IDictionary::getAsInt
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return The `int32_t` value of the setting value at \p path. If \p path is invalid, non-existent, or cannot be
     *   converted to an `int32_t`, `0` is returned.
     */
    int32_t getAsInt(const char* path);

    /**
     * Sets the integer value at the supplied path.
     *
     * If an item was already present, changes its original type to 'integer'. If the present item is a dictionary with
     * children, all children are destroyed. If \p path doesn't exist, creates integer item, and all the required items
     * along the path if necessary.
     *
     * @see carb::dictionary::IDictionary::setInt
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param value Integer value that will be stored to the supplied path.
     */
    void setInt(const char* path, int32_t value);

    /**
     * Attempts to get the supplied item as `double`, either directly or via conversion.
     *
     * @see carb::dictionary::IDictionary::getAsFloat64
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return The `double` value of the setting value at \p path. If \p path is invalid, non-existent, or cannot be
     *   converted to a `double`, `0.0` is returned.
     */
    double(CARB_ABI* getAsFloat64)(const char* path);

    /**
     * Sets the floating point value at the supplied path.
     *
     * If an item was already present, changes its original type to 'double'. If the present item is a dictionary with
     * children, all children are destroyed. If \p path doesn't exist, creates 'double' item, and all the required items
     * along the path if necessary.
     *
     * @see carb::dictionary::IDictionary::setFloat64
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param value Floating point value that will be stored to the supplied path.
     */
    void(CARB_ABI* setFloat64)(const char* path, double value);

    /**
     * Attempts to get the supplied item as `float`, either directly or via conversion.
     *
     * @see carb::dictionary::IDictionary::getAsFloat
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return The `float` value of the setting value at \p path. If \p path is invalid, non-existent, or cannot be
     *   converted to a `float`, `0.0f` is returned.
     */
    float getAsFloat(const char* path);

    /**
     * Sets the floating point value at the supplied path.
     *
     * If an item was already present, changes its original type to 'float'. If the present item is a dictionary with
     * children, all children are destroyed. If \p path doesn't exist, creates 'float' item, and all the required items
     * along the path if necessary.
     *
     * @see carb::dictionary::IDictionary::setFloat
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param value Floating point value that will be stored to the supplied path.
     */
    void setFloat(const char* path, float value);

    /**
     * Attempts to get the supplied item as `bool`, either directly or via conversion.
     *
     * @see carb::dictionary::IDictionary::getAsBool
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return The `bool` value of the setting value at \p path. If \p path is invalid, non-existent, or cannot be
     *   converted to a `bool`, `false` is returned. See \ref carb::dictionary::IDictionary::getAsBool for an
     *   explanation of how different item types are converted.
     */
    bool(CARB_ABI* getAsBool)(const char* path);

    /**
     * Sets the boolean value at the supplied path.
     *
     * If an item was already present, changes its original type to 'bool'. If the present item is a dictionary with
     * children, all children are destroyed. If \p path doesn't exist, creates 'bool' item, and all the required items
     * along the path if necessary.
     *
     * @see carb::dictionary::IDictionary::setBool
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param value Boolean value that will be stored to the supplied path.
     */
    void(CARB_ABI* setBool)(const char* path, bool value);

    //! @private
    const char*(CARB_ABI* internalCreateStringBufferFromItemValue)(const char* path, size_t* pStringLen);

    /**
     * Attempts to create a new string buffer with a value, either the real string value or a string resulting
     * from converting the item value to a string.
     *
     * @note Please use \ref destroyStringBuffer() to free the created buffer.
     *
     * @see carb::dictionary::IDictionary::createStringBufferFromItemValue() carb::settings::getStringFromItemValue()
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param pStringLen (optional) Receives the length of the string. This can be useful if the string contains NUL
     * characters (i.e. byte data).
     * @return Pointer to the created string buffer if applicable, nullptr otherwise.
     */
    const char* createStringBufferFromItemValue(const char* path, size_t* pStringLen = nullptr) const
    {
        return internalCreateStringBufferFromItemValue(path, pStringLen);
    }

    //! @private
    const char*(CARB_ABI* internalGetStringBuffer)(const char* path, size_t* pStringLen);

    /**
     * Returns internal raw data pointer to the string value of an item. Thus, doesn't perform any
     * conversions.
     *
     * @warning This function returns the internal string buffer. Another thread may change the setting value which can
     * cause the string buffer to be destroyed. It is recommended to take a \ref ScopedRead lock around calling this
     * function and using the return value.
     *
     * @see carb::dictionary::IDictionary::getStringBuffer() carb::settings::getString()
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param pStringLen (optional) Receives the length of the string. This can be useful if the string contains NUL
     * characters (i.e. byte data).
     * @return Raw pointer to the internal string value if applicable, nullptr otherwise.
     */
    const char* getStringBuffer(const char* path, size_t* pStringLen = nullptr) const
    {
        return internalGetStringBuffer(path, pStringLen);
    }

    //! @private
    void(CARB_ABI* internalSetString)(const char* path, const char* value, size_t stringLen);

    /**
     * Sets a string value at the given path.
     *
     * The given @p path is walked and any dictionary items are changed or created to create the path. If the @p path
     * item itself exists but is not of type `eString` it is changed to be `eString`. Change notifications for
     * subscriptions are queued. The given string is then set in the item at @p path. If @p value is \c nullptr, the
     * string item will store an empty string.
     *
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param value String value that will be stored to the supplied path. `nullptr` is interpreted as an empty string.
     * @param stringLen (optional) The length of the string at \p value to copy. This can be useful if only a portion
     * of the string should be copied, or if \p value contains NUL characters (i.e. byte data). The default value of
     * `size_t(-1)` treats \p value as a NUL-terminated string.
     */
    void setString(const char* path, const char* value, size_t stringLen = size_t(-1)) const
    {
        internalSetString(path, value, stringLen);
    }

    /**
     * Reads the value from a path, converting if necessary.
     * @tparam T The type of item to read. Must be a supported type.
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @returns The value read from the \p path.
     */
    template <typename T>
    T get(const char* path);

    /**
     * Sets the value at a path.
     * @tparam T The type of item to read. Must be a supported type.
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param value The value to store in the item at \p path.
     */
    template <typename T>
    void set(const char* path, T value);

    /**
     * Checks if the item could be accessible as array, i.e. all child items names are valid
     * contiguous non-negative integers starting with zero.
     *
     * @see carb::dictionary::IDictionary::isAccessibleAsArray
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return True if accessible, or false otherwise.
     */
    bool(CARB_ABI* isAccessibleAsArray)(const char* path);

    /**
     * Checks if the item could be accessible as the array of items of provided type,
     * either directly, or via conversion of all elements.
     *
     * @see carb::dictionary::IDictionary::isAccessibleAsArrayOf
     * @param itemType Item type to check for.
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return True if a valid array and with all items accessible, or false otherwise.
     */
    bool(CARB_ABI* isAccessibleAsArrayOf)(dictionary::ItemType itemType, const char* path);

    /**
     * Checks if the all the children of the item have array-style indices. If yes, returns the number
     * of children (array elements).
     *
     * @see carb::dictionary::IDictionary::getArrayLength
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return Number of array elements if applicable, or 0 otherwise.
     */
    size_t(CARB_ABI* getArrayLength)(const char* path);

    /**
     * Runs through all the array elements and infers a type that is most suitable for the
     * array.
     *
     * The rules are thus:
     * - Strings attempt to convert to float or bool if possible.
     * - The converted type of the first element is the initial type.
     * - If the initial type is a \ref dictionary::ItemType::eBool and later elements can be converted to
     *   \ref dictionary::ItemType::eBool without losing precision, \ref dictionary::ItemType::eBool is kept. (String
     *   variants of "true"/"false", or values exactly equal to 0/1)
     * - Elements of type \ref dictionary::ItemType::eFloat can convert to \ref dictionary::ItemType::eInt if they don't
     *   lose precision.
     *
     * @see dictionary::IDictionary::getPreferredArrayType
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return \ref dictionary::ItemType that is most suitable for the array, or \ref dictionary::ItemType::eCount on
     *   failure.
     */
    dictionary::ItemType(CARB_ABI* getPreferredArrayType)(const char* path);

    /**
     * Attempts to read an array item as a 64-bit integer.
     *
     * Attempts to read the path and array index as an integer, either directly or via conversion, considering the
     * item at path to be an array, and using the supplied index to access its child.
     * Default value (`0`) is returned if \p path doesn't exist, index doesn't exist, or there is a conversion
     * failure.
     *
     * @see dictionary::IDictionary::getAsInt64At
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @return Integer value, either raw value or cast from the real item type. Zero is returned if \p path doesn't
     *   exist, is not an array, \p index doesn't exist in the array, or a conversion error occurs.
     */
    int64_t(CARB_ABI* getAsInt64At)(const char* path, size_t index);

    /**
     * Attempts to set a 64-bit integer value in an array item.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). \p index is converted to a string and appended to \p path as a child path name. The
     * child path is created as `eInt` or changed to be `eInt`, and the value is set from \p value. Subscription
     * notifications are triggered when the current thread no longer has any settings locks.
     *
     * @see dictionary::IDictionary::setInt64At
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @param value Integer value that will be stored to the supplied path.
     */
    void(CARB_ABI* setInt64At)(const char* path, size_t index, int64_t value);

    /**
     * Attempts to read an array item as a 32-bit integer.
     *
     * Attempts to read the path and array index as an integer, either directly or via conversion, considering the
     * item at path to be an array, and using the supplied index to access its child.
     * Default value (`0`) is returned if \p path doesn't exist, index doesn't exist, or there is a conversion
     * failure.
     *
     * @warning The value is truncated by casting to 32-bits. In debug builds, an assert occurs if the value read from
     *   the item would be truncated.
     * @see dictionary::IDictionary::getAsIntAt()
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @return Integer value, either raw value or cast from the real item type. Zero is returned if \p path doesn't
     *   exist, is not an array, \p index doesn't exist in the array, or a conversion error occurs.
     */
    int32_t getAsIntAt(const char* path, size_t index);

    /**
     * Attempts to set a 32-bit integer value in an array item.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). \p index is converted to a string and appended to \p path as a child path name. The
     * child path is created as `eInt` or changed to be `eInt`, and the value is set from \p value. Subscription
     * notifications are triggered when the current thread no longer has any settings locks.
     *
     * @see dictionary::IDictionary::setIntAt()
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @param value Integer value that will be stored to the supplied path.
     */
    void setIntAt(const char* path, size_t index, int32_t value);

    /**
     * Fills the given buffer with the array values from the given path.
     *
     * If the provided @p path is not a dictionary item, @p arrayOut is not modified. If the array item contains more
     * items than @p arrayBufferLength, a warning message is logged and items past the end are ignored.
     * @warning Dictionary items that are not arrays will only have child keys which are convertible to array indices
     *   written to @p arrayOut. For example, if @p path is the location of a dictionary item with keys '5' and '10',
     *   only those items will be written into @p arrayOut; the other indices will not be written. It is highly
     *   recommended to take a \ref ScopedRead lock around calling this function, and call only if
     *   \ref isAccessibleAsArray or \ref isAccessibleAsArrayOf return \c true.
     *
     * @see carb::dictionary::IDictionary::getAsInt64Array
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param arrayOut Array buffer to fill with integer values.
     * @param arrayBufferLength Size of the supplied array buffer.
     */
    void(CARB_ABI* getAsInt64Array)(const char* path, int64_t* arrayOut, size_t arrayBufferLength);

    /**
     * Creates or updates an item to be an integer array.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). If \p path already exists as a `eDictionary` it is cleared (change notifications
     * will be queued for any child items that are destroyed). New child items are created for all elements of the given
     * @p array.
     *
     * @see carb::dictionary::IDictionary::setInt64Array
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param array Integer array to copy values from.
     * @param arrayLength Array length.
     */
    void(CARB_ABI* setInt64Array)(const char* path, const int64_t* array, size_t arrayLength);

    /**
     * Fills the given buffer with the array values from the given path.
     *
     * If the provided @p path is not a dictionary item, @p arrayOut is not modified. If the array item contains more
     * items than @p arrayBufferLength, a warning message is logged and items past the end are ignored.
     * @warning Dictionary items that are not arrays will only have child keys which are convertible to array indices
     *   written to @p arrayOut. For example, if @p path is the location of a dictionary item with keys '5' and '10',
     *   only those items will be written into @p arrayOut; the other indices will not be written. It is highly
     *   recommended to take a \ref ScopedRead lock around calling this function, and call only if
     *   \ref isAccessibleAsArray or \ref isAccessibleAsArrayOf return \c true.
     * @warning Any integer element that does not fit within \c int32_t is truncated by casting.
     *
     * @see carb::dictionary::IDictionary::getAsIntArray
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param arrayOut Array buffer to fill with integer values.
     * @param arrayBufferLength Size of the supplied array buffer.
     */
    void(CARB_ABI* getAsIntArray)(const char* path, int32_t* arrayOut, size_t arrayBufferLength);

    /**
     * Creates or updates an item to be an integer array.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). If \p path already exists as a `eDictionary` it is cleared (change notifications
     * will be queued for any child items that are destroyed). New child items are created for all elements of the given
     * @p array.
     *
     * @see carb::dictionary::IDictionary::setIntArray
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param array Integer array to copy values from.
     * @param arrayLength Array length.
     */
    void(CARB_ABI* setIntArray)(const char* path, const int32_t* array, size_t arrayLength);

    /**
     * Attempts to read an array item as a `double`.
     *
     * Attempts to read the path and array index as a `double`, either directly or via conversion, considering the
     * item at path to be an array, and using the supplied index to access its child.
     * Default value (`0.0`) is returned if \p path doesn't exist, index doesn't exist, or there is a conversion
     * failure.
     *
     * @see carb::dictionary::IDictionary::getAsFloat64At
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return `double` value, either raw value or cast from the real item type.
     */
    double(CARB_ABI* getAsFloat64At)(const char* path, size_t index);

    /**
     * Attempts to set a `double` value in an array item.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). \p index is converted to a string and appended to \p path as a child path name. The
     * child path is created as `eFloat` or changed to be `eFloat`, and the value is set from \p value. Subscription
     * notifications are triggered when the current thread no longer has any settings locks.
     *
     * @see carb::dictionary::IDictionary::setFloat64At
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @param value `double` value that will be stored to the supplied path.
     */
    void(CARB_ABI* setFloat64At)(const char* path, size_t index, double value);

    /**
     * Attempts to read an array item as a `float`.
     *
     * Attempts to read the path and array index as a `float`, either directly or via conversion, considering the
     * item at path to be an array, and using the supplied index to access its child.
     * Default value (`0.0f`) is returned if \p path doesn't exist, index doesn't exist, or there is a conversion
     * failure.
     *
     * @see carb::dictionary::IDictionary::getAsFloatAt()
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @return `float` value, either raw value or cast from the real item type.
     */
    float getAsFloatAt(const char* path, size_t index);

    /**
     * Attempts to set a `float` value in an array item.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). \p index is converted to a string and appended to \p path as a child path name. The
     * child path is created as `eFloat` or changed to be `eFloat`, and the value is set from \p value. Subscription
     * notifications are triggered when the current thread no longer has any settings locks.
     *
     * @see carb::dictionary::IDictionary::setFloatAt()
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @param value Floating point value that will be stored to the supplied path.
     */
    void setFloatAt(const char* path, size_t index, float value);

    /**
     * Fills the given buffer with the array values from the given path.
     *
     * If the provided @p path is not a dictionary item, @p arrayOut is not modified. If the array item contains more
     * items than @p arrayBufferLength, a warning message is logged and items past the end are ignored.
     * @warning Dictionary items that are not arrays will only have child keys which are convertible to array indices
     *   written to @p arrayOut. For example, if @p path is the location of a dictionary item with keys '5' and '10',
     *   only those items will be written into @p arrayOut; the other indices will not be written. It is highly
     *   recommended to take a \ref ScopedRead lock around calling this function, and call only if
     *   \ref isAccessibleAsArray or \ref isAccessibleAsArrayOf return \c true.
     *
     * @see carb::dictionary::IDictionary::getAsFloat64Array
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param arrayOut Array buffer to fill with floating point values.
     * @param arrayBufferLength Size of the supplied array buffer.
     */
    void(CARB_ABI* getAsFloat64Array)(const char* path, double* arrayOut, size_t arrayBufferLength);

    /**
     * Creates or updates an item to be a `double` array.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). If \p path already exists as a `eDictionary` it is cleared (change notifications
     * will be queued for any child items that are destroyed). New child items are created for all elements of the given
     * @p array.
     *
     * @see carb::dictionary::IDictionary::setFloat64Array
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param array Floating point array to copy values from.
     * @param arrayLength Array length.
     */
    void(CARB_ABI* setFloat64Array)(const char* path, const double* array, size_t arrayLength);

    /**
     * Fills the given buffer with the array values from the given path.
     *
     * If the provided @p path is not a dictionary item, @p arrayOut is not modified. If the array item contains more
     * items than @p arrayBufferLength, a warning message is logged and items past the end are ignored.
     * @warning Dictionary items that are not arrays will only have child keys which are convertible to array indices
     *   written to @p arrayOut. For example, if @p path is the location of a dictionary item with keys '5' and '10',
     *   only those items will be written into @p arrayOut; the other indices will not be written. It is highly
     *   recommended to take a \ref ScopedRead lock around calling this function, and call only if
     *   \ref isAccessibleAsArray or \ref isAccessibleAsArrayOf return \c true.
     * @warning Any element that does not fit within \c float is truncated by casting.
     *
     * @see carb::dictionary::IDictionary::getAsFloatArray
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param arrayOut Array buffer to fill with floating point values.
     * @param arrayBufferLength Size of the supplied array buffer.
     */
    void(CARB_ABI* getAsFloatArray)(const char* path, float* arrayOut, size_t arrayBufferLength);

    /**
     * Creates or updates an item to be a `float` array.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). If \p path already exists as a `eDictionary` it is cleared (change notifications
     * will be queued for any child items that are destroyed). New child items are created for all elements of the given
     * @p array.
     *
     * @see carb::dictionary::IDictionary::setFloatArray
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param array Floating point array to copy values from.
     * @param arrayLength Array length.
     */
    void(CARB_ABI* setFloatArray)(const char* path, const float* array, size_t arrayLength);

    /**
     * Attempts to read an array item as a `bool`.
     *
     * Attempts to read the path and array index as a `bool`, either directly or via conversion, considering the
     * item at path to be an array, and using the supplied index to access its child.
     * Default value (`false`) is returned if \p path doesn't exist, index doesn't exist, or there is a conversion
     * failure.
     *
     * @see carb::dictionary::IDictionary::getAsBoolAt
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @return Boolean value, either raw value or cast from the real item type.
     */
    bool(CARB_ABI* getAsBoolAt)(const char* path, size_t index);

    /**
     * Attempts to set a `bool` value in an array item.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). \p index is converted to a string and appended to \p path as a child path name. The
     * child path is created as `eBool` or changed to be `eBool`, and the value is set from \p value. Subscription
     * notifications are triggered when the current thread no longer has any settings locks.
     *
     * @see carb::dictionary::IDictionary::setBoolAt
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @param value Boolean value that will be stored to the supplied path.
     */
    void(CARB_ABI* setBoolAt)(const char* path, size_t index, bool value);

    /**
     * Fills the given buffer with the array values from the given path.
     *
     * If the provided @p path is not a dictionary item, @p arrayOut is not modified. If the array item contains more
     * items than @p arrayBufferLength, a warning message is logged and items past the end are ignored.
     * @warning Dictionary items that are not arrays will only have child keys which are convertible to array indices
     *   written to @p arrayOut. For example, if @p path is the location of a dictionary item with keys '5' and '10',
     *   only those items will be written into @p arrayOut; the other indices will not be written. It is highly
     *   recommended to take a \ref ScopedRead lock around calling this function, and call only if
     *   \ref isAccessibleAsArray or \ref isAccessibleAsArrayOf return \c true.
     *
     * @see carb::dictionary::IDictionary::getAsBoolArray
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param arrayOut Array buffer to fill with boolean values.
     * @param arrayBufferLength Size of the supplied array buffer.
     */
    void(CARB_ABI* getAsBoolArray)(const char* path, bool* arrayOut, size_t arrayBufferLength);

    /**
     * Creates or updates an item to be a `bool` array.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). If \p path already exists as a `eDictionary` it is cleared (change notifications
     * will be queued for any child items that are destroyed). New child items are created for all elements of the given
     * @p array.
     *
     * @see carb::dictionary::IDictionary::setBoolArray
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param array Boolean array to copy values from.
     * @param arrayLength Array length.
     */
    void(CARB_ABI* setBoolArray)(const char* path, const bool* array, size_t arrayLength);

    //! @private
    const char*(CARB_ABI* internalCreateStringBufferFromItemValueAt)(const char* path, size_t index, size_t* pStringLen);

    /**
     * Attempts to create a new string buffer with a value, either the real string value or a string resulting
     * from converting the item value to a string.
     *
     * This function effectively converts \p index to a string, appends it to \p path after a path separator ('/'), and
     * calls \ref createStringBufferFromItemValue().
     *
     * @see carb::dictionary::IDictionary::createStringBufferFromItemValueAt()
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @param pStringLen (optional) Receives the length of the string. This can be useful if the string contains NUL
     * characters (i.e. byte data). Undefined if the function returns nullptr.
     * @return Pointer to the created string buffer if applicable, nullptr otherwise. Non-nullptr return values must be
     *   passed to \ref destroyStringBuffer to dispose of when finished.
     *
     * @note It is undefined to create `std::string` out of nullptr. For using the value as `std::string`, see
     *   \ref carb::settings::getStringFromItemValueAt()
     */
    const char* createStringBufferFromItemValueAt(const char* path, size_t index, size_t* pStringLen = nullptr) const
    {
        return internalCreateStringBufferFromItemValueAt(path, index, pStringLen);
    }

    //! @private
    const char*(CARB_ABI* internalGetStringBufferAt)(const char* path, size_t index, size_t* pStringLen);

    /**
     * Returns internal raw data pointer to the string value of an item in an array. Thus, doesn't perform any
     * conversions.
     *
     * @warning This function returns the internal string buffer. Another thread may change the setting value which can
     * cause the string buffer to be destroyed. It is recommended to take a \ref ScopedRead lock around calling this
     * function and using the return value.
     *
     * @see carb::dictionary::IDictionary::getStringBufferAt()
     * @param path Settings database key path of an array item (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @param pStringLen (optional) Receives the length of the string. This can be useful if the string contains NUL
     * characters (i.e. byte data). Undefined if the function returns nullptr.
     * @return Raw pointer to the internal string value if applicable, nullptr otherwise.
     *
     * @note It is undefined to create std::string out of nullptr. For using the value as std::string, @see
     * carb::settings::getStringAt()
     */
    const char* getStringBufferAt(const char* path, size_t index, size_t* pStringLen = nullptr) const
    {
        return internalGetStringBufferAt(path, index, pStringLen);
    }

    //! @private
    void(CARB_ABI* internalSetStringAt)(const char* path, size_t index, const char* value, size_t stringLen);

    /**
     * Sets a string value at the given path.
     *
     * The given @p path is walked and any dictionary items are changed or created to create the path. @p index is
     * converted to a string and appended to @p path. If the item at this composite path
     * exists but is not of type `eString` it is changed to be `eString`. Change notifications for
     * subscriptions are queued. The given string is then set in the item at the composite path path. If @p value is
     * \c nullptr, the string item will store an empty string.
     *
     * @see carb::dictionary::IDictionary::setStringAt()
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param index Index of the element in array.
     * @param value String value that will be stored to the supplied path.
     * @param stringLen (optional) The length of the string at \p value to copy. This can be useful if only a portion
     * of the string should be copied, or if \p value contains NUL characters (i.e. byte data). The default value of
     * size_t(-1) treats \p value as a NUL-terminated string.
     */
    void setStringAt(const char* path, size_t index, const char* value, size_t stringLen = size_t(-1)) const
    {
        internalSetStringAt(path, index, value, stringLen);
    }

    /**
     * Returns internal raw data pointers to the string value of all child items of an array. Thus, doesn't perform any
     * conversions.
     *
     * @warning This function returns the internal string buffer(s) for several items. Another thread may change the
     * setting value which can cause the string buffer to be destroyed. It is recommended to take a \ref ScopedRead lock
     * around calling this function and use of the filled items in @p arrayOut.
     *
     * If the provided @p path is not a dictionary item, @p arrayOut is not modified. If the array item contains more
     * items than @p arrayBufferLength, a warning message is logged and items past the end are ignored.
     * @warning Dictionary items that are not arrays will only have child keys which are convertible to array indices
     *   written to @p arrayOut. For example, if @p path is the location of a dictionary item with keys '5' and '10',
     *   only those items will be written into @p arrayOut; the other indices will not be written. It is highly
     *   recommended to take a \ref ScopedRead lock around calling this function, and call only if
     *   \ref isAccessibleAsArray or \ref isAccessibleAsArrayOf return \c true.
     *
     * @see carb::dictionary::IDictionary::getStringBufferArray carb::settings::getStringArray()
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param arrayOut Array buffer to fill with char buffer pointer values.
     * @param arrayBufferLength Size of the supplied array buffer.
     */
    void(CARB_ABI* getStringBufferArray)(const char* path, const char** arrayOut, size_t arrayBufferLength);

    /**
     * Creates or updates an item to be a string array.
     *
     * Ensures that the given \p path exists as an `eDictionary`, changing types as the path is walked (and triggering
     * subscription notifications). If \p path already exists as a `eDictionary` it is cleared (change notifications
     * will be queued for any child items that are destroyed). New child items are created for all elements of the given
     * @p array.
     *
     * @see carb::dictionary::IDictionary::setStringArray
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param array String array to copy values from.
     * @param arrayLength Array length.
     */
    void(CARB_ABI* setStringArray)(const char* path, const char* const* array, size_t arrayLength);

    /**
     * Creates or updates an item to be an array of the given type with provided values.
     *
     * This is a helper function that will call the appropriate API function based on \c SettingArrayType:
     * * `bool`: \ref setBoolArray
     * * `int32_t`: \ref setIntArray
     * * `int64_t`: \ref setInt64Array
     * * `float`: \ref setFloatArray
     * * `double`: \ref setFloat64Array
     * * `const char*`: \ref setStringArray
     *
     * @see carb::dictionary::IDictionary::setArray()
     * @tparam SettingArrayType The type of elements.
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param array Array to copy values from.
     * @param arrayLength Number of items in @p array.
     */
    template <typename SettingArrayType>
    void setArray(const char* path, const SettingArrayType* array, size_t arrayLength);

    /**
     * Creates a transaction.
     *
     * A transaction is an asynchronous settings database that can be applied at a later time as one atomic unit with
     * \ref commitTransaction. A transaction may be retained and committed several times. A transaction must be disposed
     * of with \ref destroyTransaction when it is no longer needed.
     *
     * Values can be set within the transaction with \ref setInt64Async, \ref setFloat64Async, \ref setBoolAsync and
     * \ref setStringAsync.
     *
     * @returns An opaque \ref Transaction pointer.
     */
    Transaction*(CARB_ABI* createTransaction)();

    /**
     * Destroys a transaction.
     *
     * Destroys a transaction previously created with \ref createTransaction.
     * @warning After calling this function, the given @p transaction should no longer be used or undefined behavior
     * will occur.
     * @param transaction The \ref Transaction to destroy.
     */
    void(CARB_ABI* destroyTransaction)(Transaction* transaction);

    /**
     * Commits a transaction.
     *
     * This atomically copies all of the values set in the transaction to the settings database. This is done as via:
     * ```
     * // note: transaction->database is expository only
     * update("/", transaction->database, nullptr, carb::dictionary::overwriteOriginalWithArrayHandling,
     *     carb::getCachedInterface<carb::dictionary::IDictionary>());
     * ```
     * Values can be set within the transaction with \ref setInt64Async, \ref setFloat64Async, \ref setBoolAsync and
     * \ref setStringAsync.
     *
     * This function can be called multiple times for a given @p transaction that has not been destroyed.
     *
     * Change notifications are queued.
     *
     * @param transaction The \ref Transaction to commit.
     */
    void(CARB_ABI* commitTransaction)(Transaction* transaction);

    /**
     * Sets a value in a Transaction to be applied at a later time.
     *
     * The value is not committed to the settings database until \ref commitTransaction is called.
     * @note Values cannot be read or deleted from a \ref Transaction.
     *
     * @param transaction The \ref Transaction previously created with \ref createTransaction.
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`. The settings database is not
     *   modified until \ref commitTransaction is called.
     * @param value The value to store in the \p transaction.
     */
    void(CARB_ABI* setInt64Async)(Transaction* transaction, const char* path, int64_t value);
    //! @copydoc setInt64Async
    void(CARB_ABI* setFloat64Async)(Transaction* transaction, const char* path, double value);
    //! @copydoc setInt64Async
    void(CARB_ABI* setBoolAsync)(Transaction* transaction, const char* path, bool value);
    //! @copydoc setInt64Async
    void(CARB_ABI* setStringAsync)(Transaction* transaction, const char* path, const char* value);

    /**
     * Subscribes to change events about a specific item.
     *
     * When finished with the subscription, call \ref unsubscribeToChangeEvents.
     *
     * @see carb::dictionary::IDictionary::subscribeToNodeChangeEvents
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param onChangeEventFn The function to call when a change event occurs.
     * @param userData User-specific data that will be provided to \p onChangeEventFn.
     * @return A subscription handle that can be used with \ref unsubscribeToChangeEvents.
     */
    dictionary::SubscriptionId*(CARB_ABI* subscribeToNodeChangeEvents)(const char* path,
                                                                       dictionary::OnNodeChangeEventFn onChangeEventFn,
                                                                       void* userData);

    /**
     * Subscribes to change events for all items in a subtree.
     *
     * All items including and under @p path will trigger change notifications.
     *
     * When finished with the subscription, call \ref unsubscribeToChangeEvents.
     *
     * @see carb::dictionary::IDictionary::subscribeToTreeChangeEvents
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param onChangeEventFn The function to call when a change event occurs.
     * @param userData User-specific data that will be provided to \p onChangeEventFn.
     * @return A subscription handle that can be used with \ref unsubscribeToChangeEvents.
     */
    dictionary::SubscriptionId*(CARB_ABI* subscribeToTreeChangeEvents)(const char* path,
                                                                       dictionary::OnTreeChangeEventFn onChangeEventFn,
                                                                       void* userData);

    /**
     * Unsubscribes from change events.
     *
     * It is safe to call this function from within a subscription callback. Once it returns, this function guarantees
     * that the subscription callback has exited (only if being called from another thread) and will never be called
     * again.
     *
     * @param subscriptionId The subscription handle from \ref subscribeToNodeChangeEvents or
     *   \ref subscribeToTreeChangeEvents
     */
    void(CARB_ABI* unsubscribeToChangeEvents)(dictionary::SubscriptionId* subscriptionId);

    /**
     * Merges the source item into the settings database. Destination path may be non-existing, then
     * missing items in the path will be created as dictionaries.
     *
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`. Used as the destination
     *   location within the settings database. "/" is considered to be the root.
     * @param dictionary The \ref carb::dictionary::Item that is the base of the items to merge into the setting
     *   database.
     * @param dictionaryPath A child path of @p dictionary to use as the root for merging. May be `nullptr` in order to
     *   use @p dictionary directly as the root.
     * @param onUpdateItemFn Function that will tell whether the update should overwrite the destination item with the
     *   source item. See \ref carb::dictionary::keepOriginal(), \ref carb::dictionary::overwriteOriginal(), or
     *   \ref carb::dictionary::overwriteOriginalWithArrayHandling().
     * @param userData User data pointer that will be passed into the onUpdateItemFn.
     */
    void(CARB_ABI* update)(const char* path,
                           const dictionary::Item* dictionary,
                           const char* dictionaryPath,
                           dictionary::OnUpdateItemFn onUpdateItemFn,
                           void* userData);

    /**
     * Accesses the settings database as a dictionary Item.
     *
     * This allows use of \ref carb::dictionary::IDictionary functions directly.
     *
     * @warning The root \ref dictionary::Item is owned by \c ISettings and must not be altered or destroyed.
     * @note It is highly recommended to take a \ref ScopedRead or \ref ScopedWrite lock while working with the setting
     *   database directly.
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`. Used as the destination
     *   location within the settings database. "/" is considered to be the root.
     * @returns A \ref carb::dictionary::Item that can be used directly with the \ref carb::dictionary::IDictionary
     *   interface.
     */
    const dictionary::Item*(CARB_ABI* getSettingsDictionary)(const char* path);

    /**
     * Takes a snapshot of a portion of the settings database as a dictionary Item.
     *
     * @param path Settings database key path (i.e. "/log/level") to consider the root for the copy. "/" is considered
     *   to be the root (will copy the entire settings database).
     * @returns A \ref carb::dictionary::Item that is a separate copy of the requested portion of the settings database.
     *   When no longer needed, this can be passed to \ref carb::dictionary::IDictionary::destroyItem.
     */
    dictionary::Item*(CARB_ABI* createDictionaryFromSettings)(const char* path);

    /**
     * Destroys a setting item and queues change notifications.
     *
     * @warning Any string buffers or \ref carb::dictionary::Item pointers to the referenced path become invalid and
     *   their use is undefined behavior.
     * @see carb::dictionary::IDictionary::destroyItem
     * @param path Settings database key path (i.e. "/log/level") to destroy. "/" is considered
     *   to be the root (will clear the entire settings database without actually destroying the root).
     */
    void(CARB_ABI* destroyItem)(const char* path);

    /**
     * Frees a string buffer.
     *
     * The string buffers are created by \ref createStringBufferFromItemValue() or
     *   \ref createStringBufferFromItemValueAt().
     * @see carb::dictionary::IDictionary::destroyStringBuffer
     * @param stringBuffer String buffer to destroy. Undefined behavior results if this is not a value returned from one
     *   of the functions listed above.
     */
    void(CARB_ABI* destroyStringBuffer)(const char* stringBuffer);

    /**
     * Performs a one-time initialization from a given dictionary item.
     *
     * @note This function may only be called once. Subsequent calls will result in an error message logged.
     * @param dictionary The \ref carb::dictionary::Item to initialize the settings database from. The items are copied
     *   into the root of the settings database. This item is not retained and may be destroyed immediately after this
     *   function returns.
     */
    void(CARB_ABI* initializeFromDictionary)(const dictionary::Item* dictionary);

    /**
     * Sets a value at the given path, if and only if one does not already exist.
     *
     * Atomically checks if a value exists at the given @p path, and if so, exits without doing anything. Otherwise, any
     * required dictionary items are created while walking @p path and @p value is stored. Change notifications are
     * queued.
     *
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param value Value that will be stored at the given @p path in the settings database.
     */
    void setDefaultInt64(const char* path, int64_t value);
    //! @copydoc setDefaultInt64
    void setDefaultInt(const char* path, int32_t value);
    //! @copydoc setDefaultInt64
    void setDefaultFloat64(const char* path, double value);
    //! @copydoc setDefaultInt64
    void setDefaultFloat(const char* path, float value);
    //! @copydoc setDefaultInt64
    void setDefaultBool(const char* path, bool value);
    //! @copydoc setDefaultInt64
    void setDefaultString(const char* path, const char* value);

    //! @copydoc setDefaultInt64
    //! @tparam SettingType The type of @p value.
    template <typename SettingType>
    void setDefault(const char* path, SettingType value);

    /**
     * Copies values into the setting dictionary, if and only if they don't already exist.
     *
     * Values are checked and copied atomically, and change notifications are queued.
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`. This is the root location at
     *   which \p dictionary is copied in.
     * @param dictionary The \ref dictionary::Item to copy defaults from.
     */
    void setDefaultsFromDictionary(const char* path, const dictionary::Item* dictionary);

    /**
     * Sets an array of values at the given path, if and only if one does not already exist.
     *
     * Atomically checks if a value exists at the given @p path, and if so, exits without doing anything. Otherwise, any
     * required dictionary items are created while walking @p path and @p array is stored. Change notifications are
     * queued.
     *
     * @param path Settings database key path (i.e. "/log/level"). Must not be `nullptr`.
     * @param array Array of values that will be stored at the given @p path in the settings database.
     * @param arrayLength Number of values in @p array.
     */
    void setDefaultInt64Array(const char* path, const int64_t* array, size_t arrayLength);
    //! @copydoc setDefaultInt64Array
    void setDefaultIntArray(const char* path, const int32_t* array, size_t arrayLength);
    //! @copydoc setDefaultInt64Array
    void setDefaultFloat64Array(const char* path, const double* array, size_t arrayLength);
    //! @copydoc setDefaultInt64Array
    void setDefaultFloatArray(const char* path, const float* array, size_t arrayLength);
    //! @copydoc setDefaultInt64Array
    void setDefaultBoolArray(const char* path, const bool* array, size_t arrayLength);
    //! @copydoc setDefaultInt64Array
    void setDefaultStringArray(const char* path, const char* const* array, size_t arrayLength);

    //! @copydoc setDefaultInt64Array
    //! @tparam SettingArrayType The type of the values in @p array.
    template <typename SettingArrayType>
    void setDefaultArray(const char* path, const SettingArrayType* array, size_t arrayLength);
};

/**
 * A helper class for performing a scoped write lock on the settings database.
 */
class ScopedWrite : public carb::dictionary::ScopedWrite
{
public:
    /**
     * RAII constructor. Immediately takes a write lock and holds it until *this is destroyed.
     * @note Settings locks are recursive.
     * @warning If the current thread already owns a read lock (i.e. via \ref ScopedRead), promotion to a write lock
     *   necessitates *releasing* all locks and then waiting for a write lock. This can cause state to change. Always
     *   re-evaluate state if this is the case.
     */
    ScopedWrite()
        : carb::dictionary::ScopedWrite(
              *carb::getCachedInterface<dictionary::IDictionary>(),
              const_cast<dictionary::Item*>(carb::getCachedInterface<settings::ISettings>()->getSettingsDictionary("/")))
    {
    }

    /**
     * RAII constructor. Immediately takes a write lock and holds it until *this is destroyed.
     * This version takes interface pointers for cases where it's not safe to use getCachedInterface().
     * @note Settings locks are recursive.
     * @warning If the current thread already owns a read lock (i.e. via \ref ScopedRead), promotion to a write lock
     *   necessitates *releasing* all locks and then waiting for a write lock. This can cause state to change. Always
     *   re-evaluate state if this is the case.
     * @param settings The @ref ISettings interface to lock with.
     * @param dict The @ref carb::dictionary::IDictionary interface to lock with.
     */
    ScopedWrite(ISettings* settings, dictionary::IDictionary* dict)
        : carb::dictionary::ScopedWrite(*dict, const_cast<dictionary::Item*>(settings->getSettingsDictionary("/")))
    {
    }

    //! Destructor. Releases the write lock.
    ~ScopedWrite() = default;
    CARB_PREVENT_COPY_AND_MOVE(ScopedWrite);
};

/**
 * A helper class for performing a scoped read lock on the settings database.
 */
class ScopedRead : public carb::dictionary::ScopedRead
{
public:
    /**
     * RAII constructor. Immediately takes a read lock and holds it until *this is destroyed.
     * @note Settings locks are recursive.
     */
    ScopedRead()
        : carb::dictionary::ScopedRead(*carb::getCachedInterface<dictionary::IDictionary>(),
                                       carb::getCachedInterface<settings::ISettings>()->getSettingsDictionary("/"))
    {
    }

    /**
     * RAII constructor. Immediately takes a read lock and holds it until *this is destroyed.
     * This version takes interface pointers for cases where it's not safe to use getCachedInterface().
     * @note Settings locks are recursive.
     * @param settings The @ref ISettings interface to lock with.
     * @param dict The @ref carb::dictionary::IDictionary interface to lock with.
     */
    ScopedRead(ISettings* settings, dictionary::IDictionary* dict)
        : carb::dictionary::ScopedRead(*dict, const_cast<dictionary::Item*>(settings->getSettingsDictionary("/")))
    {
    }

    //! Destructor. Releases the read lock.
    ~ScopedRead() = default;
    CARB_PREVENT_COPY_AND_MOVE(ScopedRead);
};

inline int32_t ISettings::getAsInt(const char* path)
{
    auto val = getAsInt64(path);
    CARB_ASSERT(val >= INT_MIN && val <= INT_MAX);
    return int32_t(val);
}

inline void ISettings::setInt(const char* path, int32_t value)
{
    setInt64(path, (int64_t)value);
}

inline float ISettings::getAsFloat(const char* path)
{
    return (float)getAsFloat64(path);
}

inline void ISettings::setFloat(const char* path, float value)
{
    setFloat64(path, (double)value);
}

inline int32_t ISettings::getAsIntAt(const char* path, size_t index)
{
    auto val = getAsInt64At(path, index);
    CARB_ASSERT(val >= INT_MIN && val <= INT_MAX);
    return int32_t(val);
}
inline void ISettings::setIntAt(const char* path, size_t index, int32_t value)
{
    setInt64At(path, index, (int64_t)value);
}

inline float ISettings::getAsFloatAt(const char* path, size_t index)
{
    return (float)getAsFloat64At(path, index);
}
inline void ISettings::setFloatAt(const char* path, size_t index, float value)
{
    setFloat64At(path, index, (double)value);
}

inline void ISettings::setDefaultInt64(const char* path, int64_t value)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setInt64(path, value);
    }
}
inline void ISettings::setDefaultInt(const char* path, int32_t value)
{
    setDefaultInt64(path, (int64_t)value);
}

inline void ISettings::setDefaultFloat64(const char* path, double value)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setFloat64(path, value);
    }
}
inline void ISettings::setDefaultFloat(const char* path, float value)
{
    setDefaultFloat64(path, (double)value);
}

inline void ISettings::setDefaultBool(const char* path, bool value)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setBool(path, value);
    }
}

inline void ISettings::setDefaultString(const char* path, const char* value)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setString(path, value);
    }
}

inline void ISettings::setDefaultsFromDictionary(const char* path, const dictionary::Item* dictionary)
{
    if (dictionary)
    {
        update(path, dictionary, nullptr, dictionary::kUpdateItemKeepOriginal, nullptr);
    }
}

inline void ISettings::setDefaultInt64Array(const char* path, const int64_t* array, size_t arrayLength)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setInt64Array(path, array, arrayLength);
    }
}
inline void ISettings::setDefaultIntArray(const char* path, const int32_t* array, size_t arrayLength)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setIntArray(path, array, arrayLength);
    }
}
inline void ISettings::setDefaultFloat64Array(const char* path, const double* array, size_t arrayLength)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setFloat64Array(path, array, arrayLength);
    }
}
inline void ISettings::setDefaultFloatArray(const char* path, const float* array, size_t arrayLength)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setFloatArray(path, array, arrayLength);
    }
}
inline void ISettings::setDefaultBoolArray(const char* path, const bool* array, size_t arrayLength)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setBoolArray(path, array, arrayLength);
    }
}
inline void ISettings::setDefaultStringArray(const char* path, const char* const* array, size_t arrayLength)
{
    ScopedWrite writeLock;
    dictionary::ItemType itemType = getItemType(path);
    if (itemType == dictionary::ItemType::eCount)
    {
        setStringArray(path, array, arrayLength);
    }
}

#ifndef DOXYGEN_BUILD
template <>
inline int32_t ISettings::get<int>(const char* path)
{
    return getAsInt(path);
}

template <>
inline int64_t ISettings::get<int64_t>(const char* path)
{
    return getAsInt64(path);
}

template <>
inline float ISettings::get<float>(const char* path)
{
    return getAsFloat(path);
}

template <>
inline double ISettings::get<double>(const char* path)
{
    return getAsFloat64(path);
}

template <>
inline bool ISettings::get<bool>(const char* path)
{
    return getAsBool(path);
}

template <>
inline const char* ISettings::get<const char*>(const char* path)
{
    return getStringBuffer(path);
}

template <>
inline void ISettings::set<int32_t>(const char* path, int32_t value)
{
    setInt(path, value);
}

template <>
inline void ISettings::set<int64_t>(const char* path, int64_t value)
{
    setInt64(path, value);
}

template <>
inline void ISettings::set<float>(const char* path, float value)
{
    setFloat(path, value);
}

template <>
inline void ISettings::set<double>(const char* path, double value)
{
    setFloat64(path, value);
}

template <>
inline void ISettings::set<bool>(const char* path, bool value)
{
    setBool(path, value);
}

template <>
inline void ISettings::set<const char*>(const char* path, const char* value)
{
    setString(path, value);
}

template <>
inline void ISettings::setArray(const char* path, const bool* array, size_t arrayLength)
{
    setBoolArray(path, array, arrayLength);
}

template <>
inline void ISettings::setArray(const char* path, const int32_t* array, size_t arrayLength)
{
    setIntArray(path, array, arrayLength);
}

template <>
inline void ISettings::setArray(const char* path, const int64_t* array, size_t arrayLength)
{
    setInt64Array(path, array, arrayLength);
}

template <>
inline void ISettings::setArray(const char* path, const float* array, size_t arrayLength)
{
    setFloatArray(path, array, arrayLength);
}

template <>
inline void ISettings::setArray(const char* path, const double* array, size_t arrayLength)
{
    setFloat64Array(path, array, arrayLength);
}

template <>
inline void ISettings::setArray(const char* path, const char* const* array, size_t arrayLength)
{
    setStringArray(path, array, arrayLength);
}

template <>
inline void ISettings::setDefault(const char* path, bool value)
{
    setDefaultBool(path, value);
}

template <>
inline void ISettings::setDefault(const char* path, int32_t value)
{
    setDefaultInt(path, value);
}

template <>
inline void ISettings::setDefault(const char* path, int64_t value)
{
    setDefaultInt64(path, value);
}

template <>
inline void ISettings::setDefault(const char* path, float value)
{
    setDefaultFloat(path, value);
}

template <>
inline void ISettings::setDefault(const char* path, double value)
{
    setDefaultFloat64(path, value);
}

template <>
inline void ISettings::setDefault(const char* path, const char* value)
{
    setDefaultString(path, value);
}

template <>
inline void ISettings::setDefaultArray(const char* settingsPath, const bool* array, size_t arrayLength)
{
    setDefaultBoolArray(settingsPath, array, arrayLength);
}

template <>
inline void ISettings::setDefaultArray(const char* settingsPath, const int32_t* array, size_t arrayLength)
{
    setDefaultIntArray(settingsPath, array, arrayLength);
}

template <>
inline void ISettings::setDefaultArray(const char* settingsPath, const int64_t* array, size_t arrayLength)
{
    setDefaultInt64Array(settingsPath, array, arrayLength);
}

template <>
inline void ISettings::setDefaultArray(const char* settingsPath, const float* array, size_t arrayLength)
{
    setDefaultFloatArray(settingsPath, array, arrayLength);
}

template <>
inline void ISettings::setDefaultArray(const char* settingsPath, const double* array, size_t arrayLength)
{
    setDefaultFloat64Array(settingsPath, array, arrayLength);
}

template <>
inline void ISettings::setDefaultArray(const char* settingsPath, const char* const* array, size_t arrayLength)
{
    setDefaultStringArray(settingsPath, array, arrayLength);
}
#endif

} // namespace settings
} // namespace carb
