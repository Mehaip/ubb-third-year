#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <string>

namespace CudaImpl
{
    struct ConvolutionData
    {
        int *h_F;
        int *h_C;

        int *d_F;
        int *d_C;

        int n, m, k;
    };

    ConvolutionData readInput(const std::string &filename);
    void writeOutput(const std::string &filename, const ConvolutionData &data);

    // Cleanup function
    void freeData(ConvolutionData &data);

    void applyConvolutionInPlace(ConvolutionData &data);
    void applyConvolutionInPlaceParallel(ConvolutionData &data, int numBlocks);
}

#endif