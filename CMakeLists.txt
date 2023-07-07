# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
add_definitions(-DLINUX_PLATFORM -DBUFFER_OPTIMIZATION) # do not edit!

# edit: choose eve platform
#add_definitions(-DEVE_1)
#add_definitions(-DEVE_2)
add_definitions(-DEVE_3)
#add_definitions(-DEVE_4)

# edit: choose clock source for eve4 platform
#add_definitions(-DEVE_4_INTERNAL_OSC)
#add_definitions(-DEVE_4_EXTERNAL_OSC)

# edit: choose display type
#add_definitions(-DNTP_35)
#add_definitions(-DRTP_35)
#add_definitions(-DCTP_35)
#add_definitions(-DNTP_43)
#add_definitions(-DRTP_43)
#add_definitions(-DCTP_43)
#add_definitions(-DNTP_50)
#add_definitions(-DRTP_50)
#add_definitions(-DCTP_50)
#add_definitions(-DNTP_70)
#add_definitions(-DRTP_70)
add_definitions(-DCTP_70)
#add_definitions(-DIPS_35)
#add_definitions(-DIPS_43)
#add_definitions(-DIPS_50)
#add_definitions(-DIPS_70)
#add_definitions(-DIPS_101)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(riverdi-eve-demo-esp32)
