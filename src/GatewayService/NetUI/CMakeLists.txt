
set( PROJECT_NAME NetUI )
project( ${PROJECT_NAME} CXX )

add_library_( ${PROJECT_NAME} STATIC 
    DEPENDS
        oatpp::oatpp
        base
)

target_sources( ${PROJECT_NAME} 
    PRIVATE
        OatppServer.cpp
        controller/GatewayController.cpp
        Constants.hpp
        controller/CircuitBreaker.cpp
)