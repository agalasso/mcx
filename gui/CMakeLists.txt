if (WIN32)
  set(wxWidgets_CONFIGURATION msw)
  set(wxWidgets_USE_STATIC ON)
  set(wxWidgets_USE_DEBUG ON)
  set(wxWidgets_USE_UNICODE OFF)
endif()
find_package(wxWidgets REQUIRED COMPONENTS core base adv html)
include(${wxWidgets_USE_FILE})

add_definitions(-DVERSION=\"0.7\")

set(mcx_SRC
  main.cpp
  mcxgui.cpp
  crc16.cpp
  mcxcomm.cpp
)

if(UNIX)
  set(mcx_SRC
    ${mcx_SRC}
    mcxcomm_linux.cpp
  )
elseif(WIN32)
  set(mcx_SRC
    ${mcx_SRC}
    mcxcomm_win.cpp
  )
endif()

add_executable(mcx WIN32 ${mcx_SRC})
target_link_libraries(mcx ${wxWidgets_LIBRARIES})
