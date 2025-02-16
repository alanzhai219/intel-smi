#include <ze_api.h>

// need to create a function to parse the error code
// ze_get_error_code

#define CHECK_ZE_STATUS(status) \
{ \
  if (status != ZE_RESULT_SUCCESS) { \
    std::cout << "failed\n"; 
    return EXIT_FAILURE; \ 
  } \
}

int main() {
  return EXIT_SUCCESS;
}
