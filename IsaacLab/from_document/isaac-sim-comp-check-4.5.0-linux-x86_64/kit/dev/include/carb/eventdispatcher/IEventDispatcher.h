// Copyright (c) 2022-2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//!
//! @brief Interface definition for IEventDispatcher
#pragma once

#include "../Interface.h"
#include "EventDispatcherTypes.h"

//! The version number of the latest @ref carb::eventdispatcher::IEventDispatcher interface.
#define carb_eventdispatcher_IEventDispatcher_latest CARB_HEXVERSION(1, 1)
#ifndef carb_eventdispatcher_IEventDispatcher
//! The default version number of the @ref carb::eventdispatcher::IEventDispatcher interface
//! to use for this build.  This macro may be overridden in the project settings to instead
//! use the latest version of the interface.
#    define carb_eventdispatcher_IEventDispatcher CARB_HEXVERSION(0, 1)
#endif

namespace carb
{

//! Namespace for *carb.eventdispatcher.plugin* and related utilities.
namespace eventdispatcher
{

class ObserverGuard;

//! Interface for *carb.eventdispatcher.plugin*.
struct IEventDispatcher
{
    // 0.1 - Initial version
    // 1.0 - (no changes)
    // 1.1 - Added ability to change and query order
    CARB_PLUGIN_INTERFACE_EX("carb::eventdispatcher::IEventDispatcher",
                             carb_eventdispatcher_IEventDispatcher_latest,
                             carb_eventdispatcher_IEventDispatcher)

    //! @private (see observeEvent())
    Observer(CARB_ABI* internalObserveEvent)(int order,
                                             RString eventName,
                                             size_t numVariants,
                                             NamedVariant const* variants,
                                             ObserverFn fn,
                                             CleanupFn cleanup,
                                             void* ud);

    /**
     * Stops the given observer. Safe to perform while dispatching.
     *
     * Since observers can be in use by this thread or any thread, this function is carefully synchronized with all
     * other IEventDispatcher operations.
     *  - During stopObserving(), further calls to the observer are prevented, even if other threads are currently
     *    dispatching an event that would be observed by the observer in question.
     *  - If any other thread is currently calling the observer in question, stopObserving() will wait until all other
     *    threads have left the observer function.
     *  - If the observer function is \b not in the callstack of the current thread, the cleanup function provided to
     *    \c internalObserveEvent() is called and any \ref variant::Variant objects captured to filter events are
     *    destroyed.
     *  - If the observer function is in the callstack of the current thread, stopObserving() will return without
     *    waiting, calling the cleanup function or destroying \ref variant::Variant objects. Instead, this cleanup will
     *    be performed when the \ref dispatchEvent() call in the current thread finishes.
     *
     * When stopObserving() returns, it is guaranteed that the observer function will no longer be called and all calls
     * to it have completed (except if the calling thread is dispatching).
     *
     * @warning This function must be called exactly once per \ref Observer created by \ref observeEvent(). The
     * \ref ObserverGuard calls this function automatically.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     *
     * @param ob The \ref Observer to stop.
     * @returns \c true if the Observer was found and stopped; \c false otherwise.
     */
    bool(CARB_ABI* stopObserving)(Observer ob);

    //! @private (see hasObservers())
    bool(CARB_ABI* internalHasObservers)(RString eventName, size_t numVariants, NamedVariant const* variants);

    //! @private (see dispatchEvent())
    size_t(CARB_ABI* internalDispatch)(const EventData&);

    /**
     * Queries to see if the system is currently dispatching an event.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     *
     * @param currentThread If \c false, checks to see if any thread is currently dispatching. However, the return value
     * should be used for debugging purposes only as it is a transient value and could be stale by the time it is read
     * by the application. If \c true, checks to see if the current thread is dispatching (that is, the callstack
     * includes a call to \ref dispatchEvent()).
     * @returns \c true if any thread or the current thread is dispatching based on the value of \p currentThread;
     * \c false otherwise.
     */
    bool(CARB_ABI* isDispatching)(bool currentThread);

#if CARB_VERSION_ATLEAST(carb_eventdispatcher_IEventDispatcher, 1, 1)
    //! @private (see observeEvent)
    Observer(CARB_ABI* internalObserveEvent2)(const ObserveEventDesc& desc);

