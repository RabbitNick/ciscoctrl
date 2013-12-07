


########################################################################
# Environment setup
########################################################################
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

FIND_PACKAGE(Boost 1.48.0 REQUIRED COMPONENTS system program_options thread)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}) 


AUX_SOURCE_DIRECTORY( . DIR_CISCOCTRL_SRCS )
ADD_LIBRARY( ciscoctrl ${DIR_CISCOCTRL_SRCS} )
TARGET_LINK_LIBRARIES( ciscoctrl ${Boost_LIBRARIES})
