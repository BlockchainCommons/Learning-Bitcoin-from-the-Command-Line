#include <jansson.h>
#include <bitcoinrpc.h>

int main(void) {
  
  bitcoinrpc_global_init();

  bitcoinrpc_cl_t *rpc_client;
  rpc_client = bitcoinrpc_cl_init_params ("bitcoinrpc", "d8340efbcd34e312044c8431c59c792c", "127.0.0.1", 18332);

  if (rpc_client) {

    printf("Successfully connected to server!\n");
  
  } else {

    printf("Failed to connect to server!\n");

  }

  bitcoinrpc_global_cleanup();

}
