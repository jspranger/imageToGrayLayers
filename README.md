# ImageToGrayLayers

## Description

This is a command line application that takes an image as input and generates layers that can be used to perform multiple passes by low power laser engravers, provided that they are unable to change the laser power during the same pass. This process should create more precise shading effects, hence turning the output more realistic.

The project depends on the OpenCV library in order to perform the required image processing.

An optional histogram equalization of pixel intensity values may be performed prior to extracting the regions. The regions are then extracted using k-means clustering with a user-specified number of bins.

The layers may be output as masks or normalized intensity values, and may contain isolated regions, or accumulate regions from previously exported layers.

Routing options are not specified here. Should you intend to use line to line tracing, or dithering, or any other method, it must be specified in the CAM application itself.

#### Important notice:

This project is a work in progress, and the results are highly dependent on laser power configurations, engraving speed, and other factors. Further tests must be executed in order to validate the approach.

If it interestes you, feel free to contribute to the project!
All help is appreciated.

## Command line parameters

### `-i <image file>`
Required parameter in which the input image file must be provided.

### `-l <number of layers>`
Optional parameter to specify the number of layers to export.
Value may range between 2 and 256.
The higher the amount of layers, the smoother the edges - to each layer an engraving pass must be performed.

Defaults to 8.

### `-o <output folder path>`
Optional parameter to specify the folder into which the layers will be output.

Defaults to the current directory in which the application is invoked.

### `-e`
Optional parameter to improve image contrast prior to generating the gray layers.
It tells the application to perform histogram equalization of the pixel intensity values for the input image.

Not enabled by default.

### `-g`
Optional parameter that requests each layer to be output containing the pixel intensity variations instead of the region mask, and may be used to generate dithering. The pixel values are normalized between 0 - 255.

Not enabled by default.

### `-a`
Optional parameter that allows each layer to be exported containing the region calculated for it, as well as the regions of previous layers. The order of layer exporting is reversed in order to allow the same laser power to be used for all passes.

This method should provide smoother edges between regions while engraving.

Not enabled by default.

### `-h [--help]`
Optional parameter to display the help menu and exit.

## Future work

- Provide samples of engraved images and settings
- Generate the G-code for the multiple passes
- Setup CI pipelines and automated testing