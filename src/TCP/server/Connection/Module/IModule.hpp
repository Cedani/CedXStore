#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

namespace mdl {

    typedef enum {
        Notification
    } ModuleType;

    class IModule {
        virtual ~IModule() = default;

        virtual ModuleType getType() = 0;
    };

    // class IModule;
    class Notification;
    // class LauncherProfile;
}