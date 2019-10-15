// Copyright 2019 Pokitec
// All rights reserved.

#pragma once

#include "TTauri/Foundation/exceptions.hpp"
#include <gsl/gsl>
#include <string>
#include <unordered_map>
#include <cstddef>
#include <thread>
#include <mutex>

namespace TTauri {

struct FoundationGlobals;
inline FoundationGlobals *Foundation_globals = nullptr;

struct FoundationGlobals {
private:
    std::unordered_map<std::string,gsl::span<std::byte const>> staticResources;

    std::thread maintenanceThread;
    bool _stopMaintenanceThread = false;

    mutable std::mutex mutex;

public:
    date::time_zone const *time_zone = nullptr;
    std::thread::id main_thread_id;
    std::function<void(std::function<void()>)> main_thread_runner;
    std::string applicationName;

    FoundationGlobals(std::thread::id main_thread_id, std::string applicationName, URL tzdata_location) noexcept;
    ~FoundationGlobals();
    FoundationGlobals(FoundationGlobals const &) = delete;
    FoundationGlobals &operator=(FoundationGlobals const &) = delete;
    FoundationGlobals(FoundationGlobals &&) = delete;
    FoundationGlobals &operator=(FoundationGlobals &&) = delete;

    void addStaticResource(std::string const &key, gsl::span<std::byte const> value) noexcept;

    gsl::span<std::byte const> getStaticResource(std::string const &key) const;

    void FoundationGlobals::stopMaintenanceThread() noexcept;
    void maintenanceThreadProcedure() noexcept;
};

}