    /**
     * Retrieves the name of an observer.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @param ob The observer to query, typically accessed from \ref ObserverGuard::get.
     * @param nameOut Receives the name of the observer, originally passed to \ref observeEvent or similar.
     * @returns `true` if the observer was valid and the name was retrieved; `false` otherwise.
     */
    bool(CARB_ABI* getObserverName)(Observer ob, RStringKey& nameOut);

    /**
     * Gets the 'order' value of an observer.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @param ob The observer to query, typically accessed from \ref ObserverGuard::get.
     * @param orderOut Receives the order value of the observer.
     * @returns `true` if the observer was valid and the order was retrieved; `false` otherwise.
     */
    bool(CARB_ABI* getObserverOrder)(Observer ob, int& orderOut);

    /**
     * Changes the 'order' value of an observer.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @param ob The observer to change, typically accessed from \ref ObserverGuard::get.
     * @param newOrder The new order value to set.
     * @returns `true` if the observer was valid and the order was set; `false` otherwise.
     */
    bool(CARB_ABI* setObserverOrder)(Observer ob, int newOrder);

    /**
     * Gets the 'enabled' state of an observer.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @param ob The observer to query, typically accessed from \ref ObserverGuard::get.
     * @param enabledOut Receives the enabled state of the observer.
     * @returns `true` if the observer was valid and the enabled state was retrieved; `false` otherwise.
     */
    bool(CARB_ABI* getObserverEnabled)(Observer ob, bool& enabledOut);

    /**
     * Changes the 'enabled' state of an observer.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @param ob The observer to query, typically accessed from \ref ObserverGuard::get.
     * @param enabled The new enabled state to set.
     * @returns `true` if the observer was valid and the enabled state was set; `false` otherwise.
     */
    bool(CARB_ABI* setObserverEnabled)(Observer ob, bool enabled);
#endif

    // Interface functions above
    ///////////////////////////////////////////////////////////////////////////
    // Inline helper functions below

#if CARB_VERSION_ATLEAST(carb_eventdispatcher_IEventDispatcher, 1, 1)
    /**
     * Registers an observer with the Event Dispatcher system.
     *
     * An observer is an invocable object (function, functor, lambda, etc.) that is called whenever \ref dispatchEvent()
     * is called. The observers are invoked in the thread that calls \ref dispatchEvent(), and multiple threads could be
     * calling \ref dispatchEvent() simultaneously, so observers must be thread-safe unless the application can ensure
     * synchronization around \ref dispatchEvent() calls.
     *
     * Observers can pass zero or any number of @p filterArgs parameters. These @p filterArgs cause an observer to only
     * be invoked for a \ref dispatchEvent() call that contains at least the same values. For instance, having a filter
     * pair for key "WindowID" with a specific value will only cause the observer to be called if \ref dispatchEvent()
     * is given the same value as a "WindowID" parameter.
     *
     * Observers can be added inside of an observer notification (i.e. during a call to \ref dispatchEvent()), however
     * these new observers will <b>not be called</b> for the currently dispatching event. A subsequent recursive call to
     * \ref dispatchEvent() <em>(on the current thread only)</em> will also call the new observer. The new observer will
     * be available to all other threads once the \ref dispatchEvent() call--in which it was added--returns.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     *
     * @param observerName A name for this observer, used in profiling and debugging.
     * @param order A value determining call order. Observers with lower order values are called earlier. Observers with
     * the same order value and same filter argument values will be called in the order they are registered. Observers
     * with the same order value with \a different filter argument values are called in an indeterminate order.
     * @param eventName The name of the event to observe.
     * @param invocable An object that is invoked when an event matching the \p eventName and \p filterArgs is
     * dispatched. The object must be callable as `void(const Event&)`.
     * @param filterArgs Zero or more arguments that filter observer invocations. Each argument must be of type
     * `std::pair<RStringKey, T>` where the first parameter is the key and the second is the value. The value must be
     * of a type understood by a \ref variant::Translator specialization.
     * @returns An \ref ObserverGuard representing the lifetime of the observer. When the \ref ObserverGuard is reset or
     * destroyed, the observer is unregistered as with \ref stopObserving().
     */
    template <class Invocable, class... Args>
    CARB_NODISCARD ObserverGuard
    observeEvent(RStringKey observerName, int order, RString eventName, Invocable&& invocable, Args&&... filterArgs);

