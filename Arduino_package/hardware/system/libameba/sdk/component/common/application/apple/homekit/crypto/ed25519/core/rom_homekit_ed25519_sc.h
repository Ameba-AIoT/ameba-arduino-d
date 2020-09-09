#ifndef ROM_HOMEKIT_ED25519_SC_H
#define ROM_HOMEKIT_ED25519_SC_H

/*
The set of scalars is \Z/l
where l = 2^252 + 27742317777372353535851937790883648493.
*/
#if 0
#define sc_reduce crypto_sign_ed25519_ref10_sc_reduce
#define sc_muladd crypto_sign_ed25519_ref10_sc_muladd
#endif
void rom_ed25519_sc_reduce(unsigned char *);
void rom_ed25519_sc_muladd(unsigned char *,const unsigned char *,const unsigned char *,const unsigned char *);

#endif /* ROM_HOMEKIT_ED25519_SC_H */
