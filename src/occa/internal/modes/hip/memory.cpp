#include <occa/internal/modes/hip/memory.hpp>
#include <occa/internal/modes/hip/device.hpp>
#include <occa/internal/modes/hip/utils.hpp>

namespace occa {
  namespace hip {
    inline hipDeviceptr_t addHipPtrOffset(hipDeviceptr_t hipPtr, const udim_t offset) {
      return (hipDeviceptr_t) (((char*) hipPtr) + offset);
    }

    memory::memory(modeDevice_t *modeDevice_,
                   udim_t size_,
                   const occa::json &properties_) :
      occa::modeMemory_t(modeDevice_, size_, properties_),
      hipPtr(reinterpret_cast<hipDeviceptr_t&>(ptr)),
      mappedPtr(NULL) {}

    memory::~memory() {
      if (isOrigin) {
        if (mappedPtr) {
          OCCA_HIP_ERROR("Device: mappedFree()",
                         hipHostFree(mappedPtr));
        } else if (hipPtr) {
          hipFree((void*) hipPtr);
        }
      }
      hipPtr = 0;
      mappedPtr = NULL;
      size = 0;
    }

    hipStream_t& memory::getHipStream() const {
      return ((device*) modeDevice)->getHipStream();
    }

    void* memory::getKernelArgPtr() const {
      return (void*) hipPtr;
    }

    modeMemory_t* memory::addOffset(const dim_t offset) {
      memory *m = new memory(modeDevice,
                             size - offset,
                             properties);
      m->hipPtr = addHipPtrOffset(hipPtr, offset);
      if (mappedPtr) {
        m->mappedPtr = mappedPtr + offset;
      }
      return m;
    }

    void* memory::getPtr(const occa::json &props) {
      if (props.get("mapped", false)) {
        return mappedPtr;
      }
      return ptr;
    }

    void memory::copyFrom(const void *src,
                          const udim_t bytes,
                          const udim_t offset,
                          const occa::json &props) {
      const bool async = props.get("async", false);

      if (!async) {
        OCCA_HIP_ERROR("Memory: Copy From",
                       hipMemcpyHtoD(addHipPtrOffset(hipPtr, offset),
                                     const_cast<void*>(src),
                                     bytes) );
      } else {
        OCCA_HIP_ERROR("Memory: Async Copy From",
                       hipMemcpyHtoDAsync(addHipPtrOffset(hipPtr, offset),
                                          const_cast<void*>(src),
                                          bytes,
                                          getHipStream()) );
      }
    }

    void memory::copyFrom(const modeMemory_t *src,
                          const udim_t bytes,
                          const udim_t destOffset,
                          const udim_t srcOffset,
                          const occa::json &props) {
      const bool async = props.get("async", false);

      if (!async) {
        OCCA_HIP_ERROR("Memory: Copy From",
                       hipMemcpyDtoD(addHipPtrOffset(hipPtr, destOffset),
                                     addHipPtrOffset(((memory*) src)->hipPtr, srcOffset),
                                     bytes) );
      } else {
        OCCA_HIP_ERROR("Memory: Async Copy From",
                       hipMemcpyDtoDAsync(addHipPtrOffset(hipPtr, destOffset),
                                          addHipPtrOffset(((memory*) src)->hipPtr, srcOffset),
                                          bytes,
                                          getHipStream()) );
      }
    }

    void memory::copyTo(void *dest,
                        const udim_t bytes,
                        const udim_t offset,
                        const occa::json &props) const {
      const bool async = props.get("async", false);

      if (!async) {
        OCCA_HIP_ERROR("Memory: Copy From",
                       hipMemcpyDtoH(dest,
                                     addHipPtrOffset(hipPtr, offset),
                                     bytes) );
      } else {
        OCCA_HIP_ERROR("Memory: Async Copy From",
                       hipMemcpyDtoHAsync(dest,
                                          addHipPtrOffset(hipPtr, offset),
                                          bytes,
                                          getHipStream()) );
      }
    }

    void memory::detach() {
      hipPtr = 0;
      size = 0;
    }
  }
}
