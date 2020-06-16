// Copyright 2019 Pokitec
// All rights reserved.

#include "TTauri/Foundation/fast_mutex.hpp"
#include "TTauri/Foundation/logger.hpp"
#include <Windows.h>
#include <Synchapi.h>

namespace tt {

/*
 * semaphore value:
 *  0 - Unlocked, no other thread is waiting.
 *  1 - Locked, no other thread is waiting.
 *  2 - Locked, zero or more threads are waiting.
 */

fast_mutex::fast_mutex() noexcept {}
fast_mutex::~fast_mutex() {}

tt_no_inline void fast_mutex::lock_contented(int32_t expected) noexcept
{
    do {
        ttlet should_wait = expected == 2;

        // Set to 2 when we are waiting.
        expected = 1;
        if (should_wait || semaphore.compare_exchange_strong(expected, 2)) {
            // Casting first memory of a struct is allowed.
            expected = 2;
            if (!WaitOnAddress(semaphore_ptr(), &expected, sizeof (int32_t), INFINITE)) {
                LOG_FATAL("Could not wait on address {}", getLastErrorMessage());
            }
        }

        // Set to 2 when aquiring the lock, so that during unlock we wake other waiting threads.
        expected = 0;
    } while (!semaphore.compare_exchange_strong(expected, 2));
}

void fast_mutex::lock() noexcept
{
    // Switch to 1 means there are no waiters.
    int32_t expected = 0;
    if (!semaphore.compare_exchange_strong(expected, 1)) {
        lock_contented(expected);
    }
}

void fast_mutex::unlock() noexcept
{
    if (semaphore.fetch_sub(1) != 1) {
        semaphore.store(0);

        WakeByAddressSingle(semaphore_ptr());
    }
}

}
