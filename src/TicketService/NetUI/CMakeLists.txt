
set( PROJECT_NAME NetUI )
project( ${PROJECT_NAME} CXX )

add_library_( ${PROJECT_NAME} STATIC 
    DEPENDS
        oatpp::oatpp
        base
        BL
)

target_sources( ${PROJECT_NAME} 
    PRIVATE
        IBLFacade.h
        oatpp/OatppDTOs.h
        oatpp/OatppServerController.h
        oatpp/OatppServerController.cpp
        oatpp/OatppServerSettings.h
        oatpp/OatppServer.cpp
        oatpp/OatppServer.h
        IServer.h
)