    /**
     * Registers an observer with the Event Dispatcher system.
     *
     * An observer is an invocable object (function, functor, lambda, etc.) that is called whenever \ref dispatchEvent()
     * is called. The observers are invoked in the thread that calls \ref dispatchEvent(), and multiple threads could be
     * calling \ref dispatchEvent() simultaneously, so observers must be thread-safe unless the application can ensure
     * synchronization around \ref dispatchEvent() calls.
     *
     * Observers can pass zero or any number of @p filterArgs parameters. These @p filterArgs cause an observer to only
     * be invoked for a \ref dispatchEvent() call that contains at least the same values. For instance, having a filter
     * pair for key "WindowID" with a specific value will only cause the observer to be called if \ref dispatchEvent()
     * is given the same value as a "WindowID" parameter.
     *
     * Observers can be added inside of an observer notification (i.e. during a call to \ref dispatchEvent()), however
     * these new observers will <b>not be called</b> for the currently dispatching event. However, a recursive call to
     * \ref dispatchEvent() <em>(on the current thread only)</em> will also call the new observer. The new observer will
     * be available to all other threads once the \ref dispatchEvent() call--in which it was added--returns.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     *
     * @param observerName A name for this observer, used in profiling and debugging.
     * @param order A value determining call order. Observers with lower order values are called earlier. Observers with
     * the same order value and same filter argument values will be called in the order they are registered. Observers
     * with the same order value with \a different filter argument values are called in an indeterminate order.
     * @param eventName The name of the event to observe.
     * @param invocable An object that is invoked when an event matching the \p eventName and \p filterArgs is
     * dispatched. The object must be callable as `void(const Event&)`.
     * @tparam InIter An InputIterator that is forward-iterable and resolves to a \ref NamedVariant when dereferenced.
     * @param begin An InputIterator representing the start of the filter parameters.
     * @param end A past-the-end InputIterator representing the end of the filter parameters.
     * @returns An \ref ObserverGuard representing the lifetime of the observer. When the \ref ObserverGuard is reset or
     * destroyed, the observer is unregistered as with \ref stopObserving().
     */
    template <class Invocable, class InIter>
    CARB_NODISCARD ObserverGuard observeEventIter(
        RStringKey observerName, int order, RString eventName, Invocable&& invocable, InIter begin, InIter end);

