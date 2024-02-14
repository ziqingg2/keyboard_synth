#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdbool.h>

#include "freq.h"           /* freq_table[] = {f0, f1, f2, ... } */
static int key2index[128];  /* valid keyboard characters to freq_table index */
int octave = 0;;
    
void init_plus(void)
{
    key2index['z'] = 127;
    return;
}

void init_minus(void)
{
    key2index['x'] = 127;
    return;
}
    
void reset(void)
{
    key2index['z'] = 0;
    key2index['x'] = 0;
}

void init_ori(void)
{
    key2index['c'] = 127;
    return;
}

bool valid_key(int key)
{
    return(key2index[key] >= 0 ? true : false);
}

double key2freq(int key)
{
    double f0;
    if (valid_key(key)) {
        f0 = freq_table[key2index[key] + octave];
    }
    else {
        f0 = -1;
    }
    return(f0);
}

void init_key2index(void)
{
    /* From F3 to B4 on piano keyboard
     *
     * white keys: asdfghjkl;'
     * black keys: wer yu op[
     *
     * all other keys are unrecognized
     */
    int i;
    int shift_index;
    for (i=0; i<128; i++) {
        /* there are 128 possible ASCII characters
         * array index values are ASCII value
         * initialize to make all entries invalid (-1)
         */
        key2index[i] = -1;
    }
    /* Overwrite valid keys with a key index that is >= 0 
     * The index of key2index[] is the ASCII value of the key 
     * The value of key2index[] is the index into the freq[] table of 
     * piano key frequencies.
     */
    i = 12;
    key2index['a'] = i++; //F3
    key2index['w'] = i++;
    key2index['s'] = i++;
    key2index['e'] = i++;
    key2index['d'] = i++;
    key2index['r'] = i++;
    key2index['f'] = i++;
    key2index['g'] = i++;
    key2index['y'] = i++;
    key2index['h'] = i++;
    key2index['u'] = i++;
    key2index['j'] = i++;
    key2index['k'] = i++;
    key2index['o'] = i++;
    key2index['l'] = i++;
    key2index['p'] = i++;
    key2index[';'] = i++;
    key2index['['] = i++;
    key2index['\''] = i++; //B4
}


    
    
void octave_shift(void) {
    if (key2index['z'] == 127)
    {
        octave +=  12;
        if (octave > 24)
        {
            octave = 24;
        }
    }
    else if (key2index['x'] == 127)
    {
        octave -= 12;
        if (octave < -12)
        {
            octave = -12;
        }
    }
    octave += 0;
}
    
#ifdef __cplusplus
} // extern "C"
#endif
