#include "barcode_module.h"

// init global variables
bool         g_barcode_detected          = false;
int          g_barcode[ARRAY_SIZE]       = {0};
int          g_index                     = 0;
const char * barcode_values_array[44][2] = 
{
    {"111221211","0"},
    {"211211112","1"},
    {"112211112","2"},
    {"212211111","3"},
    {"111221112","4"},
    {"211221111","5"},
    {"112221111","6"},
    {"111211212","7"},
    {"211211211","8"},
    {"112211211","9"},
    {"211112112","A"},
    {"112112112","B"},
    {"212112111","C"},
    {"111122112","D"},
    {"211122111","E"},
    {"112122111","F"},
    {"111112212","G"},
    {"211112211","H"},
    {"112112211","I"},
    {"111122211","J"},
    {"211111122","K"},
    {"112111122","L"},
    {"212111121","M"},
    {"111121122","N"},
    {"211121121","O"},
    {"112121121","P"},
    {"111111222","Q"},
    {"211111221","R"},
    {"112111221","S"},
    {"111121221","T"},
    {"221111112","U"},
    {"122111112","V"},
    {"222111111","W"},
    {"121121112","X"},
    {"221121111","Y"},
    {"122121111","Z"},
    {"121111212","-"},
    {"221111211","."},
    {"122111211"," "},
    {"121212111","$"},
    {"121211121","/"},
    {"121112121","+"},
    {"111212121","%"},
    {"121121211","*"}
};

int main ()
{
    // init ir sensor
    stdio_init_all();
    ir_sensor_init();

    for (;;)
    {

        // if all barcode ascii is collected
        if (g_barcode[18] != 0)
        {
            // temp barcode array
            int temp_barcode[9] = {0};

            // copy barcode array from index 10 to 18, to temp barcode array
            memcpy(temp_barcode, &g_barcode[10], sizeof(temp_barcode));

            printf("temp barcode: ");
            for (int i = 0; i < TEMP_ARRAY_SIZE; i++)
            {
                printf("[%d]", temp_barcode[i]);
            }
            printf("\n");

            // scale down barcode & store in string
            char * barcode_ascii_string = scale_down_barcode(temp_barcode);
            printf("%s\n", barcode_ascii_string);

            // convert barcode ascii to barcode value
            char * barcode_value_string = convert_barcode(barcode_ascii_string);

            // print barcode value
            printf("%s\n", barcode_value_string);
            
            // reset g_index
            g_index = 0;

            // clear barcode array
            memset(g_barcode, 0, sizeof(g_barcode));

        }

        // print out pulse width of barcode
        print_barcode(g_barcode);
        sleep_ms(1000);

    }

    return 0;
}