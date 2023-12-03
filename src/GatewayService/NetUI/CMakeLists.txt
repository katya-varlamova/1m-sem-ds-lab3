
set( PROJECT_NAME NetUI )
project( ${PROJECT_NAME} CXX )

add_library_( ${PROJECT_NAME} STATIC 
    DEPENDS
        oatpp::oatpp
        Boost::boost
#        amqpcpp
        base
)

target_sources( ${PROJECT_NAME} 
    PRIVATE
        OatppServer.cpp
        controller/GatewayController.cpp
        Constants.hpp
        controller/CircuitBreaker.cpp
)
