#include <iostream>
#include <fstream>
#include <vector>

#include <cstring>

#include <level_zero/ze_api.h>
#include <level_zero/zes_api.h>
#include "ze_utils.hpp"

// need to create a function to parse the error code
// ze_get_error_code

static void ze_check_status(const ze_result_t result, const char* file, const int line) {
    if (result != ZE_RESULT_SUCCESS) {
        printf("L0 error at %s:%d: %s\n", file, line, zeGetReturnString(result).c_str());
        exit(EXIT_FAILURE);
    }
}

#define CHECK_ZE_STATUS(status) (ze_check_status(status, __FILE__, __LINE__))

inline bool smi_argparse(int argc, char* argv[], const char* shortName, const char* longName) {
    char** arg = &argv[1];     // first arg
    char** argE = &argv[argc];  // last arg
    for (; arg != argE; ++arg) {
        if ((0 == strcmp(*arg, shortName)) || (0 == strcmp(*arg, longName))) {
            return true;
        }
    }
    return false;
}

static void smi_usage() {
    std::cout << "intel-smi usage:\n";
    std::cout << "  -n, --new_api   : use new api to init, otherwise will use old api\n";
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

            // std::cout << "Found "<< to_string(type) << " device..." << "\n";
            std::cout << "Driver version: " << driver_properties.driverVersion << "\n";

            ze_api_version_t version = {};
            zeDriverGetApiVersion(pDriver, &version);
            // std::cout << "API version: " << to_string(version) << "\n";

            // std::cout << to_string(device_properties) << "\n";

            ze_device_compute_properties_t compute_properties = {};
            compute_properties.stype = ZE_STRUCTURE_TYPE_DEVICE_COMPUTE_PROPERTIES;
            zeDeviceGetComputeProperties(phDevice, &compute_properties);
            // std::cout << to_string(compute_properties) << "\n";

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
                // std::cout << to_string( pMemoryProperties[ mem ] ) << "\n";
            }
            delete[] pMemoryProperties;

            ze_device_memory_access_properties_t memory_access_properties = {};
            memory_access_properties.stype = ZE_STRUCTURE_TYPE_DEVICE_MEMORY_ACCESS_PROPERTIES;
            zeDeviceGetMemoryAccessProperties(phDevice, &memory_access_properties);
            // std::cout << to_string( memory_access_properties ) << "\n";

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
                // std::cout << to_string( pCacheProperties[ cache ] ) << "\n";
            }
            delete[] pCacheProperties;

            ze_device_image_properties_t image_properties = {};
            image_properties.stype = ZE_STRUCTURE_TYPE_DEVICE_IMAGE_PROPERTIES;
            zeDeviceGetImageProperties(phDevice, &image_properties);
            // std::cout << to_string( image_properties ) << "\n";

            break;
        }
    }

    return found;
}

static void printShortInfo(ze_driver_handle_t driver,
                           ze_device_handle_t device,
                           uint32_t device_id) {
    // driver info
    ze_driver_properties_t drv_props = {};
    drv_props.stype = ZE_STRUCTURE_TYPE_DRIVER_PROPERTIES;
    CHECK_ZE_STATUS(zeDriverGetProperties(driver, &drv_props));
    std::cout << "Driver Vers: " << zeGetDriverVersionString(drv_props.driverVersion) << "\n";

    // device info
    ze_device_properties_t dev_props = {};
    dev_props.stype = ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES;
    CHECK_ZE_STATUS(zeDeviceGetProperties(device, &dev_props));

    std::cout << zeGetDeviceTypeString(dev_props.type) << ": " << device_id << ": " << dev_props.name << "\n";
}

static std::string getProcessName(uint32_t pid) {
    std::string proc_name;
    std::string file_name = "/proc/" + std::to_string(pid) + "/cmdline";
    
    std::ifstream file(file_name);
    if (!file) {
        return std::string();
    }

    std::getline(file, proc_name, '\0');

    return proc_name;
}

static void printProcesses(zes_device_handle_t device) {
    // get state list
    uint32_t proc_cnt = 0;
    CHECK_ZE_STATUS(zesDeviceProcessesGetState(device, &proc_cnt, nullptr));
    if (proc_cnt == 0) {
        return;
    }

    std::vector<zes_process_state_t> dev_state_list(proc_cnt);
    CHECK_ZE_STATUS(zesDeviceProcessesGetState(device, &proc_cnt, dev_state_list.data()));

    std::cout << "Running Processes: " << proc_cnt << "\n";

    // get process name
    for (auto& state : dev_state_list) {
        std::cout << "process name: " << getProcessName(state.processId)  << "\n";
    }
}

int main(int argc, char* argv[]) {
    // process args
    bool use_print_usage = smi_argparse(argc, argv, "-h", "--help");
    bool use_new_api = smi_argparse(argc, argv, "-n", "--new_api");

    // 
    if (use_print_usage) {
        smi_usage();
        exit(EXIT_SUCCESS);
    }

    // step 0: zeInitDrivers
    // drv
    uint32_t drv_cnt = 0;
    std::vector<ze_driver_handle_t> drv_list;

    if (use_new_api) {
        ze_init_driver_type_desc_t driverTypeDesc = {};
        driverTypeDesc.flags = ZE_INIT_DRIVER_TYPE_FLAG_GPU;
        driverTypeDesc.pNext = nullptr;

        CHECK_ZE_STATUS(zeInitDrivers(&drv_cnt, nullptr, &driverTypeDesc));
        drv_list.resize(drv_cnt);
        CHECK_ZE_STATUS(zeInitDrivers(&drv_cnt, drv_list.data(), &driverTypeDesc));

    } else {
        // init
        CHECK_ZE_STATUS(zeInit(ZE_INIT_DRIVER_TYPE_FLAG_GPU));

        CHECK_ZE_STATUS(zeDriverGet(&drv_cnt, nullptr));
        drv_list.resize(drv_cnt);
        CHECK_ZE_STATUS(zeDriverGet(&drv_cnt, drv_list.data()));
    }

    // step 1: get device
    uint32_t dev_cnt = 0;
    std::vector<ze_device_handle_t> dev_list;

    uint32_t dev_id = 0;
    for (auto drv : drv_list) {
        // get dev cnt
        CHECK_ZE_STATUS(zeDeviceGet(drv, &dev_cnt, nullptr));
        // get dev list attached to specific drv
        dev_list.resize(dev_cnt);
        CHECK_ZE_STATUS(zeDeviceGet(drv, &dev_cnt, dev_list.data()));
        // get dev from dev list
        for (auto dev: dev_list) {
            // printShortInfo, drv, dev, dev_id
            printShortInfo(drv, dev, dev_id);
            ++dev_id;
            // printProcess
        }
    }


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


  ze_driver_handle_t pDriver = nullptr;
  ze_device_handle_t pDevice = nullptr;
  for (uint32_t driverIdx = 0; driverIdx < drv_cnt; ++driverIdx) {
    pDriver = drv_list[driverIdx];
  }

  const ze_device_type_t dev_type = ZE_DEVICE_TYPE_GPU; 


  return EXIT_SUCCESS;
}
