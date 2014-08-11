#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

#define LINE_BUFSIZE 200
#define SEP_CHAR ':'
#define COMP_STR(str1, str2) ( strcmp(str1, str2) == 0 )

#define FALSE 0
#define TRUE 1

typedef struct {
	int op_mode;
	char arduino_path[100];
} config_t;

enum OP_MODES { OP_HOURLY, OP_HALF_HOURLY, OP_QUARTER_HOURLY };

static void cp_str_pos(char *dest, char *src, int start, int end, int max)
{
	int i, base = 0;
	for(i=start; i<end; i++) {
		dest[base] = src[i];
		base++;
		if( i + 2 > max) break;
	}
	dest[base] = 0;
}

void load_config(const char *cfgfile, config_t *config)
{
	FILE *fp;
	if( ! (fp = fopen(cfgfile, "r")) ) {
		fprintf(log_fp, "Error: couldn't open configfile\n");
		exit(EXIT_FAILURE);
	}
	else {
		fprintf(log_fp, "Loading configfile \"%s\"...\n", cfgfile);
	}
	
	
	char line_buffer[LINE_BUFSIZE] = { 0 };
	char ref_buffer[100] = { 0 };
	char val_buffer[100] = { 0 };
	while( fgets(line_buffer, LINE_BUFSIZE - 1, fp) ) {
	
		int in_quote = FALSE;
		int pos;
		int end = FALSE;
		int start_ref_str = -1, end_ref_str = 0;
		int start_val_str = 0, end_val_str = 0;
		char current_ch;
		
		for(pos=0; pos<LINE_BUFSIZE; pos++ ) {
			if( end ) break;
			current_ch = line_buffer[pos];
			switch( current_ch ) {
				case ' ':
					if(! in_quote) {
						end_ref_str = pos;					
						pos++;
					}
				case '\"':
					in_quote = in_quote ? FALSE : TRUE;
					if( in_quote ) start_val_str = pos + 1;
					else end_val_str = pos;
					pos++;
					break;
				case '\0':
					in_quote = FALSE;
					end = TRUE;
					break;
				default:
					if( start_ref_str == -1 ) start_ref_str = pos;
			}
			//printf("current_ch=%c start_ref_str=%d end_ref_str=%d\n", current_ch, start_ref_str, end_ref_str);
		}
		cp_str_pos(ref_buffer, line_buffer, start_ref_str, end_ref_str, 100);
		cp_str_pos(val_buffer, line_buffer, start_val_str, end_val_str, 100);
		fprintf(log_fp, "Config read: \"%s: %s\"\n", ref_buffer, val_buffer);
		
		if( COMP_STR(ref_buffer, "op_mode") ) {
			if( COMP_STR(val_buffer, "half-hourly") ) config->op_mode = OP_HALF_HOURLY;
			else if( COMP_STR(val_buffer, "quarter-hourly") ) config->op_mode = OP_QUARTER_HOURLY;
			else config->op_mode = OP_HOURLY;
		}
		else if( COMP_STR(ref_buffer, "arduino_path") ) {
			strncpy(config->arduino_path, val_buffer, 99);
		}
		
		memset(ref_buffer, '\0', sizeof(ref_buffer));
		memset(val_buffer, '\0', sizeof(val_buffer));
		
	}
}
