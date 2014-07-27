#include <iostream>

#include "occa.hpp"


int main(int argc, char **argv){

  int entries = 5;

  float *a  = new float[entries];
  float *b  = new float[entries];
  float *ab = new float[entries];

  for(int i = 0; i < entries; ++i){
    a[i]  = (float) i;
    b[i]  = (float) (1 - i);
    ab[i] = 0;
  }

  // occa::availableDevices<occa::OpenCL>();
  
  std::string mode = "OpenMP";
  int platformID = 0;
  int deviceID   = 0;
  
  occa::device device;
  occa::kernel addVectors;
  occa::memory o_a, o_b, o_ab;

  device.setup(mode, platformID, deviceID);
  
  o_a  = device.malloc(entries*sizeof(float));
  o_b  = device.malloc(entries*sizeof(float));
  o_ab = device.malloc(entries*sizeof(float));

  std::cout << "REM: faked filepath of addVectors.occa";
  //addVectors = device.buildKernelFromSource("addVectors.occa",
  //                                          "addVectors");
  addVectors = device.buildKernelFromSource("C:\\Users\\florian\\Documents\\OCCA2\\examples\\addVectors\\addVectors.occa",
                                            "addVectors");

  int dims = 1;
  int itemsPerGroup(2);
  int groups((entries + itemsPerGroup - 1)/itemsPerGroup);

  addVectors.setWorkingDims(dims, itemsPerGroup, groups);

  o_a.copyFrom(a);
  o_b.copyFrom(b);

  addVectors(entries, o_a, o_b, o_ab);

  o_ab.copyTo(ab);

  for(int i = 0; i < 5; ++i)
    std::cout << i << ": " << ab[i] << '\n';

  delete [] a;
  delete [] b;
  delete [] ab;

  addVectors.free();
  o_a.free();
  o_b.free();
  o_ab.free();
  device.free();
  
}
