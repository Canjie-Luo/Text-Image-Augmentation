# Scene Text Image Transformer

[![Build Status](https://travis-ci.org/Canjie-Luo/Scene-Text-Image-Transformer.svg?branch=master)](https://travis-ci.org/Canjie-Luo/Scene-Text-Image-Transformer)

A tool for scene text data augmentation. We provide the tool to avoid overfitting and gain robustness of models.

We are now focusing on the shape of the cropped scene text image. The next version for both detection and recognition tasks will be released later. 

## Requirements

- [GCC](https://gcc.gnu.org/gcc-4.8/) 4.8.*
- [Python](https://www.python.org/) 2.7.*
- [Boost](https://www.boost.org/) 1.67
- [OpenCV](https://opencv.org/) 2.4.*

We recommend [Anaconda](https://www.anaconda.com/) to manage the version of your dependencies. For example:

```bash
     conda install boost=1.67.0
```

## Installation
Build library:

```bash
    mkdir build
    cd build
    cmake -D CUDA_USE_STATIC_CUDA_RUNTIME=OFF ..
    make
```

Copy the **Augment.so** to the target folder and follow **demo.py** to use the tool.

```bash
    cp Augment.so ..
    cd ..
    python demo.py
```

## Demo

- Distortion

![](pic/gif_Distort.gif) 

- Stretch

![](pic/gif_Stretch.gif)

- Perspective

![](pic/gif_Perspective.gif)

## Speed

To transform an image with size (H:64, W:200), it takes less than 3ms using a 2.0GHz CPU. It is possible to accelerate the process by calling multi-process batch samplers in an on-the-fly manner, such as setting [**\"num_workers\"**](https://pytorch.org/docs/0.3.1/data.html?highlight=dataset#torch.utils.data.DataLoader) in [PyTorch](https://pytorch.org/docs/0.3.1/data.html?highlight=dataset#torch.utils.data.DataLoader).

## Improvement for Recognition

We compare the accuracies of [CRNN](https://github.com/meijieru/crnn.pytorch) trained using only the corresponding small training set.

| <center>Dataset</center> | <center>IIIT5K</center> | <center>IC13</center> | <center>IC15</center> |
| :---: | :---: | :---: | :---:|
| Without Data Augmentation | <center>40.8%</center> | <center>6.8%</center> | <center>8.7%</center> |
| <center>With Data Augmentation</center> | <center>53.4%</center> | <center>9.6%</center> | <center>24.9%</center> | 


## Citation

```
@inproceedings{schaefer2006image,
  title={Image deformation using moving least squares},
  author={Schaefer, Scott and McPhail, Travis and Warren, Joe},
  booktitle={ACM transactions on graphics (TOG)},
  volume={25},
  number={3},
  pages={533--540},
  year={2006},
  organization={ACM}
}
```

## Acknowledgment
The tool is the combination of [@cxcxcxcx's](https://github.com/cxcxcxcx) [imgwarp-opencv](https://github.com/cxcxcxcx/imgwarp-opencv) and [@Yati Sagade's](https://github.com/yati-sagade) [opencv-ndarray-conversion](https://github.com/yati-sagade/opencv-ndarray-conversion). Thanks for your contribution. 

## Attention
The tool is only free for academic research purposes.
