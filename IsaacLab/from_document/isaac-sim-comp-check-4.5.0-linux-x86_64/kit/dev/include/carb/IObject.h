// Copyright (c) 2020-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//!
//! @brief Implementation of Carbonite objects.
#pragma once

#include "Interface.h"

#include <utility>

namespace carb
{

/**
 * Reference-counted object base.
 */
class IObject
{
public:
    CARB_PLUGIN_INTERFACE("carb::IObject", 1, 0)

    /**
     * Destructor.
     */
    virtual ~IObject() = default;

    /**
     * Atomically add one to the reference count.
     * @returns The current reference count after one was added, though this value may change before read if other
     * threads are also modifying the reference count. The return value is guaranteed to be non-zero.
     */
    virtual size_t addRef() = 0;

    /**
     * Atomically subtracts one from the reference count. If the result is zero, carb::deleteHandler() is called for
     * `this`.
     * @returns The current reference count after one was subtracted. If zero is returned, carb::deleteHandler() was
     * called for `this`.
     */
    virtual size_t release() = 0;
};


/**
 * Smart pointer type for ref counting `IObject`. It automatically controls reference count for the underlying
 * `IObject` pointer.
 */
template <class T>
class ObjectPtr
{
public:
    //////////// Ctors/dtor ////////////

    /**
     * Policy directing how the smart pointer is initialized from from raw pointer.
     */
    enum class InitPolicy
    {
        eBorrow, ///< Increases reference count.
        eSteal ///< Assign the pointer without increasing the reference count.
    };

    /**
     * Default Constructor
     */
    ObjectPtr() : m_object(nullptr)
    {
    }

    /**
     * Nullptr Constructor
     */
    ObjectPtr(std::nullptr_t) : m_object(nullptr)
    {
    }

    /**
     * Constructor
     * @param object The raw pointer to an object. If not `nullptr`, it will be "borrowed"; that is, the reference count
     * will be increased as long as `*this` contains it.
     */
    explicit ObjectPtr(T* object) : m_object(object)
    {
        if (m_object)
        {
            m_object->addRef();
        }
    }


    /**
     * Constructor.
     * @param object The raw pointer to an object.
     * @param policy Directive on whether the reference count should be increased or not.
     */
    ObjectPtr(T* object, InitPolicy policy) : m_object(object)
    {
        if (policy == InitPolicy::eBorrow && m_object != nullptr)
        {
            m_object->addRef();
        }
    }

    /**
     * Copy constructor. Always increases the reference count.
     * @param other The smart pointer from which to copy a reference.
     */
    ObjectPtr(const ObjectPtr<T>& other) : ObjectPtr(other.m_object, InitPolicy::eBorrow)
    {
    }

    /// @copydoc ObjectPtr(const ObjectPtr<T>& other)
    template <class U>
    ObjectPtr(const ObjectPtr<U>& other) : ObjectPtr(other.m_object, InitPolicy::eBorrow)
    {
    }

    /**
     * Move constructor. Steals the reference count from @p other and leaves it empty.
     * @param other The smart pointer from which to steal a reference.
     */
    ObjectPtr(ObjectPtr<T>&& other) : m_object(other.m_object)
    {
        other.m_object = nullptr;
    }

    /// @copydoc ObjectPtr(ObjectPtr<T>&& other)
    template <class U>
    ObjectPtr(ObjectPtr<U>&& other) : m_object(other.m_object)
    {
        other.m_object = nullptr;
    }

    /**
     * Destructor.
     */
    ~ObjectPtr()
    {
        _release();
    }

    //////////// Helpers ////////////


    //////////// Ptr ////////////

    /**
     * Converts the smart pointer to a raw pointer.
     * @returns The raw pointer referenced by the smart pointer. May be `nullptr`.
     */
    T* get() const
    {
        return m_object;
    }

    /**
     * Pointer dereference operator.
     * @returns The raw pointer referenced by the smart pointer.
     */
    T* operator->() const
    {
        CARB_ASSERT(m_object);
        return m_object;
    }

    /**
     * Dereference operator.
     * @returns A reference to the pointed-at object.
     */
    T& operator*() const
    {
        CARB_ASSERT(m_object);
        return *m_object;
    }

    /**
     * Boolean conversion operator.
     * @returns `true` if the smart pointer is not empty; `false` if the smart pointer is empty.
     */
    explicit operator bool() const
    {
        return get() != nullptr;
    }


    //////////// Explicit access ////////////

    /**
     * Returns the address of the internal reference.
     * @returns The address of the internal reference.
     */
    T* const* getAddressOf() const
    {
        return &m_object;
    }

    /// @copydoc getAddressOf() const
    T** getAddressOf()
    {
        return &m_object;
    }

    /**
     * Helper function to release any current reference and return the address of the internal reference pointer.
     * @returns The address of the internal reference.
     */
    T** releaseAndGetAddressOf()
    {
        _release();
        return &m_object;
    }

    /**
     * Resets this smart pointer to `nullptr` and returns the previously reference object @a without releasing the held
     * reference.
     * @returns The previously referenced object.
     */
    T* detach()
    {
        T* temp = m_object;
        m_object = nullptr;
        return temp;
    }

    /**
     * Releases the reference on any held object and instead @a steals the given object.
     * @param other The object to steal a reference to.
     */
    void attach(T* other)
    {
        _release();
        m_object = other;
    }


    //////////// Assignment operator ////////////

