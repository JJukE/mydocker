// Copyright (c) 2024, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto. Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

//! @file
//! @brief Utilities for *carb.eventdispatcher.plugin*
#pragma once

#include "IEventDispatcher.h"
#include "IMessageQueue.h"

namespace carb
{
namespace eventdispatcher
{

/**
 * Pops all entries from a message queue and dispatches them.
 *
 * Effectively:
 * ```cpp
 * auto ed = carb::getCachedInterface<IEventDispatcher>();
 * IMessageQueue::ExpectedType exp;
 * while ((exp = p->pop([&](const Event& e) { ed->internalDispatch(e); })).has_value())
 *     ;
 * return exp;
 * ```
 * @param p The \ref IMessageQueue to pop in a loop.
 * @returns An expected value that is the first unexpected result from \ref IMessageQueue::pop.
 */
inline IMessageQueue::ExpectedType popAllAndDispatch(IMessageQueue* p) noexcept
{
    auto ed = carb::getCachedInterface<IEventDispatcher>();
    IMessageQueue::ExpectedType exp;
    while ((exp = p->pop([&](const Event& e) { ed->internalDispatch(e); })).has_value())
        ;
    return exp;
}

} // namespace eventdispatcher
} // namespace carb
