//Test utilisation enclave via sdk sgx normal
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "sgx_urts.h"
#include "Enclave_u.h"

#define ENCLAVE_PATH "enclave.signed.so"
#define MAX_SECRET_LEN 256

int main() {
    sgx_enclave_id_t eid;
    sgx_status_t ret = sgx_create_enclave(ENCLAVE_PATH, SGX_DEBUG_FLAG, NULL, NULL, &eid, NULL);
    if (ret != SGX_SUCCESS) {
        std::cerr << "Failed to create enclave, error: " << std::hex << ret << std::endl;
        return -1;
    }

    char secret[MAX_SECRET_LEN];
    std::cout << "Application started. PID: " << getpid() << std::endl;

    while (true) {
        get_secret(eid, secret, MAX_SECRET_LEN);
//        std::cout << "Secret in enclave memory: " << secret << std::endl;

std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    sgx_destroy_enclave(eid);
    return 0;
}
