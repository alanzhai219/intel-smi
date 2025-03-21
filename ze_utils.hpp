#include <string>
#include <level_zero/ze_api.h>
#include <level_zero/zes_api.h>

// api wrapper for level-zero.
// ze_get_error_code is a overload funtion.
// It is used to parse the return code for log and debug.
// current support below return code:
// - ze_result_t
std::string zeGetReturnString(const ze_result_t result) {
    switch (result) {
        case ZE_RESULT_SUCCESS:
            return "ZE_RESULT_SUCCESS";
        case ZE_RESULT_NOT_READY:
            return "ZE_RESULT_NOT_READY";
        case ZE_RESULT_ERROR_UNINITIALIZED:
            return "ZE_RESULT_ERROR_UNINITIALIZED";
        case ZE_RESULT_ERROR_DEVICE_LOST:
            return "ZE_RESULT_ERROR_DEVICE_LOST";
        case ZE_RESULT_ERROR_INVALID_ARGUMENT:
            return "ZE_RESULT_ERROR_INVALID_ARGUMENT";
        case ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY:
            return "ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY";
        case ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY:
            return "ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY";
        case ZE_RESULT_ERROR_MODULE_BUILD_FAILURE:
            return "ZE_RESULT_ERROR_MODULE_BUILD_FAILURE";
        case ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS:
            return "ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS";
        case ZE_RESULT_ERROR_NOT_AVAILABLE:
            return "ZE_RESULT_ERROR_NOT_AVAILABLE";
        case ZE_RESULT_ERROR_UNSUPPORTED_VERSION:
            return "ZE_RESULT_ERROR_UNSUPPORTED_VERSION";
        case ZE_RESULT_ERROR_UNSUPPORTED_FEATURE:
            return "ZE_RESULT_ERROR_UNSUPPORTED_FEATURE";
        case ZE_RESULT_ERROR_INVALID_NULL_HANDLE:
            return "ZE_RESULT_ERROR_INVALID_NULL_HANDLE";
        case ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE:
            return "ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE";
        case ZE_RESULT_ERROR_INVALID_NULL_POINTER:
            return "ZE_RESULT_ERROR_INVALID_NULL_POINTER";
        case ZE_RESULT_ERROR_INVALID_SIZE:
            return "ZE_RESULT_ERROR_INVALID_SIZE";
        case ZE_RESULT_ERROR_UNSUPPORTED_SIZE:
            return "ZE_RESULT_ERROR_UNSUPPORTED_SIZE";
        case ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT:
            return "ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT";
        case ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT:
            return "ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT";
        case ZE_RESULT_ERROR_INVALID_ENUMERATION:
            return "ZE_RESULT_ERROR_INVALID_ENUMERATION";
        case ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION:
            return "ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION";
        case ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT:
            return "ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT";
        case ZE_RESULT_ERROR_INVALID_NATIVE_BINARY:
            return "ZE_RESULT_ERROR_INVALID_NATIVE_BINARY";
        case ZE_RESULT_ERROR_INVALID_GLOBAL_NAME:
            return "ZE_RESULT_ERROR_INVALID_GLOBAL_NAME";
        case ZE_RESULT_ERROR_INVALID_KERNEL_NAME:
            return "ZE_RESULT_ERROR_INVALID_KERNEL_NAME";
        case ZE_RESULT_ERROR_INVALID_FUNCTION_NAME:
            return "ZE_RESULT_ERROR_INVALID_FUNCTION_NAME";
        case ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION:
            return "ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION";
        case ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION:
            return "ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION";
        case ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX:
            return "ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX";
        case ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE:
            return "ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE";
        case ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE:
            return "ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE";
        case ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE:
            return "ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE";
        case ZE_RESULT_ERROR_OVERLAPPING_REGIONS:
            return "ZE_RESULT_ERROR_OVERLAPPING_REGIONS";
        case ZE_RESULT_ERROR_UNKNOWN:
            return "ZE_RESULT_ERROR_UNKNOWN";
        default:
            return "Unknown ze_result_t value: " + std::to_string(static_cast<int>(result));
    }
}

std::string zeGetDeviceTypeString(ze_device_type_t type) {
    switch (type) {
        case ZE_DEVICE_TYPE_GPU:
            return "ZE_DEVICE_TYPE_GPU";
        case ZE_DEVICE_TYPE_CPU:
            return "ZE_DEVICE_TYPE_CPU";
        case ZE_DEVICE_TYPE_FPGA:
            return "ZE_DEVICE_TYPE_FPGA";
        case ZE_DEVICE_TYPE_MCA:
            return "ZE_DEVICE_TYPE_MCA";
        case ZE_DEVICE_TYPE_VPU:
            return "ZE_DEVICE_TYPE_VPU";
        default:
            return "Unknown ze_device_type_t value: " + std::to_string(static_cast<int>(type));
    }
}

/*
  uint32_t [31 ------ 23][22 ------ 15][14 ------ 0]
  uint32_t [    major   ][    minor   ][    rev    ]   
*/
std::string zeGetDriverVersionString(uint32_t driver_version) {
    uint32_t major = driver_version >> 24;
    uint32_t minor = (driver_version >> 16) & 0xFF;
    uint32_t rev = driver_version & 0xFFFF;
    std::string version = std::to_string(major) + "." +
                          std::to_string(minor) + "." +
                          std::to_string(rev);
    return version;
}

std::string zesGetEngineString(zes_engine_type_flags_t eng) {
    switch(eng) {
        case ZES_ENGINE_TYPE_FLAG_OTHER:
            return "ZES_ENGINE_TYPE_FLAG_OTHER";
        case ZES_ENGINE_TYPE_FLAG_COMPUTE:
            return "ZES_ENGINE_TYPE_FLAG_COMPUTE";
        case ZES_ENGINE_TYPE_FLAG_3D:
            return "ZES_ENGINE_TYPE_FLAG_3D";
        case ZES_ENGINE_TYPE_FLAG_MEDIA:
            return "ZES_ENGINE_TYPE_FLAG_MEDIA";
        case ZES_ENGINE_TYPE_FLAG_DMA:
            return "ZES_ENGINE_TYPE_FLAG_DMA";
        case ZES_ENGINE_TYPE_FLAG_RENDER:
            return "ZES_ENGINE_TYPE_FLAG_RENDER";
        default:
            return "Unknown zes_engine_type_flags_t value: " + std::to_string(static_cast<uint32_t>(eng));
    }
}
