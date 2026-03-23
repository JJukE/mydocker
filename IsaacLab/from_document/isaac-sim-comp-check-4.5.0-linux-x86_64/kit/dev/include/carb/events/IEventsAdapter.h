// Copyright (c) 2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief carb.events adapter to carb.eventdispatcher
#pragma once

#include "../Interface.h"
#include "../IObject.h"
#include "../RString.h"
#include "../cpp/Span.h"
#include "IEvents.h"

#include "../../omni/Expected.h"
#include "../../omni/core/Result.h"

namespace carb
{
namespace eventdispatcher
{
class IMessageQueue;
}

namespace events
{

//! Adapter type.
enum class AdapterType
{
    eDispatch, //!< Simple dispatch-only adapter type. See \ref IEventsAdapter for full explanation.
    ePushPump, //!< Simple push-pump adapter type. See \ref IEventsAdapter for full explanation.
    eFull, //!< Full adapter type. See \ref IEventsAdapter for full explanation.

    // Must be last
    eCount //!< Number of adapter types.
};

//! Structure defining a mapping entry for use with \ref AdapterDesc.
struct MappingEntry
{
    //! An event type. Must be unique across all mappings provided to an \ref AdapterDesc. `0` is a valid \ref EventType
    //! and often used for event streams that only push one event type.
    EventType type;

    //! Name of `type` for the pop/dispatch side. Required and must be unique across all mappings. Must be unique with
    //! respect to \ref pushName if \ref pushName is provided.
    RString dispatchName;

    //! Optional name of `type` for the push side. If empty, no event is dispatched for an event stream's `push()` call.
    //! An empty `RString()` is used provide an unspecified push name. Ignored except for \ref AdapterType::eFull.
    RString pushName;

    //! Constructor
    //! @param type The \ref EventType to map.
    //! @param dispatchName The \ref RString to map \p type to.
    //! @param pushName For \ref AdapterType::eFull adapters, indicates the \ref RString to map \p type to on the push
    //!  side.
    constexpr MappingEntry(EventType type, RString dispatchName, RString pushName = RString()) noexcept
        : type(type), dispatchName(dispatchName), pushName(pushName)
    {
    }
};

//! Describes the adapter type desired by \ref IEventsAdapter::createAdapter().
struct AdapterDesc
{
    //! Used as a version number to ensure the struct is compatible with the interface.
    uint32_t sizeOf{ uint32_t(sizeof(AdapterDesc)) };

    //! The type of the adapter requested.
    AdapterType type;

    //! The name of the adapter. This is used for debugging and logging purposes.
    const char* name;

    //! The mappings of EventType to/from dispatch-name and push-name.
    //!
    //! The mappings here are used two-way, so the EventType is used to look up the dispatch-name or the push-name, and
    //! vice versa. All \ref EventType, \ref MappingEntry::dispatchName and \ref MappingEntry::pushName should be
    //! unique across `mappings`. If they are not, the adapter creation will fail.
    //!
    //! \ref MappingEntry::pushName is not required and is used only for \ref AdapterType::eFull.
    cpp::span<MappingEntry> mappings;

    //! The message queue used for the adapter.
    //!
    //! Only used for \ref AdapterType::eFull. Ignored for \ref AdapterType::ePushPump and \ref AdapterType::eDispatch.
    eventdispatcher::IMessageQueue* messageQueue;

