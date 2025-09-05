// enc_bench_1_8G.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <sys/time.h>
#include <sys/resource.h>

#define TOTAL (1800ULL * 1024 * 1024) // ~1.8 Go
#define CHUNK (64 * 1024 * 1024) // 64 MiB par itération

double now_sec() {
    struct timeval tv; gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

void report_mem(const char* label) {
    struct rusage r;
    getrusage(RUSAGE_SELF, &r);
    printf("[%s] max RSS = %ld KB\n", label, r.ru_maxrss);
}

int main() {
    unsigned char *buf = malloc(CHUNK);
    unsigned char *out = malloc(CHUNK + 16);
    unsigned char key[32] = {0}, iv[12] = {0};
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    long iters = TOTAL / CHUNK;

    double t0 = now_sec(); report_mem("Start");

    for (long i = 0; i < iters; i++) {
        memset(buf, i & 0xFF, CHUNK);
        int len, outl;
        EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv);
        EVP_EncryptUpdate(ctx, out, &len, buf, CHUNK);
        EVP_EncryptFinal_ex(ctx, out + len, &outl);
    }

    double t1 = now_sec(); report_mem("After encrypt");
    printf("Encryption time: %.2f s\n", t1 - t0);

    EVP_CIPHER_CTX_free(ctx);
    free(buf); free(out);
    return 0;
}
