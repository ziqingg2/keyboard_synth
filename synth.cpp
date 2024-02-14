#include <stdio.h>
#include <stdlib.h> //for exit()
#include <math.h>   //for sin()
#include "synth.h"

/* constructor */
Synth::Synth(void){
    ;
}

/* destructor */
Synth::~Synth(void) 
{
    ;
}

void Synth::init_synth(int in_samp_rate)
{
    Tone *pt = &this->tone[0]; /* Tone structure */
    this->num_keys = 0; //number of keys voiced
    for (int i=0; i<KEYS_VOICED; i++) {
        pt[i].key = ' '; //space
    }
    this->samp_rate = in_samp_rate;
    this->ampl = FS_AMPL;
    this->attack_factor = ATTACK_FACTOR;
    this->decay_factor = DECAY_FACTOR;
    this->drop_level = DROP_LEVEL;
}

void Synth::execute_cmd(int in_cmd)
{
    switch (in_cmd) {
        case (CMD_ADD_KEY):
            add_key(this->new_freq);
            break;
        case (CMD_RM_KEY):
            rm_key();
            break;
    }
}

double *Synth::synth_block(int framesPerBuffer)
{
    /* check for actions from user interface thread */
    //Your Code Here
    int local_cmd = this->cmd;
    if (local_cmd > 0)
    {
        this->execute_cmd(local_cmd);
        this->cmd = 0;
    }

    /* synthesize tones */
    //Your Code Here
    Tone *pt = &this->tone[0];
    for (int m = 0; m < framesPerBuffer; m++)
    {
        output[m] = 0.0;
    }
    
    for (int i = 0; i < framesPerBuffer; i++)
    {
        double v = 0;
        for (int j  = 0; j < this->num_keys; j++)
        {
            if (pt[j].phase_inc > -1 )
           {
                v += this->ampl * sin(pt[j].phase + 0.8*sin(pt[j].phase*60/80)) * (1 - pt[j].attack_amp) * pt[j].decay_amp;
                pt[j].phase += pt[j].phase_inc;
                pt[j].attack_amp *=  this->attack_factor;
                pt[j].decay_amp *=  this->decay_factor;
                if (pt[j].decay_amp < this->drop_level)
                {
                    this->rm_key();
                }
            }
        }
        output[i] += v;
    }
    
    return( &output[0] );
}

void Synth::add_key(double new_freq)
{
    this->new_freq = new_freq;
    /* increment number of keys */
    if (++(this->num_keys) > KEYS_VOICED) {
        /* list is full, so remove oldest key by shifting key list */
        shift_keys();
        this->num_keys = KEYS_VOICED;
    }
    /* enter new key info */
    init_key(new_freq);
}

void Synth::rm_key(void)
{
    /* remove oldest key by shifting key list down */
    shift_keys();
    /* decreast number of keys voicing */
    /* number of keys can never be less than zero */
    if (--(this->num_keys) < 0) {
        this->num_keys = 0;
    }
}

void Synth::shift_keys(void)
{
    Tone *pt = &this->tone[0];
    /* shift tone array values down by one place */
    for (int i=0; i<KEYS_VOICED-1; i++) {
        pt[i].key = pt[i+1].key;
        pt[i].f0 = pt[i+1].f0;
        pt[i].phase_inc = pt[i+1].phase_inc;
        pt[i].phase = pt[i+1].phase;
        pt[i].attack_factor = pt[i+1].attack_factor;
        pt[i].decay_factor = pt[i+1].decay_factor;
        pt[i].attack_amp = pt[i+1].attack_amp;
        pt[i].decay_amp = pt[i+1].decay_amp;
    }
    /* top array entry is initialized to inactive */
    int n = KEYS_VOICED-1;
    pt[n].key = ' ';
    pt[n].phase_inc = -1;
}

void Synth::init_key(double freq)
{
    /* add new key information at num_keys-1 index in Tone array */
    int n;
    Tone *pt = &this->tone[0];

    n = this->num_keys-1; /* from number to index */
    if (n < 0) {
        /* since init_key() is called after ++num_keys, this should not occur */
        fprintf(stderr, "ERROR in init_key(): num_keys %d\n", this->num_keys);
        exit(-1);
    }
    pt[n].attack_factor = ATTACK_FACTOR;
    pt[n].decay_factor = DECAY_FACTOR;
    pt[n].attack_amp = 1;
    pt[n].decay_amp = 1;
    pt[n].phase_inc = 2*PI*freq/this->samp_rate;
    pt[n].phase = 0.0;
}