    /**
     * Registers an observer with the Event Dispatcher system.
     *
     * An observer is an invocable object (function, functor, lambda, etc.) that is called whenever \ref dispatchEvent()
     * is called. The observers are invoked in the thread that calls \ref dispatchEvent(), and multiple threads could be
     * calling \ref dispatchEvent() simultaneously, so observers must be thread-safe unless the application can ensure
     * synchronization around \ref dispatchEvent() calls.
     *
     * Observers can pass zero or any number of @p filterArgs parameters. These @p filterArgs cause an observer to only
     * be invoked for a \ref dispatchEvent() call that contains at least the same values. For instance, having a filter
     * pair for key "WindowID" with a specific value will only cause the observer to be called if \ref dispatchEvent()
     * is given the same value as a "WindowID" parameter.
     *
     * Observers can be added inside of an observer notification (i.e. during a call to \ref dispatchEvent()), however
     * these new observers will <b>not be called</b> for the currently dispatching event. However, a recursive call to
     * \ref dispatchEvent() <em>(on the current thread only)</em> will also call the new observer. The new observer will
     * be available to all other threads once the \ref dispatchEvent() call--in which it was added--returns.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     *
     * @param observerName A name for this observer, used in profiling and debugging.
     * @param order A value determining call order. Observers with lower order values are called earlier. Observers with
     * the same order value and same filter argument values will be called in the order they are registered. Observers
     * with the same order value with \a different filter argument values are called in an indeterminate order.
     * @param eventName The name of the event to observe.
     * @param invocable An object that is invoked when an event matching the \p eventName and \p filterArgs is
     * dispatched. The object must be callable as `void(const Event&)`.
     * @tparam R A range type. Since this implementation is for pre-C++20 and C++20 ranges are not available, this is an
     *  approximation: this type must work with `std::begin()` and `std::end()` to produce an iterator type that
     *  conforms to *InputIterator*. The iterator must resolve to a `std::pair<RStringKey, T>` when dereferenced, where
     *  the first parameter is the key and the second parameter is the value and must be a type understood by a
     *  \ref variant::Translator specialization. Each key must be unique, otherwise it is not defined which non-unique
     *  value is obtained when the key is queried.
     * @param range The range to use as message parameters. See explanation of the `R` type above.
     * @returns An \ref ObserverGuard representing the lifetime of the observer. When the \ref ObserverGuard is reset or
     * destroyed, the observer is unregistered as with \ref stopObserving().
     */
    template <class R,
              class Invocable CARB_NO_DOC(
                  ,
                  std::enable_if_t<
                      std::is_same<void, cpp::void_t<decltype(std::begin(std::declval<R>()), std::end(std::declval<R>()))>>::value,
                      bool> = false)>
    CARB_NODISCARD ObserverGuard
    observeEventRange(RStringKey observerName, int order, RString eventName, Invocable&& invocable, R&& range)
    {
        return observeEventIter(
            observerName, order, eventName, std::forward<Invocable>(invocable), range.begin(), range.end());
    }
#    ifndef DOXYGEN_BUILD
    // Enable implicit initializer_list since {} cannot be deduced to a type
    template <class Invocable, class T>
    CARB_NODISCARD ObserverGuard observeEventRange(RStringKey observerName,
                                                   int order,
                                                   RString eventName,
                                                   Invocable&& invocable,
                                                   std::initializer_list<std::pair<RStringKey, T>> range)
    {
        return observeEventIter(
            observerName, order, eventName, std::forward<Invocable>(invocable), range.begin(), range.end());
    }
#    endif
#else
    template <class Invocable, class... Args>
    CARB_NODISCARD ObserverGuard observeEvent(int order, RString eventName, Invocable&& invocable, Args&&... filterArgs);

    template <class Invocable, class InIter>
    CARB_NODISCARD ObserverGuard
    observeEventIter(int order, RString eventName, Invocable&& invocable, InIter begin, InIter end);

    template <class R,
              class Invocable CARB_NO_DOC(
                  ,
                  std::enable_if_t<
                      std::is_same<void, cpp::void_t<decltype(std::begin(std::declval<R>()), std::end(std::declval<R>()))>>::value,
                      bool> = false)>
    CARB_NODISCARD ObserverGuard observeEventRange(int order, RString eventName, Invocable&& invocable, R&& range)
    {
        return observeEventIter(order, eventName, std::forward<Invocable>(invocable), range.begin(), range.end());
    }

    template <class Invocable, class T>
    CARB_NODISCARD ObserverGuard observeEventRange(int order,
                                                   RString eventName,
                                                   Invocable&& invocable,
                                                   std::initializer_list<std::pair<RStringKey, T>> range)
    {
        return observeEventIter(order, eventName, std::forward<Invocable>(invocable), range.begin(), range.end());
    }
#endif

    /**
     * Queries the Event Dispatcher whether any observers are listening to a specific event signature.
     *
     * Emulates a call to \ref dispatchEvent() (without actually calling any observers) and returns \c true if any
     * observers would be called.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @param eventName The name of the event to query.
     * @param filterArgs Zero or more key/value pairs that would be used for observer filtering as in a call to
     * \ref dispatchEvent(). Each argument must be of type `std::pair<RStringKey, T>` where the first parameter is the
     * key and the second is the value. The value must be of a type understood by a \ref variant::Translator
     * specialization.
     * @returns \c true if at least one observer would be called if the same arguments were passed to
     * \ref dispatchEvent(); \c false otherwise.
     */
    template <class... Args>
    bool hasObservers(RString eventName, Args&&... filterArgs);

