set(FLUID_CORE_MODULES proto_desc memory lod_tensor executor init)

# TODO(panyx0718): Should this be called paddle_fluid_inference_api_internal?
cc_library(paddle_fluid_api
    SRCS io.cc
    DEPS ${FLUID_CORE_MODULES} ${GLOB_OP_LIB})

# Create static library
get_property(fluid_modules GLOBAL PROPERTY FLUID_MODULES)
cc_library(paddle_fluid DEPS ${fluid_modules})

# Create shared library
cc_library(paddle_fluid_shared SHARED
    SRCS io.cc
    DEPS ${fluid_modules})
set_target_properties(paddle_fluid_shared PROPERTIES OUTPUT_NAME paddle_fluid)
if(NOT APPLE)
  # TODO(liuyiqun): Temporarily disable the link flag because it is not support on Mac.
  set(LINK_FLAGS "-Wl,--version-script ${CMAKE_CURRENT_SOURCE_DIR}/paddle_fluid.map")
  set_target_properties(paddle_fluid_shared PROPERTIES LINK_FLAGS "${LINK_FLAGS}")
endif()

if(WITH_TESTING)
  # both tests/book and analysis depends the models that generated by python/paddle/fluid/tests/book
  add_subdirectory(tests/book)
  add_subdirectory(analysis)
endif()

if (TENSORRT_FOUND)
  add_subdirectory(tensorrt)
endif()
