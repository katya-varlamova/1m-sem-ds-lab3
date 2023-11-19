if ( WIN32 )
    find_package(PostgreSQL REQUIRED) #libpqxx::pqxx

    add_library( libpqxx INTERFACE IMPORTED )
    set_target_properties( libpqxx PROPERTIES IMPORTED_GLOBAL TRUE )
    target_link_libraries( libpqxx INTERFACE ${PostgreSQL_LIBRARIES})
    add_library( libpqxx::pqxx ALIAS libpqxx )
elseif( APPLE )
    add_library( libpqxx INTERFACE IMPORTED )

    set_target_properties( libpqxx PROPERTIES IMPORTED_GLOBAL TRUE )
    target_include_directories( libpqxx INTERFACE "/usr/local/Cellar/postgresql/include" )
    if ( ${LIBPQXX_LINK_STATIC} )
        target_link_libraries( libpqxx INTERFACE "/usr/local/Cellar/libpq/libpq.a" )
    else()
        target_link_libraries( libpqxx INTERFACE "/usr/local/Cellar/libpq/libpq.dylib" )
    endif()

    add_library( libpqxx::pqxx ALIAS libpqxx )
else()
    add_library( libpqxx INTERFACE IMPORTED )

    set_target_properties( libpqxx PROPERTIES IMPORTED_GLOBAL TRUE )
    target_include_directories( libpqxx SYSTEM INTERFACE "/usr/include" "/usr/include/postgresql")
    if ( ${LIBPQXX_LINK_STATIC} )
        target_link_libraries( libpqxx INTERFACE "/usr/lib/x86_64-linux-gnu/libpq.a" )
    else()
        target_link_libraries( libpqxx INTERFACE "/usr/lib/x86_64-linux-gnu/libpq.so" )
    endif()

    add_library( libpqxx::pqxx ALIAS libpqxx )
endif()

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS filesystem unit_test_framework date_time REQUIRED ) #boost::unit_tests boost::filesystem boost::beast etc
find_package(yaml-cpp REQUIRED)
find_package(spdlog REQUIRED) # spdlog::spdlog
find_package(OpenSSL REQUIRED) # OpenSSL::SSL OpenSSL::Crypto
find_package(cpp-jwt REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(oatpp REQUIRED)