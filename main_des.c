#include "des.h"

int main(int argc, char* argv[]) 
{
	FILE *fkey, *fin, *fout;
        unsigned long file_size;
        unsigned short int padding;

        if (argc != 5) 
	{
        	printf("ERROR: Wrong parameters!\nUsage: ./des [e|d] [key file] [source file] [destination file]\n");
        	return 1;
        }

	if(argv[1][0] == 'e' || argv[1][0] == 'd')
	{
                fkey = fopen(argv[2], "r");
                if (!fkey) 
		{
                        printf("Could not open key file\n");
                        return 1;
                }

                short int bytes_read;
                unsigned char* des_key = (unsigned char*) malloc(8*sizeof(char));
                bytes_read = fread(des_key, sizeof(unsigned char), 8, fkey);
                if (bytes_read != 8) 
		{
                        printf("Key read from key file doesn't have valid key size\n");
                        fclose(fkey);
                        return 1;
                }
                fclose(fkey);

                fin = fopen(argv[3], "r");
                fout = fopen(argv[4], "w");

                short int bytes_written, mode;
                unsigned long block_count = 0, number_of_blocks;
                unsigned char* data_block = (unsigned char*) malloc(8*sizeof(char));
                unsigned char* processed_block = (unsigned char*) malloc(8*sizeof(char));
                key_set* key_sets = (key_set*)malloc(17*sizeof(key_set));

                generate_sub_keys(des_key, key_sets);
		printf("des_key=%s\n",des_key);

		if(argv[1][0] == 'e')
                        mode = 1;
		else 
                        mode = 0;

                fseek(fin, 0L, SEEK_END);
                file_size = ftell(fin);

                fseek(fin, 0L, SEEK_SET);
printf("file_size=%d\n",file_size);
                number_of_blocks = file_size/8 + ((file_size%8)?1:0);
printf("number_of_blocks=%d\n",number_of_blocks);

                while(fread(data_block, 1, 8, fin)) 
		{
                        block_count++;
printf("block_count=%d\n",block_count);
                        if (block_count == number_of_blocks) 
			{
                                if (mode == 1) 
				{
                                        padding = 8 - file_size%8;
                                        if (padding < 8) 
					{ 
                                                memset((data_block + 8 - padding), (unsigned char)padding, padding);
                                        }

                                        process_msg(data_block, processed_block, key_sets, mode);
                                        bytes_written = fwrite(processed_block, 1, 8, fout);
printf("bytes1=%d\n",bytes_written);

                                        if (padding == 8) 
					{
                                                memset(data_block, (unsigned char)padding, 8);
                                                process_msg(data_block, processed_block, key_sets, mode);
                                                bytes_written = fwrite(processed_block, 1, 8, fout);
printf("bytes2=%d\n",bytes_written);
                                        }
                                } 
				else 
				{
                                        process_msg(data_block, processed_block, key_sets, mode);
                                        padding = processed_block[7];
printf("data_block=%s\n",data_block);

                                        if (padding < 8)
                                                bytes_written = fwrite(processed_block, 1, 8 - padding, fout);
                                }
                        } 
			else 
			{
                                process_msg(data_block, processed_block, key_sets, mode);
                                bytes_written = fwrite(processed_block, 1, 8, fout);
printf("bytes3=%d\n",bytes_written);
                        }
                        memset(data_block, 0, 8);
printf("!processed_block=%s\n",processed_block);
                }

                free(des_key);
                free(data_block);
                free(processed_block);
                fclose(fin);
                fclose(fout);

                return 0;
        }
	else 
	{
                printf("ERROR: Wrong parameter!\nFirst parameter must be [e | d]\n", argv[1]);
                return 1;
        }

        return 0;
}