    //! Typical Constructor
    //! @param type The \ref AdapterType.
    //! @param name The name of the adapter.
    //! @param mappings A span of \ref MappingEntry objects for the adapter.
    //! @param messageQueue The \ref eventdispatcher::IMessageQueue instance, if required.
    constexpr AdapterDesc(AdapterType type,
                          const char* name,
                          cpp::span<MappingEntry> mappings,
                          eventdispatcher::IMessageQueue* messageQueue = nullptr) noexcept
        : type(type), name(name), mappings(mappings), messageQueue(messageQueue)
    {
    }
};

/**
 * Interface for creating IEventStream objects that adapt IEventStream functionality to carb::eventdispatcher systems.
 *
 * The IEventsAdapter interface can be used similarly to IEvents to create a specialized type of \ref IEventStream that
 * can be used to map and redirect events through the newer \ref carb::eventdispatcher::IEventDispatcher system.
 *
 * IEventsAdapter is meant to ease the transition from using \ref IEventStream by switching to
 * \ref eventdispatcher::IEventDispatcher. The \ref IEventStream created by IEventsAdapter can be used by existing
 * APIs (Python and C++) and it will translate events to-and-from \ref IEvents.
 *
 * \par Typical Transition Case
 * \parblock
 * Assume that you have current code which uses \ref IEventStream, but the desire is to convert it to
 * \ref eventdispatcher::IEventDispatcher. Of course, this could be done but is likely to break any existing extensions
 * or plugins that use the current \ref IEventStream API.
 *
 * Instead, code can switch to using \ref eventdispatcher::IEventDispatcher but also use IEventsAdapter to create a
 * special \ref IEventStream that observes \ref eventdispatcher::Event distribution, but translates them back to a
 * \ref IEvent for existing code. Conversely, if existing code pushes or dispatches events using \ref IEventStream, the
 * adapter will translate these to \ref eventdispatcher::Event and notify observers.
 *
 * \ref IEventStream is complicated and used in a variety of ways.  The following \ref AdapterType values correspond to
 * the typical \ref IEventStream uses:
 * \endparblock
 *
 * \par AdapterType::eDispatch
 * \parblock
 * \ref AdapterType::eDispatch - The simplest adapter type. Most functionality for \ref IEventStream is disabled. This
 * is for adapting \ref IEventStream instances that only call \ref IEventStream::dispatch; \ref IEventStream::push and
 * \ref IEventStream::pop functionality is disabled and will result in error logging. Push-side subscribers will never
 * be called.
 *
 * Conversion to use this mode simply requires using \ref eventdispatcher::IEventDispatcher::dispatchEvent to dispatch
 * events. The \ref AdapterDesc::mappings should contain entries for all event names that should be converted to IEvent
 * by the adapter.
 * \endparblock
 *
 * \par AdapterType::ePushPump
 * \parblock
 * Another common use case for \ref IEventStream is a stream to which objects are pushed and then immediately pumped
 * from the same thread. From the \ref eventdispatcher::IEventDispatcher perspective, this is likely to convert to a
 * single \ref eventdispatcher::IEventDispatcher::dispatchEvent call. However, this mode correctly supports both push-
 * and pop-side subscribers as they would be correctly called in legacy code.
 *
 * In this mode, when \ref eventdispatcher::IEventDispatcher::dispatchEvent is called, if any push-side subscribers
 * exist on the \ref IEventStream, they are notified prior to pop-side subscribers (and event observers).
 *
 * **WARNING**: If multiple events were pushed before pumping, changing to single dispatchEvent calls will change the
 * push-side notification order from multiple push notifications in a row, to push, pop, push, pop notification instead.
 * If this is problematic, consider using the adapter to push multiple events before pumping or switch to the more
 * complex \ref AdapterType::eFull type.
 *
 * If instead the adapter \ref IEventStream is used to push and pump, \ref eventdispatcher::IEventDispatcher observers
 * are only called during the pop-side phase (i.e. pump) to maintain compatibility.
 * \endparblock
 *
 * \par AdapterType::eFull
 * \parblock
 * The most complex use of legacy \ref IEventStream is as both a message queue and event dispatcher. In this case,
 * \ref IEventStream::push is called for potentially several events, but \ref IEventStream::pump (or
 * \ref IEventStream::pop) is called later and possibly even from a different thread. Converting to
 * \ref eventdispatcher::IEventDispatcher is a bit more complex as a queue must now be involved. This adapter type can
 * be combined with \ref eventdispatcher::IMessageQueue to provide the message queuing.
 *
 * For conversion, the paradigm is typically:
 * -# When an event is queued, call \ref eventdispatcher::IEventDispatcher::dispatchEvent with a pre-delivery
 *    notification event. This should be specified as \ref MappingEntry::pushName during adapter creation and will
 *    function as the push-side \ref IEventStream notification. Typically the name of this event will be similar to the
 *    \ref MappingEntry::dispatchName (such as adding `:push` or `:pre` for instance).
 * -# Immediately after the above event is dispatched, push a message with the corresponding
 *    \ref MappingEntry::dispatchName to the \ref eventdispatcher::IMessageQueue in use (this must also be given to
 *    adapter creation via \ref AdapterDesc::messageQueue).
 * -# At a later point, pop entries from the \ref eventdispatcher::IMessageQueue and dispatch them via
 *    \ref eventdispatcher::IEventDispatcher::dispatchEvent.
 *
 * The adapter will observe the pre-delivery event and call push-side \ref IEventListener subscribers when notified.
 * When the event is dequeued and dispatched, the adapter will call pop-side \ref IEventListener subscribers.
 *
 * Conversely, if \ref IEventStream::push is called, the adapter dispatches the pre-delivery notification and adds the
 * event to the \ref eventdispatcher::IMessageQueue. If the adapter's \ref IEventStream::pump is called (or
 * \ref IEventStream::pop), the event is popped from the \ref eventdispatcher::IMessageQueue and immediately dispatched.
 * \endparblock
 *
 * \par Event Variants -> IEvent Payload Translation
 * \parblock
 * The \ref RStringKey is converted to a string via \ref RStringKey::toString and used as the child item name in the
 * payload.
 * * \ref variant::Variant of `bool` -> \ref dictionary::ItemType::eBool
 * * \ref variant::Variant of integer types -> \ref dictionary::ItemType::eInt
 * * \ref variant::Variant of `float`, `double` -> \ref dictionary::ItemType::eFloat
 * * \ref variant::Variant of `const char*`, `std::string`, `omni::string` -> \ref dictionary::ItemType::eString
 * * \ref variant::Variant of `dictionary::Item*` -> copied into a new \ref dictionary::Item
 * * `Variant` of \ref variant::VariantArray, \ref variant::VariantMap -> recursively copied into a new
 *   \ref dictionary::Item
 * * \ref variant::Variant of `carb::IObject*` -> attached as an object and retrievable via \ref IEvent::retrieveObject
 * * Anything else: result of \ref variant::Variant::toString stored in the payload as a string
 * \endparblock
 *
 * \par IEvent Payload -> Event Variants Translation
 * \parblock
 * If the item name is numeric only, then it is used as the number portion of \ref RStringKey. Otherwise, the string
 * name is converted directly to \ref RStringKey and used as the key.
 *
 * Any objects attached to the \ref IEvent as via \ref IEvent::attachObject are directly converted to
 * \ref variant::Variant of `carb::IObjectPtr` and stored as their attached object name.
 * * \ref dictionary::ItemType::eBool -> \ref variant::Variant of `bool`
 * * \ref dictionary::ItemType::eFloat -> \ref variant::Variant of `double`
 * * \ref dictionary::ItemType::eInt -> \ref variant::Variant of `int64_t`
 * * \ref dictionary::ItemType::eString -> \ref variant::Variant of `omni::string`
 * * \ref dictionary::ItemType::eDictionary -> \ref variant::Variant of \ref dictionary::Item*. Note that this variant
 *   type does not manage the underlying \ref dictionary::Item* or hold a reference to it in any way. If this value is
 *   used beyond the lifetime of the \ref eventdispatcher::Event that delivered it, it should be copied using a method
 *   such as \ref dictionary::IDictionary::update.
 * \endparblock
 */
class IEventsAdapter
{
public:
    // 1.0 - Initial release
    CARB_PLUGIN_INTERFACE("carb::events::IEventsAdapter", 1, 0)

