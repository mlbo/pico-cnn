# Changelog 
## Version 1.0

 tag `v1.0`

 * Restructured C code (split in .c and .h)
 * Import of ONNX models and generation of C code
 * Tested neural networks:
    * LeNet
    * MNIST Multi-Layer-Perceptron (MLP)
    * MNIST Perceptron
    * AlexNet
    * VGG-16
    * VGG-19
    * MobileNet-V2
    * Inception-V3
    * Inception-Resnet-V2
    * TC-ResNet-8
 * Removed experimental hand-crafted ARM-NEON Code
 * Removed experimental OpenMP support

## Version 0.1 

 tag `v0.1`

 * Tested neural networks:
    * LeNet-5
    * AlexNet
    * VGG-16
 * Experimental support of ARM-NEON SIMD instructions for several layers
 * Experimental support of OpenMP on layer level