#ifndef _ROM_HOMEKIT_ED25519_H_
#define _ROM_HOMEKIT_ED25519_H_

#define ED25519_SKEY_LEN      32
#define ED25519_PUBKEY_LEN    32
#define ED25519_SIG_SIZE      64

int rom_ed25519_verify_signature(const unsigned char sig[ED25519_SIG_SIZE],
                                 const unsigned char *m, unsigned long long mlen,
                                 const unsigned char pk[ED25519_PUBKEY_LEN]);

/* internal */
int boot_rom_ed25519_crypto_sign_detached(unsigned char *sig, unsigned long long *siglen,
                                 const unsigned char *m, unsigned long long mlen,
                                 const unsigned char *sk);
#endif	/* _ROM_HOMEKIT_ED25519_H_ */
