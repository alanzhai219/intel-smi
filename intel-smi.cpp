#include <ze_api.h>

#include "ze_utils.hpp"

// need to create a function to parse the error code
// ze_get_error_code

#define CHECK_ZE_STATUS(status) \
{ \
  if (status != ZE_RESULT_SUCCESS) { \
    std::cout << ze_get_error_code(status) << "\n"; \ 
    return EXIT_FAILURE; \ 
  } \
}

inline ze_device_handle_t findDevice(ze_driver_handle_t pDriver,
                                     ze_device_type_t type){
    // get all devices
    uint32_t deviceCount = 0;
    zeDeviceGet(pDriver, &deviceCount, nullptr);

    std::vector<ze_device_handle_t> devices(deviceCount);
    zeDeviceGet(pDriver, &deviceCount, devices.data());

    ze_device_handle_t found = nullptr;

    // for each device, find the first one matching the type
    for(uint32_t device = 0; device < deviceCount; ++device) {
        auto phDevice = devices[device];

        ze_device_properties_t device_properties = {};
        device_properties.stype = ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES;
        zeDeviceGetProperties(phDevice, &device_properties);

        if(type == device_properties.type) {
            found = phDevice;

            ze_driver_properties_t driver_properties = {};
            driver_properties.stype = ZE_STRUCTURE_TYPE_DRIVER_PROPERTIES;
            zeDriverGetProperties(pDriver, &driver_properties);

            std::cout << "Found "<< to_string(type) << " device..." << "\n";
            std::cout << "Driver version: " << driver_properties.driverVersion << "\n";

            ze_api_version_t version = {};
            zeDriverGetApiVersion(pDriver, &version);
            std::cout << "API version: " << to_string(version) << "\n";

            std::cout << to_string(device_properties) << "\n";

            ze_device_compute_properties_t compute_properties = {};
            compute_properties.stype = ZE_STRUCTURE_TYPE_DEVICE_COMPUTE_PROPERTIES;
            zeDeviceGetComputeProperties(phDevice, &compute_properties);
            std::cout << to_string(compute_properties) << "\n";

            uint32_t memoryCount = 0;
            zeDeviceGetMemoryProperties(phDevice, &memoryCount, nullptr);
            auto pMemoryProperties = new ze_device_memory_properties_t[memoryCount];
            for( uint32_t mem = 0; mem < memoryCount; ++mem )
            {
                pMemoryProperties[mem].stype = ZE_STRUCTURE_TYPE_DEVICE_MEMORY_PROPERTIES;
                pMemoryProperties[mem].pNext = nullptr;
            }
            zeDeviceGetMemoryProperties(phDevice, &memoryCount, pMemoryProperties);
            for( uint32_t mem = 0; mem < memoryCount; ++mem )
            {
                std::cout << to_string( pMemoryProperties[ mem ] ) << "\n";
            }
            delete[] pMemoryProperties;

            ze_device_memory_access_properties_t memory_access_properties = {};
            memory_access_properties.stype = ZE_STRUCTURE_TYPE_DEVICE_MEMORY_ACCESS_PROPERTIES;
            zeDeviceGetMemoryAccessProperties(phDevice, &memory_access_properties);
            std::cout << to_string( memory_access_properties ) << "\n";

            uint32_t cacheCount = 0;
            zeDeviceGetCacheProperties(phDevice, &cacheCount, nullptr );
            auto pCacheProperties = new ze_device_cache_properties_t[cacheCount];
            for( uint32_t cache = 0; cache < cacheCount; ++cache )
            {
                pCacheProperties[cache].stype = ZE_STRUCTURE_TYPE_DEVICE_CACHE_PROPERTIES;
                pCacheProperties[cache].pNext = nullptr;
            }
            zeDeviceGetCacheProperties(phDevice, &cacheCount, pCacheProperties);
            for( uint32_t cache = 0; cache < cacheCount; ++cache )
            {
                std::cout << to_string( pCacheProperties[ cache ] ) << "\n";
            }
            delete[] pCacheProperties;

            ze_device_image_properties_t image_properties = {};
            image_properties.stype = ZE_STRUCTURE_TYPE_DEVICE_IMAGE_PROPERTIES;
            zeDeviceGetImageProperties(phDevice, &image_properties);
            std::cout << to_string( image_properties ) << "\n";

            break;
        }
    }

    return found;
}

int main() {
  // step 0: zeInitDrivers
  uint32_t driverCount = 0;

  ze_init_driver_type_desc_t driverTypeDesc = {ZE_STRUCTURE_TYPE_INIT_DRIVER_TYPE_DESC};
  driverTypeDesc.flags = ZE_INIT_DRIVER_TYPE_FLAG_GPU;
  driverTypeDesc.pNext = nullptr;

  CHECK_ZE_STATUS(zeInitDrivers(&driverCount, nullptr, &driverTypeDesc));

  // : get loader versions
#ifdef INTEL_SIM_DEBUG
  zel_component_version_t *versions;
  size_t size = 0;
  zelLoaderGetVersions(&size, nullptr);
  std::cout << "zelLoaderGetVersions number of components found: " << size << std::endl;
  versions = new zel_component_version_t[size];
  zelLoaderGetVersions(&size, versions);

  for(size_t i = 0; i < size; i++){
      std::cout << "Version " << i << std::endl;
      std::cout << "Name: " << versions[i].component_name << std::endl;
      std::cout << "Major: " << versions[i].component_lib_version.major << std::endl;
      std::cout << "Minor: " << versions[i].component_lib_version.minor << std::endl;
      std::cout << "Patch: " << versions[i].component_lib_version.patch << std::endl;
  }

  delete[] versions;
#endif

  std::vector<ze_driver_handle_t> drivers(driverCount);
  CHECK_ZE_STATUS(zeInitDrivers(&driverCount, drivers.data(), &driverTypeDesc));

  ze_driver_handle_t pDriver = nullptr;
  ze_device_handle_t pDevice = nullptr;
  for (uint32_t driverIdx = 0; driverIdx < driverCount; ++driverIdx) {
    pDriver = drivers[driverIdx];

  }

  const ze_device_type_t dev_type = ZE_DEVICE_TYPE_GPU; 

  ze_driver_handle_t pDriver = nullptr;


  return EXIT_SUCCESS;
}
