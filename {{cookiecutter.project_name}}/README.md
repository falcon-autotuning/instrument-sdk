# {{ cookiecutter.project_name }}

Instrument plugin project generated from the Instrument SDK template.

This project provides a starting point for developing and testing instrument integrations using the Instrument Script Server. It includes:

* Plugin implementation scaffold (optional)
* Integrated test harness
* Embedded configuration and ISA files
* CMake-based build system

***

# Getting Started

## Build the Project

```bash
make build
```

***

## Run Tests

```bash
make test
```

This will:

1. Start the instrument server
2. Load the configured plugin (if present)
3. Execute the test suite

***

# Project Structure

```
{{ cookiecutter.project_name }}/
  CMakeLists.txt
  plugin/
    plugin.c             # Plugin implementation (optional)
  tests/
    test-instrument.c    # Test harness
    config/              # Test configuration files
  isa/
    *.yml                # Instrument API definitions
  cmake/
    run_embed_bundle.cmake
```

***

# Plugin Implementation

## When Is a Plugin Required?

Implementing a plugin in this project is **optional**.

You only need to implement a custom plugin if your instrument is **not supported by an existing plugin** provided by the Instrument Script Server (such as VISA or other protocol-based plugins).

***

## Use Existing Plugins When Possible

If your instrument is compatible with an existing plugin (for example via):

* VISA
* Other protocol adapters

You should not implement a custom plugin.

Instead, configure your instrument appropriately and focus on defining its API and tests.

***

## If You Do Not Need a Plugin

If an existing plugin already satisfies your instrument requirements, you should remove the plugin scaffold from this project.

### Remove plugin source

```bash
rm -r plugin/
```

***

### Update CMake configuration

Either disable plugin builds:

```bash
cmake -DBUILD_PLUGINS=OFF ..
```

Or remove plugin-related logic from `CMakeLists.txt`, such as:

```cmake
if(BUILD_PLUGINS)
  add_instrument_plugin(...)
endif()
```

***

## If You Do Need a Plugin

Implement your plugin in:

```
plugin/plugin.c
```

You must define the following interface:

```c
plugin_get_metadata()
plugin_initialize()
plugin_execute_command()
plugin_shutdown()
```

Within `plugin_execute_command`, you will:

1. Inspect `cmd->verb`
2. Parse parameters
3. Execute instrument-specific logic
4. Populate the `PluginResponse`

***

## Example Command Handling

```c
if (strcmp(cmd->verb, "IDN") == 0) {
    resp->success = true;
    strncpy(resp->text_response, "My Instrument", PLUGIN_MAX_PAYLOAD);
    return 0;
}
```

***

## Recommendation

Always check whether your instrument can use an existing plugin before creating a new one. This reduces duplication and simplifies maintenance.

***

# Testing

Tests are located in:

```
tests/test-instrument.c
```

The test system:

* Uses `cmocka`
* Manages server lifecycle automatically
* Loads configuration and plugin artifacts from embedded data

***

## Adding Tests

Use the following pattern:

```c
static void test_feature(void **state) {
  (void)state;

  const Result *output = perform_measurement(script, map);

  assert_non_null(output);
  assert_string_equal(output->status, "success");

  free_result(output);
}
```

Then register it in the test array:

```c
cmocka_unit_test_setup_teardown(test_feature,
                               test_setup_server_fixture,
                               test_teardown_server_fixture),
```

***

# Embedded Bundle System

During the build process, the following assets are embedded into a generated header:

* ISA definition files (`isa/`)
* Test configuration files (`tests/config/`)
* Compiled plugin binary (if enabled)

Generated file:

```
build/include/{{ cookiecutter.project_name }}/embedded_bundle.h
```

This enables:

* Fully self-contained test execution
* No reliance on external file paths
* Reproducible environments

***

# Build Options

## Enable or Disable Plugin Build

```bash
cmake -DBUILD_PLUGINS=ON ..
```

or

```bash
cmake -DBUILD_PLUGINS=OFF ..
```

***

# Customization Checklist

After generating this project, you should:

* Implement or remove the plugin depending on compatibility
* Define your instrument API in `isa/`
* Extend and adapt tests in `tests/`
* Update configuration files as needed

***

# Summary

This project provides:

* A CMake-based development environment
* An optional plugin implementation scaffold
* A complete test harness with embedded data
* Integration with the Instrument Plugin API

***

# Final Note

This repository is a starting point for development. The generated project is not complete out of the box and is intended to be tailored to your specific instrument and use case.
