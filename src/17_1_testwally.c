#include <stdio.h>
#include "wally_core.h"

int main(void) {

  int lw_response;
  
  lw_response = wally_init(0);
  printf("Startup: %d\n",lw_response);

  wally_cleanup(0);
  
}
