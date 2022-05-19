// fillNoData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <gdal.h>
#include <gdal_priv.h>
#include <cpl_conv.h>
#include <gdal_alg.h>
#include <ctime>
#include <cstdio>

int main(int argc, char* argv[])
{
    // Check the number of parameters
    if (argc < 3) {
        // Tell the user how to run the program
        std::cerr << "Usage: fillNoData tiffilename.tif\n"
            << "Dataset should be path to dataset *.tif file\n"
            << "Enter number between 100-500 for Max Search Radius.  Example: 250\n"
            << "Example usage: fillNoData.exe D:/path/to/myfile.tif 250\n"
            << std::endl;
        return 1;
    }

    std::clock_t start;
    int duration;

    start = std::clock();

    GDALAllRegister();
    GDALDataset* pSrcDataset;
    GDALRasterBand* poBand;

    const char* inRaster = argv[1];
    const char* mSearch = argv[2];

    int maskFlags;
    int noData;
    int maxSearch;
    int maxInt = 3;
    int nBlockXSize, nBlockYSize;
    double adfGeoTransform[6];
    maxSearch = atoi(mSearch);

    pSrcDataset = (GDALDataset*)GDALOpen(inRaster, GA_Update);
    CPLAssert(pSrcDataset != NULL);

    poBand = pSrcDataset->GetRasterBand(1);
    poBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
    printf("Block=%dx%d Type=%s\n",
        nBlockXSize, nBlockYSize,
        GDALGetDataTypeName(poBand->GetRasterDataType()));

    noData = pSrcDataset->GetRasterBand(1)->GetNoDataValue();
    printf("No Data Value = %i\n", noData);

    printf("Driver: %s/%s\n",
        pSrcDataset->GetDriver()->GetDescription(),
        pSrcDataset->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));

    printf("Size is %dx%dx%d\n",
        pSrcDataset->GetRasterXSize(), pSrcDataset->GetRasterYSize(),
        pSrcDataset->GetRasterCount());

    if (pSrcDataset->GetProjectionRef() != NULL)
        printf("Projection is '%s'\n", pSrcDataset->GetProjectionRef());

    if (pSrcDataset->GetGeoTransform(adfGeoTransform) == CE_None)
        printf("Origin = (%.6f, %.6f)\n", adfGeoTransform[0], adfGeoTransform[3]);
    printf("Pixel Size = (%.6f, %.6f)\n", adfGeoTransform[1], adfGeoTransform[5]);


    maskFlags = pSrcDataset->GetRasterBand(1)->GetMaskFlags();
    printf("Mask Flags = %i\n", maskFlags);

    printf("Processing image\n");

    GDALFillNodata(poBand, pSrcDataset->GetRasterBand(1)->GetMaskBand(), maxSearch, 0, maxInt, NULL, NULL, NULL);

    GDALClose(pSrcDataset);

    duration = (std::clock() - start) / (int)CLOCKS_PER_SEC;
    printf("\nProcessing time is: %d seconds\n", duration);

    return 0;

}


