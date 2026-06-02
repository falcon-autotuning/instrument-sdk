cmake_minimum_required(VERSION 3.20)

project(
  {{cookiecutter.project_name}}
  VERSION 1.0.0
  DESCRIPTION "{{cookiecutter.description}}"
  HOMEPAGE_URL "{{cookiecutter.homepage_url}}"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Build Options
# ============================================================================
option(BUILD_PLUGINS "Build instrument plugins" ON)
option(USE_CCACHE "Enable ccache or sccache if available" ON)

# ============================================================================
# Windows fixes
# ============================================================================
if(WIN32)
  add_compile_definitions(NOMINMAX)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# ============================================================================
# Optional compiler cache
# ============================================================================
if(USE_CCACHE)
  find_program(SCCACHE_PROGRAM sccache)
  if(SCCACHE_PROGRAM)
    message(STATUS "Using sccache: ${SCCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER ${SCCACHE_PROGRAM})
  else()
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
      message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
      set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    endif()
  endif()
endif()

# ============================================================================
# Dependencies
# ============================================================================
find_package(isa-test-utils CONFIG REQUIRED)
find_package(cmocka CONFIG REQUIRED)
find_package(instrument-data CONFIG REQUIRED)
find_package(instrument-plugin-api CONFIG REQUIRED)

enable_testing()

# ============================================================================
# Collect input files
# ============================================================================
file(GLOB_RECURSE ISA_FILES    "${CMAKE_SOURCE_DIR}/isa/*")
file(GLOB_RECURSE CONFIG_FILES "${CMAKE_SOURCE_DIR}/tests/config/*")

set(GENERATED_INCLUDE_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/include/{{cookiecutter.project_name}})
file(MAKE_DIRECTORY ${GENERATED_INCLUDE_DIR})

# ============================================================================
# Optional plugin build
# ============================================================================
if(BUILD_PLUGINS)
  add_instrument_plugin({{cookiecutter.project_name}}_plugin
    SOURCES
      src/plugin.c
    INSTALL
  )

  set(PLUGIN_TARGET {{cookiecutter.project_name}}_plugin)
  set(PLUGIN_FILE $<TARGET_FILE:${PLUGIN_TARGET}>)
else()
  set(PLUGIN_TARGET "")
  set(PLUGIN_FILE "")
endif()

# ============================================================================
# Generate embed bundle AFTER build
# ============================================================================
set(EMBED_HEADER ${GENERATED_INCLUDE_DIR}/embedded_bundle.h)

add_custom_command(
  OUTPUT ${EMBED_HEADER}
  COMMAND ${CMAKE_COMMAND}
    -DOUTPUT=${EMBED_HEADER}
    -DISA_FILES="${ISA_FILES}"
    -DCONFIG_FILES="${CONFIG_FILES}"
    -DPLUGIN_FILE=${PLUGIN_FILE}
    -P ${CMAKE_SOURCE_DIR}/cmake/run_embed_bundle.cmake
  DEPENDS
    ${ISA_FILES}
    ${CONFIG_FILES}
    ${PLUGIN_TARGET}
  COMMENT "Generating embedded bundle"
)

add_custom_target(embed_bundle ALL DEPENDS ${EMBED_HEADER})

# ============================================================================
# Test executable
# ============================================================================
add_executable({{cookiecutter.project_name}}
  tests/test-instrument.c
)

add_dependencies({{cookiecutter.project_name}} embed_bundle)

target_include_directories({{cookiecutter.project_name}} PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_link_libraries({{cookiecutter.project_name}}
  PRIVATE
    isa-test-utils::isa-test-utils
    cmocka::cmocka
)

add_test(NAME {{cookiecutter.project_name}} COMMAND {{cookiecutter.project_name}})

# ============================================================================
# Plugin search path for runtime
# ============================================================================
set(PLUGIN_SEARCH_PATHS
  "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
  "${CMAKE_BINARY_DIR}/lib/plugins"
  "${CMAKE_BINARY_DIR}"
)

if(WIN32)
  set(PATH_SEPARATOR ";")
else()
  set(PATH_SEPARATOR ":")
endif()

string(REPLACE ";" "${PATH_SEPARATOR}"
       PLUGIN_SEARCH_PATH_ENV "${PLUGIN_SEARCH_PATHS}")

set_tests_properties({{cookiecutter.project_name}} PROPERTIES
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  ENVIRONMENT "PATH=${PLUGIN_SEARCH_PATH_ENV}${PATH_SEPARATOR}$ENV{PATH}"
)