    /**
     * Queries the Event Dispatcher whether any observers are listening to a specific event signature.
     *
     * Emulates a call to \ref dispatchEvent() (without actually calling any observers) and returns \c true if any
     * observers would be called.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @tparam InIter An InputIterator that is forward-iterable and resolves to a \ref NamedVariant when dereferenced.
     * The entries are used for observer filtering.
     * @param eventName The name of the event to query.
     * @param begin An InputIterator representing the start of the event key/value pairs.
     * @param end A past-the-end InputIterator representing the end of the event key/value pairs.
     * @returns \c true if at least one observer would be called if the same arguments were passed to
     * \ref dispatchEvent(); \c false otherwise.
     */
    template <class InIter>
    bool hasObserversIter(RString eventName, InIter begin, InIter end);

    /**
     * Queries the Event Dispatcher whether any observers are listening to a specific event signature.
     *
     * Emulates a call to \ref dispatchEvent() (without actually calling any observers) and returns \c true if any
     * observers would be called.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @tparam R A range type. Since this implementation is for pre-C++20 and C++20 ranges are not available, this is an
     *  approximation: this type must work with `std::begin()` and `std::end()` to produce an iterator type that
     *  conforms to *InputIterator*. The iterator must resolve to a `std::pair<RStringKey, T>` when dereferenced, where
     *  the first parameter is the key and the second parameter is the value and must be a type understood by a
     *  \ref variant::Translator specialization. Each key must be unique, otherwise it is not defined which non-unique
     *  value is obtained when the key is queried.
     * @param eventName The name of the event to query.
     * @param range The range to use as observer parameters. See explanation of the `R` type above.
     * @returns \c true if at least one observer would be called if the same arguments were passed to
     * \ref dispatchEvent(); \c false otherwise.
     */
    template <class R CARB_NO_DOC(
        ,
        std::enable_if_t<std::is_same<void, cpp::void_t<decltype(std::begin(std::declval<R>()), std::end(std::declval<R>()))>>::value,
                         bool> = false)>
    bool hasObserversRange(RString eventName, R&& range)
    {
        // Implementation needs to be inline because certain compilers get very confused about the SFINAE
        std::vector<NamedVariant> variants;
        for (auto& r : range)
            variants.emplace_back(detail::translate(r));
        std::sort(variants.begin(), variants.end(), detail::NamedVariantLess{});
        CARB_ASSERT(std::adjacent_find(variants.begin(), variants.end(), detail::NamedVariantEqual{}) == variants.end(),
                    "At least one non-unique key specified");
        return internalHasObservers(eventName, variants.size(), variants.data());
    }
#ifndef DOXYGEN_BUILD
    // Enable implicit initializer_list since {} cannot be deduced to a type
    template <class T>
    bool hasObserversRange(RString eventName, std::initializer_list<std::pair<RStringKey, T>> range)
    {
        return hasObserversRange<std::initializer_list<std::pair<RStringKey, T>>>(eventName, std::move(range));
    }
#endif

    /**
     * Re-dispatch an Event potentially as a different eventName.
     *
     * This can also be combined with a \ref IMessageQueue to dispatch queued events.
     * @warning While it is safe to recursively dispatch events, this call can lead to endless recursion if the event
     *   name is unchanged.
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @param event The event to re-dispatch.
     * @param newEventName The event name to override \ref Event::eventName from \p event. Only used if not `nullptr`.
     * @returns The count of observers that were called. Recursive dispatch calls are not included.
     */
    size_t dispatchEvent(const Event& event, RString* newEventName = nullptr);