    /**
     * Assignment to @a nullptr. Releases any previously held reference.
     * @returns @a *this
     */
    ObjectPtr& operator=(decltype(nullptr))
    {
        _release();
        return *this;
    }

    /**
     * Releases any previously held reference and copies a reference to @p other.
     * @param other The object to reference.
     * @returns @a *this
     */
    ObjectPtr& operator=(T* other)
    {
        ObjectPtr(other).swap(*this);
        return *this;
    }

    /// @copydoc operator=
    template <typename U>
    ObjectPtr& operator=(U* other)
    {
        ObjectPtr(other).swap(*this);
        return *this;
    }

    /// @copydoc operator=
    ObjectPtr& operator=(const ObjectPtr& other)
    {
        ObjectPtr(other).swap(*this);
        return *this;
    }

    /// @copydoc operator=
    template <class U>
    ObjectPtr& operator=(const ObjectPtr<U>& other)
    {
        ObjectPtr(other).swap(*this);
        return *this;
    }

    /**
     * Releases any previously held reference and steals the reference from @p other.
     * @param other The reference to steal. Will be swapped with @a *this.
     * @returns @a *this
     */
    ObjectPtr& operator=(ObjectPtr&& other)
    {
        other.swap(*this);
        return *this;
    }

    /// @copydoc operator=(ObjectPtr&& other)
    template <class U>
    ObjectPtr& operator=(ObjectPtr<U>&& other)
    {
        ObjectPtr(std::move(other)).swap(*this);
        return *this;
    }

    /**
     * Swaps with another smart pointer.
     * @param other The smart pointer to swap with.
     */
    void swap(ObjectPtr& other)
    {
        std::swap(m_object, other.m_object);
    }

private:
    void _release()
    {
        if (T* old = std::exchange(m_object, nullptr))
        {
            old->release();
        }
    }

    T* m_object;
};

/**
 * Checks equality of two \ref ObjectPtr types.
 *
 * @param[in] lhs An ObjectPtr type to compare.
 * @param[in] rhs An ObjectPtr type to compare.
 * @returns `true` if the two objects identify the same underlying object.  Returns
 *          `false` otherwise.
 */
template <class T, class U>
bool operator==(const ObjectPtr<T>& lhs, const ObjectPtr<U>& rhs) noexcept
{
    return lhs.get() == rhs.get();
}

/**
 * Checks equality of an \ref ObjectPtr type and a raw pointer.
 *
 * @param[in] lhs An ObjectPtr type to compare.
 * @param[in] rhs A raw pointer to compare.
 * @returns `true` if the two objects identify the same underlying object.  Returns
 *          `false` otherwise.
 */
template <class T, class U>
bool operator==(const ObjectPtr<T>& lhs, const U* rhs) noexcept
{
    return lhs.get() == rhs;
}

/**
 * Checks equality of an \ref ObjectPtr type and a raw pointer.
 *
 * @param[in] lhs An ObjectPtr type to compare.
 * @param[in] rhs A raw pointer to compare.
 * @returns `true` if the two objects identify the same underlying object.  Returns
 *          `false` otherwise.
 */
template <class T, class U>
bool operator==(const T* lhs, const ObjectPtr<U>& rhs) noexcept
{
    return lhs == rhs.get();
}

/**
 * Checks inequality of two \ref ObjectPtr types.
 *
 * @param[in] lhs An ObjectPtr type to compare.
 * @param[in] rhs An ObjectPtr type to compare.
 * @returns `true` if the two objects identify different underlying objects.  Returns
 *          `false` otherwise.
 */
template <class T, class U>
bool operator!=(const ObjectPtr<T>& lhs, const ObjectPtr<U>& rhs) noexcept
{
    return lhs.get() != rhs.get();
}

/**
 * Checks inequality of an \ref ObjectPtr type and a raw pointer.
 *
 * @param[in] lhs An ObjectPtr type to compare.
 * @param[in] rhs A raw pointer to compare.
 * @returns `true` if the two objects identify different underlying objects.  Returns
 *          `false` otherwise.
 */
template <class T, class U>
bool operator!=(const ObjectPtr<T>& lhs, const U* rhs) noexcept
{
    return lhs.get() != rhs;
}

/**
 * Checks inequality of an \ref ObjectPtr type and a raw pointer.
 *
 * @param[in] lhs An ObjectPtr type to compare.
 * @param[in] rhs A raw pointer to compare.
 * @returns `true` if the two objects identify different underlying objects.  Returns
 *          `false` otherwise.
 */
template <class T, class U>
bool operator!=(const T* lhs, const ObjectPtr<U>& rhs) noexcept
{
    return lhs != rhs.get();
}

/**
 * Helper function to create carb::ObjectPtr from a carb::IObject pointer by "stealing" the pointer; that is, without
 * increasing the reference count.
 * @param other The raw pointer to steal.
 * @returns A smart pointer referencing @p other.
 */
template <class T>
inline ObjectPtr<T> stealObject(T* other)
{
    return ObjectPtr<T>(other, ObjectPtr<T>::InitPolicy::eSteal);
}


/**
 * Helper function to create carb::ObjectPtr from a carb::IObject pointer by "borrowing" the pointer; that is, by
 * increasing the reference count.
 * @param other The raw pointer to reference.
 * @returns A smart pointer referencing @p other.
 */
template <class T>
inline ObjectPtr<T> borrowObject(T* other)
{
    return ObjectPtr<T>(other, ObjectPtr<T>::InitPolicy::eBorrow);
}

} // namespace carb
