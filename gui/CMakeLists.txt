find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

add_definitions(-DVERSION=\"0.7\")

add_executable(mcx main.cpp mcxgui.cpp crc16.cpp mcxcomm.cpp mcxcomm_linux.cpp)
target_link_libraries(mcx ${wxWidgets_LIBRARIES})
