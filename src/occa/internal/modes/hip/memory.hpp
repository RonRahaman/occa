#ifndef OCCA_INTERNAL_MODES_HIP_MEMORY_HEADER
#define OCCA_INTERNAL_MODES_HIP_MEMORY_HEADER

#include <occa/internal/core/memory.hpp>
#include <occa/internal/modes/hip/polyfill.hpp>

namespace occa {
  namespace hip {
    class device;

    class memory : public occa::modeMemory_t {
      friend class hip::device;

      friend occa::memory wrapMemory(occa::device device,
                                     void *ptr,
                                     const udim_t bytes,
                                     const occa::json &props);

      friend void* getMappedPtr(occa::memory mem);

    public:
      hipDeviceptr_t &hipPtr;
      char *mappedPtr;

      memory(modeDevice_t *modeDevice_,
             udim_t size_,
             const occa::json &properties_ = occa::json());
      ~memory();

      hipStream_t& getHipStream() const;

      void* getKernelArgPtr() const;

      modeMemory_t* addOffset(const dim_t offset);

      void* getPtr(const occa::json &props);

      void copyTo(void *dest,
                  const udim_t bytes,
                  const udim_t destOffset = 0,
                  const occa::json &props = occa::json()) const;

      void copyFrom(const void *src,
                    const udim_t bytes,
                    const udim_t offset = 0,
                    const occa::json &props = occa::json());

      void copyFrom(const modeMemory_t *src,
                    const udim_t bytes,
                    const udim_t destOffset = 0,
                    const udim_t srcOffset = 0,
                    const occa::json &props = occa::json());
      void detach();
    };
  }
}

#endif
