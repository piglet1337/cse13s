#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>

//
// Generates the components for a new public RSA key.
// p and q will be large primes with n their product.
// The product n will be of a specified minimum number of bits.
// The primality is tested using Miller-Rabin.
// The public exponent e will have around the same number of bits as n.
// All mpz_t arguments are expected to be initialized.
//
// p: will store the first large prime.
// q: will store the second large prime.
// n: will store the product of p and q.
// e: will store the public exponent.
//
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t p_bits = (random() % (((3*nbits)/4)-(nbits/4))) + (nbits/4);
    uint64_t q_bits = nbits - p_bits;
    make_prime(p, p_bits, iters);
    make_prime(q, q_bits, iters);
    mpz_mul(n, p, q);
    mpz_t p_1;
    mpz_t q_1;
    mpz_t p_times_q;
    mpz_t lambda_n;
    mpz_t gcd_out;
    mpz_inits(p_1, q_1, p_times_q, lambda_n, gcd_out, NULL);
    mpz_sub_ui(p_1, p, 1);
    mpz_sub_ui(q_1, q, 1);
    mpz_mul(p_times_q, p_1, q_1);
    gcd(lambda_n, p_1, q_1);
    mpz_fdiv_q(lambda_n, p_times_q, lambda_n);
    while (mpz_cmp_ui(gcd_out, 1)) {
        mpz_urandomb(e, state, nbits);
        gcd(gcd_out, e, lambda_n);
        if (mpz_cmp_ui(gcd_out, 1) == 0) {break;}
    }
    mpz_clears(p_1, q_1, p_times_q, lambda_n, gcd_out, NULL);
}

//
// Writes a public RSA key to a file.
// Public key contents: n, e, signature, username.
// All mpz_t arguments are expected to be initialized.
//
// n: the public modulus.
// e: the public exponent.
// s: the signature of the username.
// username: the username that was signed as s.
//
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
}

//
// Reads a public RSA key from a file.
// Public key contents: n, e, signature, username.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// e: will store the public exponent.
// s: will store the signature.
// username: an allocated array to hold the username.
// pbfile: the file containing the public key
//
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx", n);
    gmp_fscanf(pbfile, "%Zx", e);
    gmp_fscanf(pbfile, "%Zx", s);
    gmp_fscanf(pbfile, "%s", username);
}

//
// Generates the components for a new private RSA key.
// Requires an accompanying RSA public key to complete the pair.
// All mpz_t arguments are expected to be initialized.
//
// d: will store the RSA private key.
// e: the precomputed public exponent.
// p: the first large prime from the public key generation.
// p: the second large prime from the public key generation.
//
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t p_1;
    mpz_t q_1;
    mpz_t p_times_q;
    mpz_t lambda_n;
    mpz_inits(p_1, q_1, p_times_q, lambda_n, NULL);
    mpz_sub_ui(p_1, p, 1);
    mpz_sub_ui(q_1, q, 1);
    mpz_mul(p_times_q, p_1, q_1);
    gcd(lambda_n, p_1, q_1);
    mpz_fdiv_q(lambda_n, p_times_q, lambda_n);
    mod_inverse(d, e, lambda_n);
    mpz_clears(p_1, q_1, p_times_q, lambda_n, NULL);
}

//
// Writes a private RSA key to a file.
// Private key contents: n, d.
// All mpz_t arguments are expected to be initialized.
//
// n: the public modulus.
// d: the private key.
// pvfile: the file to write the private key to.
//
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
}

//
// Reads a private RSA key from a file.
// Private key contents: n, d.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// d: will store the private key.
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx", n);
    gmp_fscanf(pvfile, "%Zx", d);
}

//
// Encrypts a message given an RSA public exponent and modulus.
// All mpz_t arguments are expected to be initialized.
//
// c: will store the encrypted message.
// m: the message to encrypt.
// e: the public exponent.
// n: the public modulus.
//
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e , n);
}

//
// Encrypts an entire file given an RSA public modulus and exponent.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to encrypt.
// outfile: the output file to write the encrypted input to.
// n: the public modulus.
// e: the public exponent.
//
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t k;
    mpz_t m;
    mpz_inits(k, m, NULL);
    mpz_init_set_ui(k, mpz_sizeinbase(n, 2) - 1);
    (void) mpz_fdiv_q_ui(k, k, 8);
    uint64_t k_uint64 = mpz_get_ui(k);
    uint8_t * array = (uint8_t *) malloc(k_uint64);
    array[0] = 0xFF;
    
    while (feof(infile) == 0) {
        uint64_t j = fread(array + 1, 1, k_uint64 - 1, infile);
        mpz_import(m, j+1, 1, 1, 1, 0, array);
        rsa_encrypt(m, m, e, n);
        (void) gmp_fprintf(outfile, "%Zx\n", m);
    }
    free(array);
    mpz_clears(k, m, NULL);
}

//
// Decrypts some ciphertext given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// m: will store the decrypted message.
// c: the ciphertext to decrypt.
// d: the private key.
// n: the public modulus.
//
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d , n);
}

//
// Decrypts an entire file given an RSA public modulus and private key.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to decrypt.
// outfile: the output file to write the decrypted input to.
// n: the public modulus.
// d: the private key.
//
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t k;
    mpz_t m;
    mpz_t c;
    mpz_inits(k, m, c, NULL);
    mpz_init_set_ui(k, mpz_sizeinbase(n, 2) - 1);
    (void) mpz_fdiv_q_ui(k, k, 8);
    uint64_t k_uint64 = mpz_get_ui(k);
    uint8_t * array = (uint8_t *) malloc(k_uint64);
    
    while (feof(infile) == 0) {
        gmp_fscanf(infile, "%Zx\n", c);
        rsa_decrypt(m, c, d, n);
        uint64_t j;
        mpz_export(array, &j, 1, 1, 1, 0, m);
        fwrite(array + 1, 1, j - 1, outfile);
    }
    free(array);
    mpz_clears(k, m, c, NULL);
}

//
// Signs some message given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// s: will store the signed message (the signature).
// m: the message to sign.
// d: the private key.
// n: the public modulus.
//
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d , n);
}

//
// Verifies some signature given an RSA public exponent and modulus.
// Requires the expected message for verification.
// All mpz_t arguments are expected to be initialized.
//
// m: the expected message.
// s: the signature to verify.
// e: the public exponent.
// n: the public modulus.
// returns: true if signature is verified, false otherwise.
//
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m)) {
        mpz_clear(t);
        return false;
    }
    mpz_clear(t);
    return true;
}
