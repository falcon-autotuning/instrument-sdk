#include <instrument-plugin-api/instrument-plugin.h>
/**
 * @brief Retrieve plugin metadata.
 *
 * Called before initialization to:
 * - Verify API compatibility
 * - Identify plugin capabilities
 *
 * @return PluginMetadata structure (by value)
 */
PluginMetadata plugin_get_metadata(void) {

  -- -implement here-- -

};

/**
 * @brief Initialize the plugin.
 *
 * Called once after loading.
 *
 * @param config Configuration data (must not be modified)
 * @return 0 on success, non-zero on failure
 */
int32_t plugin_initialize(const PluginConfig *config) {

  -- -implement here-- -

};

/**
 * @brief Execute a command.
 *
 * Called for each operation requested by the host.
 *
 * @param cmd Input command (read-only)
 * @param resp Output response (must be populated by plugin)
 *
 * @return 0 on success, non-zero on failure
 *
 * @note
 * - Must set resp->success
 * - Must copy command_id and instrument_name into response
 */
int32_t plugin_execute_command(const PluginCommand *cmd, PluginResponse *resp) {

  -- -implement here-- -

};
/**
 * @brief Shutdown the plugin.
 *
 * Called before unloading.
 *
 * Use this to release:
 * - Connections
 * - Memory
 * - Hardware resources
 */
void plugin_shutdown(void) {

  -- -implement here-- -

};
