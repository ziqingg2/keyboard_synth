#ifndef _KEYTABLE_H_
#define _KEYTABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

void init_plus(void);
void init_minus(void);
void reset(void);
bool valid_key(int key);
double key2freq(int key);
void init_key2index(void);
void octave_shift(void);


#ifdef __cplusplus
}
#endif

#endif /* _KEYTABLE_H_ */
