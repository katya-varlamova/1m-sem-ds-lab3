#pragma once

#define UNUSED_VAR( var )                                                      \
    {                                                                          \
        static_cast<void>( var );                                              \
    }

#define DI_NAME_DECLARE( name ) extern std::function<void()> name;
#define DI_NAME_DEFINE( name ) std::function<void()> name = [] {};
