#define CONFIG_FILE "./clock.cfg"

enum OP_MODES { OP_HOURLY, OP_HALF_HOURLY, OP_QUARTER_HOURLY };

typedef struct {
	int op_mode;
	char arduino_path[100];
} config_t;

void load_config(const char *cfgfile, config_t *config);