    /**
     * Dispatches an event and immediately calls all observers that would observe this particular event.
     *
     * Finds and calls all observers (in the current thread) that observe the given event signature.
     *
     * It is safe to recursively dispatch events (i.e. call dispatchEvent() from a called observer), but care must be
     * taken to avoid endless recursion. See the rules in observeEvent() for observers added during a dispatchEvent()
     * call.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @param eventName The name of the event to dispatch.
     * @param payload Zero or more key/value pairs that are used as the event payload and may be queried by observers or
     * used to filter observers. Each argument must be of type `std::pair<RStringKey, T>` where the first parameter is
     * the key and the second is the value. The value must be of a type understood by a \ref variant::Translator
     * specialization.
     * @returns The count of observers that were called. Recursive dispatch calls are not included.
     */
    template <class... Args>
    size_t dispatchEvent(RString eventName, Args&&... payload);

    /**
     * Dispatches an event and immediately calls all observers that would observe this particular event.
     *
     * Finds and calls all observers (in the current thread) that observe the given event signature.
     *
     * It is safe to recursively dispatch events (i.e. call dispatchEvent() from a called observer), but care must be
     * taken to avoid endless recursion. See the rules in observeEvent() for observers added during a dispatchEvent()
     * call.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @tparam InIter An InputIterator that is forward-iterable and resolves to a \ref NamedVariant when dereferenced.
     * The entries are used as the event payload and may be queried by observers or used to filter observers.
     * @param eventName The name of the event to dispatch.
     * @param begin An InputIterator representing the start of the event key/value pairs.
     * @param end A past-the-end InputIterator representing the end of the event key/value pairs.
     * @returns The count of observers that were called. Recursive dispatch calls are not included.
     */
    template <class InIter>
    size_t dispatchEventIter(RString eventName, InIter begin, InIter end);

    /**
     * Dispatches an event and immediately calls all observers that would observe this particular event.
     *
     * Finds and calls all observers (in the current thread) that observe the given event signature.
     *
     * It is safe to recursively dispatch events (i.e. call dispatchEvent() from a called observer), but care must be
     * taken to avoid endless recursion. See the rules in observeEvent() for observers added during a dispatchEvent()
     * call.
     *
     * @thread_safety Safe to perform while any thread is performing any operation against IEventDispatcher.
     * @tparam R A range type. Since this implementation is for pre-C++20 and C++20 ranges are not available, this is an
     *  approximation: this type must work with `std::begin()` and `std::end()` to produce an iterator type that
     *  conforms to *InputIterator*. The iterator must resolve to a `std::pair<RStringKey, T>` when dereferenced, where
     *  the first parameter is the key and the second parameter is the value and must be a type understood by a
     *  \ref variant::Translator specialization. Each key must be unique, otherwise it is not defined which non-unique
     *  value is obtained when the key is queried.
     * @param eventName The name of the event to dispatch.
     * @param range The range to use for key/value pairs. See explanation of the `R` type above.
     * @returns The count of observers that were called. Recursive dispatch calls are not included.
     */
    template <class R>
    size_t dispatchEventRange(RString eventName, R&& range)
    {
        // Implementation needs to be inline because certain compilers get very confused about the SFINAE
        std::vector<NamedVariant> variants;
        for (auto& r : range)
            variants.emplace_back(detail::translate(r));
        std::sort(variants.begin(), variants.end(), detail::NamedVariantLess{});
        CARB_ASSERT(std::adjacent_find(variants.begin(), variants.end(), detail::NamedVariantEqual{}) == variants.end(),
                    "Event has duplicate keys");
        return internalDispatch({ eventName, variants.size(), variants.data() });
    }
#ifndef DOXYGEN_BUILD
    // Enable implicit initializer_list since {} cannot be deduced to a type
    template <class T>
    size_t dispatchEventRange(RString eventName, std::initializer_list<std::pair<RStringKey, T>> range)
    {
        return dispatchEventRange<std::initializer_list<std::pair<RStringKey, T>>>(eventName, std::move(range));
    }
#endif
};

} // namespace eventdispatcher
} // namespace carb

#include "IEventDispatcher.inl"
