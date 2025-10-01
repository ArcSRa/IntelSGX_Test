// // enc_bench_1_8G.c
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <openssl/evp.h>
// #include <sys/time.h>
// #include <sys/resource.h>

// #define TOTAL (1800ULL * 1024 * 1024) // ~1.8 Go
// #define CHUNK (64 * 1024 * 1024) // 64 MiB par itération

// double now_sec() {
//     struct timeval tv; gettimeofday(&tv, NULL);
//     return tv.tv_sec + tv.tv_usec / 1e6;
// }

// void report_mem(const char* label) {
//     struct rusage r;
//     getrusage(RUSAGE_SELF, &r);
//     printf("[%s] max RSS = %ld KB\n", label, r.ru_maxrss);
// }

// int main() {
//     unsigned char *buf = malloc(CHUNK);
//     unsigned char *out = malloc(CHUNK + 16);
//     unsigned char key[32] = {0}, iv[12] = {0};
//     EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
//     long iters = TOTAL / CHUNK;

//     double t0 = now_sec(); report_mem("Start");

//     for (long i = 0; i < iters; i++) {
//         memset(buf, i & 0xFF, CHUNK);
//         int len, outl;
//         EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv);
//         EVP_EncryptUpdate(ctx, out, &len, buf, CHUNK);
//         EVP_EncryptFinal_ex(ctx, out + len, &outl);
//     }

//     double t1 = now_sec(); report_mem("After encrypt");
//     printf("Encryption time: %.2f s\n", t1 - t0);

//     EVP_CIPHER_CTX_free(ctx);
//     free(buf); free(out);
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/evp.h>

#define DATA_MB 550
#define BUFSZ (DATA_MB * 1024 * 1024)

int main() {
    unsigned char *buf = malloc(BUFSZ);
    unsigned char *enc = malloc(BUFSZ + 32); // leave space for padding
    unsigned char key[32] = "this_is_a_very_secret_key_for_ae";
    unsigned char iv[16] = "init_vector_1234";
    int enclen = 0, tmplen = 0;

    if (!buf || !enc) {
        printf("malloc failed\n");
        return 1;
    }
    for (size_t i = 0; i < BUFSZ; ++i) buf[i] = (unsigned char)(i & 0xFF);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    EVP_EncryptUpdate(ctx, enc, &enclen, buf, BUFSZ);
    EVP_EncryptFinal_ex(ctx, enc + enclen, &tmplen);
    enclen += tmplen;

    printf("Chiffrement termine (DATA_MB=%d). PID=%d\n", DATA_MB, getpid());
    printf("Pause 30s pour mesures memoire (top/ps/pmap/gramine-metrics)...\n");
    sleep(30);

    EVP_CIPHER_CTX_free(ctx);
    free(buf);
    free(enc);
    return 0;
}
