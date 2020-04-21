# Text Image Augmentation

[![Build Status](https://travis-ci.org/Canjie-Luo/Text-Image-Augmentation.svg?branch=master)](https://travis-ci.org/Canjie-Luo/Text-Image-Augmentation)

A general geometric augmentation tool for text images in the CVPR 2020 paper "[Learn to Augment: Joint Data Augmentation and Network Optimization for Text Recognition](https://arxiv.org/abs/2003.06606)". We provide the tool to avoid overfitting and gain robustness of text recognizers. 

***Note that this is a general toolkit. Please customize for your specific task. If the repo benefits your work, please [cite the papers](https://github.com/Canjie-Luo/Text-Image-Augmentation#citation).***

## News
- 2020-02 The paper "Learn to Augment: Joint Data Augmentation and Network Optimization for Text Recognition" was accepted to ***CVPR 2020***. It is a preliminary attempt for smart augmentation. 

- 2019-11 The paper "Decoupled Attention Network for Text Recognition" ([Paper](https://arxiv.org/abs/1912.10205) [Code](https://github.com/Wang-Tianwei/Decoupled-attention-network)) was accepted to ***AAAI 2020***. This augmentation tool was used in the experiments of handwritten text recognition.

- 2019-04 We applied this tool in the ReCTS competition of ***ICDAR 2019***. Our ensemble model won the championship.

- 2019-01 The similarity transformation was specifically customized for geomeric augmentation of text images. 

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
@inproceedings{luo2020learn,
  author = {Canjie Luo and Yuanzhi Zhu and Lianwen Jin and Yongpan Wang},
  title = {Learn to Augment: Joint Data Augmentation and Network Optimization for Text Recognition},
  booktitle = {CVPR},
  year = {2020}
}

@inproceedings{wang2020decoupled,
  author = {Tianwei Wang and Yuanzhi Zhu and Lianwen Jin and Canjie Luo and Xiaoxue Chen and Yaqiang Wu and Qianying Wang and Mingxiang Cai}, 
  title = {Decoupled attention network for text recognition}, 
  booktitle ={AAAI}, 
  year = {2020}
}

@article{schaefer2006image,
  title={Image deformation using moving least squares},
  author={Schaefer, Scott and McPhail, Travis and Warren, Joe},
  journal={ACM Transactions on Graphics (TOG)},
  volume={25},
  number={3},
  pages={533--540},
  year={2006},
  publisher={ACM New York, NY, USA}
}
```

## Acknowledgment

Thanks for the contribution of the following developers.

[@keeofkoo](https://github.com/keeofkoo)

[@cxcxcxcx](https://github.com/cxcxcxcx)

[@Yati Sagade](https://github.com/yati-sagade) 
## Attention
The tool is only free for academic research purposes.
