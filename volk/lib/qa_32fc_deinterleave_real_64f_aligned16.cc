#include <volk/volk.h>
#include <qa_32fc_deinterleave_real_64f_aligned16.h>
#include <volk/volk_32fc_deinterleave_real_64f_aligned16.h>
#include <cstdlib>

//test for sse

#ifndef LV_HAVE_SSE2

void qa_32fc_deinterleave_real_64f_aligned16::t1() {
  printf("sse2 not available... no test performed\n");
}

#else

void qa_32fc_deinterleave_real_64f_aligned16::t1() {
  
  volk_environment_init();
  clock_t start, end;
  double total;
  const int vlen = 3201;
  const int ITERS = 100000;
  std::complex<float> input0[vlen] __attribute__ ((aligned (16)));
  
  double output_generic[vlen] __attribute__ ((aligned (16)));
  double output_sse2[vlen] __attribute__ ((aligned (16)));

  float* inputLoad = (float*)input0;
  for(int i = 0; i < 2*vlen; ++i) {   
    inputLoad[i] = (((float) (rand() - (RAND_MAX/2))) / static_cast<float>((RAND_MAX/2)));
  }
  printf("32fc_deinterleave_real_64f_aligned\n");

  start = clock();
  for(int count = 0; count < ITERS; ++count) {
    volk_32fc_deinterleave_real_64f_aligned16_manual(output_generic, input0, vlen, "generic");
  }
  end = clock();
  total = (double)(end-start)/(double)CLOCKS_PER_SEC;
  printf("generic_time: %f\n", total);
  start = clock();
  for(int count = 0; count < ITERS; ++count) {
    volk_32fc_deinterleave_real_64f_aligned16_manual(output_sse2, input0, vlen, "sse2");
  }
  end = clock();
  total = (double)(end-start)/(double)CLOCKS_PER_SEC;
  printf("sse_time: %f\n", total);

  for(int i = 0; i < 1; ++i) {
    //printf("inputs: %d, %d\n", input0[i*2], input0[i*2 + 1]);
    //printf("generic... %d, ssse3... %d\n", output0[i], output1[i]);
  }
  
  for(int i = 0; i < vlen; ++i) {
    //printf("%d...%d\n", output0[i], output01[i]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(output_generic[i], output_sse2[i], fabs(output_generic[i])*1e-4);
  }
}

#endif
