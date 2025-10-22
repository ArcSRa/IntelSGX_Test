#include <iostream>
//#include <ostream>
#include <string.h>
#include "Enclave_t.h"

//using namespace std;

static char secret[] = "IMPORTANT_DATA_IN_MEMORY_1234";

void get_secret(char* secret_out, uint32_t max_len) {
    // Copier le secret vers la mï¿½moire passï¿½e par le host
    strncpy(secret_out, secret, max_len);
    secret_out[max_len -1] = '\0';
//   std::cout<<"Adresse du secret : " << static_cast<void*>(secret_out) << std::endl;
// std::cout<<"test"<<std::endl;
}
/*
void store_secret_key (const char *key){
    memcpy(enclave_secret_key, key, 32);
printf("Adresse de key : %p\n", (void*)key);
}
*/
