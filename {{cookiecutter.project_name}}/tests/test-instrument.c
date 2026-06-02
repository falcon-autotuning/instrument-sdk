
#include "{{ cookiecutter.project_name }}/embedded_bundle.h"
#include <cmocka.h>
#include <isa-test-utils.h>
#include <stdio.h>

#ifdef _WIN32
#define ISA_PATH "..\\isa\\{{ cookiecutter.project_name }}.yml"
#define PLUGIN_OBJ "{{ cookiecutter.project_name }}.dll"
#else
#define ISA_PATH "../isa/{{ cookiecutter.project_name }}.yml"
#define PLUGIN_OBJ "{{ cookiecutter.project_name }}.so"
#endif

static const float EPSILON = 1e-5;
static const char *kInstrumentName = "{{ cookiecutter.instrument_name }}";

typedef struct {
  EnvLocations env;
  Path *config_path;
  Path *plugin_path;
} TestContext;

static int global_group_setup(void **state) {
  TestContext *ctx = calloc(1, sizeof(TestContext));
  if (!ctx)
    return -1;
  // Here you can load environment variables, set up files, or do any other
  // necessary setup for your tests. We recomend storing setup specific
  // parameters as ENV varaibles so recompilation is not necessary for changes
  // to the test setup.
  ctx->env = prepare_environment("__API_REF__", ISA_PATH, NULL);
  ctx->config_path = path_clone(ctx->env.config_dir);
  path_push(ctx->config_path, "my-{{ cookiecutter.project_name }}.yml");
  ctx->plugin_path = path_clone(ctx->env.plugin_dir);
  path_push(ctx->plugin_path, PLUGIN_OBJ);
  *state = ctx;
  return 0;
}

static int global_group_teardown(void **state) {
  TestContext *ctx = (TestContext *)*state;
  if (!ctx)
    return 0;
  cleanup_environment(&ctx->env);
  path_free(ctx->config_path);
  path_free(ctx->plugin_path);
  free(ctx);
  *state = NULL;
  return 0;
}

static int test_setup_server_fixture(void **state) {
  TestContext *ctx = (TestContext *)*state;
  assert_non_null((void *)ctx);
  start_server();
  start_instrument(ctx->config_path, ctx->plugin_path);
  return 0;
}

static int test_teardown_server_fixture(void **state) {
  (void)state;
  stop_instrument(kInstrumentName);
  stop_server();
  return 0;
}

static void test_startup(void **state) {
  (void)state;
  char *status = instrument_status(kInstrumentName);

  assert_non_null((void *)status);
  assert_string_not_equal(status, "");

  printf("%s status:\n%s\n", kInstrumentName, status);

  free(status);
}

/* Template test fomat
static void test_<TEST_NAME>(void **state) {
  (void)state;
  --- setup temp variables ---
  ...

  --- prepare measurement script ---
  const char *script =
      flatten_yaml("function main(ctx, voltage)\n"
                   "  ctx:log(\"Starting test for %s \")\n"
                   ...
                   "  ctx:log(\"Main function complete\")\n"
                   "end\n",kInstrumentName);

  --- pass in variables to the ISS ---
  Map *map = map_new();
  map_add_...

  --- run the measurement ---
  const Result *output = perform_measurement(script, map);

  --- check the output using cmocka ---
  assert_non_null(output);
  assert_string_equal(output->status, "success");
  ...

  -- free any necessary variables like the temp ones ---
  free_result(output);
  map_free(map);
  ...
}
*/

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  // Use cmocka_unit_test_setup_teardown to automatically wrap each individual
  // test
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup_teardown(test_startup, test_setup_server_fixture,
                                      test_teardown_server_fixture),
      /*
            --- insert your tests here ---
            cmocka_unit_test_setup_teardown(test_<TEST_NAME>,
                                            test_setup_server_fixture,
                                            test_teardown_server_fixture),
      */
  };

  // Bind the global group setup and teardown hooks to manage files
  // automatically
  return cmocka_run_group_tests(tests, global_group_setup,
                                global_group_teardown);
}
