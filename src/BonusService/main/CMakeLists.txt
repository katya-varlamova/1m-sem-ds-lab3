set( PROJECT_NAME main )
project( ${PROJECT_NAME} CXX )

add_executable_( ${PROJECT_NAME} CXX_20
        DEPENDS
        ${Boost_LIBRARIES}
        Boost::boost
        BL
        DA
        NetUI
        )

target_sources( ${PROJECT_NAME}
        PRIVATE
        main.cpp
        )