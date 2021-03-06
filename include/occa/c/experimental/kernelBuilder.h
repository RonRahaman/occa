#ifndef OCCA_C_EXPERIMENTAL_KERNELBUILDER_HEADER
#define OCCA_C_EXPERIMENTAL_KERNELBUILDER_HEADER

#include <occa/c/defines.h>
#include <occa/c/types.h>
#include <occa/c/experimental/scope.h>

OCCA_START_EXTERN_C

occaKernelBuilder occaKernelBuilderFromInlinedOkl(
  occaScope scope,
  const char *kernelSource,
  const char *kernelName
);

void occaKernelBuilderRun(
  occaKernelBuilder kernelBuilder,
  occaScope scope
);

OCCA_END_EXTERN_C

#endif