    //! @private
    virtual IEventStream* internalCreateAdapter(const AdapterDesc& desc, omni::core::Result& result) noexcept = 0;

    /**
     * Creates an IEventStream that is used for simple event dispatch streams.
     * @thread_safety This function is thread-safe.
     * @par Errors
     * Accessible via the \ref omni::unexpected return value.
     * * @ref omni::core::kResultOutOfMemory - Memory could not be allocated for the IEventStream.
     * * @ref omni::core::kResultVersionCheckFailure - The \ref AdapterDesc has an invalid `sizeOf` field.
     * * @ref omni::core::kResultInvalidArgument - The \ref AdapterDesc is missing `mappings` or `pushMappings`, or the
     *   uniqueness constraints are not met; \ref AdapterType::eFull was specified and \ref AdapterDesc::messageQueue is
     *   missing. More information may be available with verbose logging.
     * @param desc The \ref AdapterDesc describing the adapter type.
     * @returns An \ref omni::expected type containing the pointer to the created IEventStream, or if the IEventStream
     *   could not be created, a \ref omni::unexpected containing the error.
     */
    omni::expected<IEventStreamPtr, omni::core::Result> createAdapter(const AdapterDesc& desc) noexcept
    {
        omni::core::Result result;
        if (auto p = internalCreateAdapter(desc, result))
        {
            return omni::expected<IEventStreamPtr, omni::core::Result>(carb::stealObject(p));
        }
        return omni::unexpected<omni::core::Result>(result);
    }
};

} // namespace events
} // namespace carb
