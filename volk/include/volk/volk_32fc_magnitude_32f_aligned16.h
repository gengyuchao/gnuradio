#ifndef INCLUDED_VOLK_32fc_MAGNITUDE_32f_ALIGNED16_H
#define INCLUDED_VOLK_32fc_MAGNITUDE_32f_ALIGNED16_H

#include <inttypes.h>
#include <stdio.h>
#include <math.h>

#if LV_HAVE_SSE3
#include <pmmintrin.h>
  /*!
    \brief Calculates the magnitude of the complexVector and stores the results in the magnitudeVector
    \param complexVector The vector containing the complex input values
    \param magnitudeVector The vector containing the real output values
    \param num_points The number of complex values in complexVector to be calculated and stored into cVector
  */
static inline void volk_32fc_magnitude_32f_aligned16_sse3(float* magnitudeVector, const lv_32fc_t* complexVector, unsigned int num_points){
    unsigned int number = 0;
    const unsigned int quarterPoints = num_points / 4;

    const float* complexVectorPtr = (float*)complexVector;
    float* magnitudeVectorPtr = magnitudeVector;

    __m128 cplxValue1, cplxValue2, result;
    for(;number < quarterPoints; number++){
      cplxValue1 = _mm_load_ps(complexVectorPtr);
      complexVectorPtr += 4;

      cplxValue2 = _mm_load_ps(complexVectorPtr);
      complexVectorPtr += 4;

      cplxValue1 = _mm_mul_ps(cplxValue1, cplxValue1); // Square the values
      cplxValue2 = _mm_mul_ps(cplxValue2, cplxValue2); // Square the Values

      result = _mm_hadd_ps(cplxValue1, cplxValue2); // Add the I2 and Q2 values

      result = _mm_sqrt_ps(result);

      _mm_store_ps(magnitudeVectorPtr, result);
      magnitudeVectorPtr += 4;
    }

    number = quarterPoints * 4;
    for(; number < num_points; number++){
      float val1Real = *complexVectorPtr++;
      float val1Imag = *complexVectorPtr++;
      *magnitudeVectorPtr++ = sqrtf((val1Real * val1Real) + (val1Imag * val1Imag));
    }
}
#endif /* LV_HAVE_SSE3 */

#if LV_HAVE_SSE
#include <xmmintrin.h>
  /*!
    \brief Calculates the magnitude of the complexVector and stores the results in the magnitudeVector
    \param complexVector The vector containing the complex input values
    \param magnitudeVector The vector containing the real output values
    \param num_points The number of complex values in complexVector to be calculated and stored into cVector
  */
static inline void volk_32fc_magnitude_32f_aligned16_sse(float* magnitudeVector, const lv_32fc_t* complexVector, unsigned int num_points){
    unsigned int number = 0;
    const unsigned int quarterPoints = num_points / 4;
    
    const float* complexVectorPtr = (float*)complexVector;
    float* magnitudeVectorPtr = magnitudeVector;

    __m128 cplxValue1, cplxValue2, iValue, qValue, result;
    for(;number < quarterPoints; number++){
      cplxValue1 = _mm_load_ps(complexVectorPtr);
      complexVectorPtr += 4;

      cplxValue2 = _mm_load_ps(complexVectorPtr);
      complexVectorPtr += 4;

      // Arrange in i1i2i3i4 format
      iValue = _mm_shuffle_ps(cplxValue1, cplxValue2, _MM_SHUFFLE(2,0,2,0));
      // Arrange in q1q2q3q4 format
      qValue = _mm_shuffle_ps(cplxValue1, cplxValue2, _MM_SHUFFLE(3,1,3,1));

      iValue = _mm_mul_ps(iValue, iValue); // Square the I values
      qValue = _mm_mul_ps(qValue, qValue); // Square the Q Values

      result = _mm_add_ps(iValue, qValue); // Add the I2 and Q2 values

      result = _mm_sqrt_ps(result);

      _mm_store_ps(magnitudeVectorPtr, result);
      magnitudeVectorPtr += 4;
    }

    number = quarterPoints * 4;
    for(; number < num_points; number++){
       float val1Real = *complexVectorPtr++;
       float val1Imag = *complexVectorPtr++;
      *magnitudeVectorPtr++ = sqrtf((val1Real * val1Real) + (val1Imag * val1Imag));
    }
}
#endif /* LV_HAVE_SSE */

#if LV_HAVE_GENERIC
  /*!
    \brief Calculates the magnitude of the complexVector and stores the results in the magnitudeVector
    \param complexVector The vector containing the complex input values
    \param magnitudeVector The vector containing the real output values
    \param num_points The number of complex values in complexVector to be calculated and stored into cVector
  */
static inline void volk_32fc_magnitude_32f_aligned16_generic(float* magnitudeVector, const lv_32fc_t* complexVector, unsigned int num_points){
  const float* complexVectorPtr = (float*)complexVector;
  float* magnitudeVectorPtr = magnitudeVector;
  unsigned int number = 0;
  for(number = 0; number < num_points; number++){
    const float real = *complexVectorPtr++;
    const float imag = *complexVectorPtr++;
    *magnitudeVectorPtr++ = sqrtf((real*real) + (imag*imag));
  }
}
#endif /* LV_HAVE_GENERIC */




#endif /* INCLUDED_VOLK_32fc_MAGNITUDE_32f_ALIGNED16_H */
