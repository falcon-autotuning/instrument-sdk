# Instrument SDK (Cookiecutter Template)

This repository is a **Cookiecutter template** for creating instrument plugins for the Instrument Script Server.

It provides a ready-to-use project scaffold including:

* Plugin implementation structure
* Test harness and example tests
* CMake build system
* Embedded test assets (ISA + config + plugin)
* Integration with `instrument-plugin-api` and testing utilities

***

# Getting Started

## 1. Install Cookiecutter

```bash
pip install cookiecutter
```

***

## 2. Generate a New Plugin Project

Run:

```bash
cookiecutter https://github.com/falcon-autotuning/instrument-sdk
```

You will be prompted for inputs such as:

```
project_name: my-instrument
description: My custom instrument plugin
homepage_url: https://github.com/me/my-instrument
```

***

## 3. Result

Cookiecutter will generate a new folder:

```
my-instrument/
```

Inside it:

```
my-instrument/
  CMakeLists.txt
  src/
    plugin.c
  tests/
    test-instrument.c
  isa/
  cmake/
```

All placeholders (e.g. names, paths, identifiers) are automatically filled in.

***

# How It Works

## Template Structure

This repository contains:

```
instrument-sdk/
  cookiecutter.json
  {{cookiecutter.project_name}}/
    ...
```

### `cookiecutter.json`

Defines:

* User inputs
* Derived variables (e.g. uppercase name)
* Naming transformations

Example:

```json
{
  "project_name": "my-instrument",
  "instrument_name": "{{ cookiecutter.project_name | replace(' ', '_') | replace('-', '_') | upper }}"
}
```

### `{{cookiecutter.project_name}}/`

This is the **template project directory**.

All files inside it may contain Jinja expressions such as:

```c
{{ cookiecutter.project_name }}
{{ cookiecutter.instrument_name }}
```

These are replaced when the project is generated.

***

# Generated Project Overview

## Plugin

* Located in:

  ```
  src/plugin.c
  ```

* Implements the required plugin API:

  ```c
  plugin_get_metadata()
  plugin_initialize()
  plugin_execute_command()
  plugin_shutdown()
  ```

***

## Tests

* Located in:

  ```
  tests/test-instrument.c
  ```

* Uses:
  * `cmocka`
  * `isa-test-utils`

* Automatically:
  * Starts server
  * Loads your plugin
  * Runs test commands

***

## Embedded Assets

The build system bundles:

* ISA YAML files
* Config files
* Compiled plugin

Into a generated header:

```
build/include/<project>/embedded_bundle.h
```

This enables **self-contained testing** with no external files.

***

## Build System (CMake)

The generated project uses:

* `instrument-plugin-api`
* `isa-test-utils`
* `cmocka`

### Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

***

### Run Tests

```bash
ctest
```

***

# Customization

After generation, you are expected to:

### Modify plugin logic

Edit:

```
src/plugin.c
```

Add:

* instrument communication
* command handling
* data processing

***

### Add tests

Edit:

```
tests/test-instrument.c
```

Use the provided template:

```
test_<NAME>()
```

***

### Add ISA definitions

Update:

```
isa/
```

***

# Design Goals

This template is designed to:

* ✅ Minimize setup friction
* ✅ Provide a working baseline
* ✅ Encourage structured plugin development
* ✅ Support reproducible, embedded testing
* ✅ Work cross-platform

***

# Important Notes

* This is a **template**, not a finished project
* Generated projects require **user modification**
* The plugin logic is intentionally minimal
* Tests are scaffolding—not complete coverage

***

# 📦 Related Projects

* **instrument-plugin-api**  
  C ABI for plugin development

* **isa-test-utils**  
  Test framework and embedding system
