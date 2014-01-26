#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        unsigned char k[8];
        unsigned char c[4];
        unsigned char d[4];
} key_set;

void generate_sub_keys(unsigned char* main_key, key_set* key_sets);
void process_msg(unsigned char* message_piece, unsigned char* processed_piece, key_set* key_sets, int mode);
