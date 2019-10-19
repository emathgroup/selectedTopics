---
title: 从编译OpenCV3 学到的东西
categories: 音视频
keywords: 'OpenCV,FFmpeg,CMake'
thumbnail: "../thumbnails/writing.png"

tags:
  - OpenCV
  - CMake
  - OpenGL
abbrlink: 20051
date: 2019-10-16 23:51:34
---
哦哦，冷冽的风，谁在唱着离歌，黑夜沉沉。
This line is added by KeyTo9_Fans.

之前主要是懒，一直都在用[OpenCV] 2.4.9，是安装Kurento media server的时候被动的安装上的。今天尝试源码编译安装了[OpenCV]3.3,想体验一把新功能。
<!-- more --> 
# 附加模块包

[OpenCV contrib]这个必须要有的。官网提供了[教程](http://docs.opencv.org/master/d3/d81/tutorial_contrib_root.html)
```
aruco        contrib_world       dpm       img_hash          plot       sfm               tracking
bgsegm       cvv                 face      line_descriptor   README.md  stereo            xfeatures2d
bioinspired  datasets            freetype  matlab            reg        structured_light  ximgproc
ccalib       dnn_modern          fuzzy     optflow           rgbd       surface_matching  xobjdetect
cnn_3dobj    dnns_easily_fooled  hdf       phase_unwrapping  saliency   text              xphoto

```
# 视频IO
OpenCV的官方文档讲到了关于OpenCV的视频处理背后的支撑框架
[Video I/O with OpenCV Overview].
```
CAP_ANY 	
Auto detect == 0.
CAP_VFW 	
Video For Windows (platform native)
CAP_V4L 	
V4L/V4L2 capturing support via libv4l.
CAP_V4L2 	
Same as CAP_V4L.
CAP_FIREWIRE 	
IEEE 1394 drivers.
CAP_FIREWARE 	
Same as CAP_FIREWIRE.
CAP_IEEE1394 	
Same as CAP_FIREWIRE.
CAP_DC1394 	
Same as CAP_FIREWIRE.
CAP_CMU1394 	
Same as CAP_FIREWIRE.
CAP_QT 	
QuickTime.
CAP_UNICAP 	
Unicap drivers.
CAP_DSHOW 	
DirectShow (via videoInput)
CAP_PVAPI 	
PvAPI, Prosilica GigE SDK.
CAP_OPENNI 	
OpenNI (for Kinect)
CAP_OPENNI_ASUS 	
OpenNI (for Asus Xtion)
CAP_ANDROID 	
Android - not used.
CAP_XIAPI 	
XIMEA Camera API.
CAP_AVFOUNDATION 	
AVFoundation framework for iOS (OS X Lion will have the same API)
CAP_GIGANETIX 	
Smartek Giganetix GigEVisionSDK.
CAP_MSMF 	
Microsoft Media Foundation (via videoInput)
CAP_WINRT 	
Microsoft Windows Runtime using Media Foundation.
CAP_INTELPERC 	
Intel Perceptual Computing SDK.
CAP_OPENNI2 	
OpenNI2 (for Kinect)
CAP_OPENNI2_ASUS 	
OpenNI2 (for Asus Xtion and Occipital Structure sensors)
CAP_GPHOTO2 	
gPhoto2 connection
CAP_GSTREAMER 	
GStreamer.
CAP_FFMPEG 	
Open and record video file or stream using the FFMPEG library.
CAP_IMAGES 	
OpenCV Image Sequence (e.g. img_%02d.jpg)
CAP_ARAVIS 	
Aravis SDK.
CAP_OPENCV_MJPEG 	
Built-in OpenCV MotionJPEG codec.
CAP_INTEL_MFX 	
Intel MediaSDK.
```
WITH_FFMPEG默认是OFF的，要想开启[FFMPEG]的支持，还需要确保[FFMPEG]是动态库版本, **--enable-shared --disable-static**, 我更喜欢 [GStreamer],WITH_GSTREAMER 默认就是ON的

```
--   Video I/O:
--     DC1394 1.x:                  NO
--     DC1394 2.x:                  YES (ver 2.2.4)
--     FFMPEG:                      YES
--       avcodec:                   YES (ver 57.105.100)
--       avformat:                  YES (ver 57.82.100)
--       avutil:                    YES (ver 55.74.100)
--       swscale:                   YES (ver 4.7.103)
--       avresample:                YES (ver 2.1.0)
--     GStreamer:                   NO
--     OpenNI:                      NO
--     OpenNI PrimeSensor Modules:  NO
--     OpenNI2:                     NO
--     PvAPI:                       NO
--     GigEVisionSDK:               NO
--     Aravis SDK:                  NO
--     UniCap:                      NO
--     UniCap ucil:                 NO
--     V4L/V4L2:                    Using libv4l1 (ver 1.10.0) / libv4l2 (ver 1.10.0)
--     XIMEA:                       NO
--     Xine:                        NO
--     Intel Media SDK:             NO
--     gPhoto2:                     YES
```


# 矩阵运算
[Eigen], [LAPACK],[ATLAS]

# OpenGL支持

编译[OpenCV] 会有一个WITH_OPENGL。这个应该是给highgui界面提供OpenGL硬件渲染用的，默认不开启，即hightgui默认都是cpu渲染。所以要想开启该选项，先要选好一个GUI类型。要么是GTK2要么是Qt.编译的时候
## 开启Qt支持
所以要 **-DWITH_QT=ON -DWITH_OPENGL=ON**,先安装好[Qt]

```
--   GUI: 
--     QT 5.x:                      YES (ver 5.9.1)
--     QT OpenGL support:           YES (Qt5::OpenGL 5.9.1)
--     OpenGL support:              YES (/usr/lib/x86_64-linux-gnu/libGLU.so /usr/lib/x86_64-linux-gnu/libGL.so)
--     VTK support:                 NO
```
## 开启gtk支持
[Using OpenGL with GTK+]这篇文章提及了。Gtk+的OpenGL支持有两种方案，一种是[GtkGLArea]，一种是[GtkGlExt]。而[GtkGlExt]代码比较legacy，官方还不支持Gtk+3。不幸的是,[OpenCV]用的是[GtkGlExt]，而非[GtkGLArea]，所以要么Gtk3+no OpenGL,要么Gtk2+OpenGL。**-DWITH_OPENGL=ON -DWITH_GTK_2_X=ON**
Ubuntu下，就是安装依赖包 libgtkglext1 libgtkglext1-dev
```
--   GUI: 
--     QT:                          NO
--     GTK+ 2.x:                    YES (ver 2.24.30)
--     GThread :                    YES (ver 2.48.2)
--     GtkGlExt:                    YES (ver 1.2.0)
--     OpenGL support:              YES (/usr/lib/x86_64-linux-gnu/libGLU.so /usr/lib/x86_64-linux-gnu/libGL.so)
--     VTK support:                 NO

```

# GPU/CUDA 加速
目前是通过[CUDA]模块来支持的。所以要有NVIDIA显卡了。
先安装好NVIDIA驱动以及[CUDA]，测试可用再运行cmake，将自动检测到支持  
留意到[CUDA]有一个子特性NVCUVID的支持是NO
```
--   NVIDIA CUDA
--     Use CUFFT:                   YES
--     Use CUBLAS:                  YES
--     USE NVCUVID:                 NO
--     NVIDIA GPU arch:             20 30 35 37 50 52 60 61
--     NVIDIA PTX archs:
--     Use fast math:               NO
```
根据官网[NVIDIA VIDEO CODEC SDK]得知，NVCUVID API已经是legacy的了，所以我就大可放心了。
# OCR支持
[Tesseract] 安装好后，将自动检测到
# 图像库
[lept]，  
[HDF5]，  
[libjasper] JPEG-2000 图像格式
# Halide
[Halide] 这个比较有意思，头一次听说，有空再补充
# intel VA-API视频加速
[VA-API]是开源的,另一个地址[VAAPI], cmake编译通过 WITH_VA_INTEL=ON，ubuntu下，安装 libvdpau-va-gl1
# intel多媒体加速
[Intel Media SDK] 是针对intel平台的音视频硬件加速的SDK，需要先安装好[Intel Media Server Studio],而该包有community版本，但安装过程貌似需要重新编译kernel。见[Linux下的安装教程](https://software.intel.com/en-us/articles/how-to-setup-media-server-studio-on-secondary-os-of-linux).考虑到这个会严重到NVIDIA显卡驱动的可用性，本人并未着手安装。
# intel并行计算库
intel开源的[IPP-ICV]是intel [IPP]的图像处理和机器视觉开发包的子集。同上，有专门针对intel架构的优化指令集。

# 行业数据库
[GDAL] 地理抽象数据库。  
[VTK], [ITK] 医学图像库

# 结语
附带我的[OpenCV]编译选项：
```
cmake ../opencv -DWITH_HALIDE=ON -DWITH_FFMPEG=OFF  -DWITH_GSTREAMER=ON -DWITH_QT=ON -DWITH_OPENGL=ON -DWITH_VA_INTEL=ON -DWITH_LIBV4L=ON -DWITH_LIBV4L=ON -DWITH_GDAL=ON -DWITH_GDCM=ON -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules
```
[FFMPEG]编译选项
```
PATH="$HOME/bin:$PATH" PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig" ./configure --prefix="$HOME/ffmpeg_build" --enable-shared --disable-static  --extra-cflags="-I$HOME/ffmpeg_build/include -I/usr/local/cuda/include"   --extra-ldflags="-L$HOME/ffmpeg_build/lib -L/usr/local/cuda/lib64"   --bindir="$HOME/bin"   --enable-gpl   --enable-libass   --enable-libfdk-aac   --enable-libfreetype   --enable-libmp3lame   --enable-libopus   --enable-libtheora   --enable-libvorbis   --enable-libvpx   --enable-libx264   --enable-libx265 --enable-nonfree --enable-cuda --enable-cuvid --enable-nvenc --enable-nonfree --enable-libnpp --enable-libpulse  --enable-libspeex --enable-avisynth  --enable-network --enable-protocol=tcp --enable-demuxer=rtsp
```


[OpenCV]: http://opencv.org/
[OpenCV contrib]: https://github.com/opencv/opencv_contrib
[Eigen]: http://eigen.tuxfamily.org/index.php
[LAPACK]: http://www.netlib.org/lapack
[ATLAS]: http://math-atlas.sourceforge.net/
[Video I/O with OpenCV Overview]: http://docs.opencv.org/trunk/d0/da7/videoio_overview.html
[FFMPEG]: http://ffmpeg.org/
[GStreamer]: https://gstreamer.freedesktop.org/
[Halide]: http://halide-lang.org/
[Qt]: http://download.qt.io/archive/qt/
[GtkGLArea]: https://developer.gnome.org/gtk3/unstable/GtkGLArea.html
[GtkGLExt]: https://projects.gnome.org/gtkglext/
[Using OpenGL with GTK+]: https://www.bassi.io/articles/2015/02/17/using-opengl-with-gtk/
[CUDA]: https://developer.nvidia.com/cuda-downloads
[NVIDIA VIDEO CODEC SDK]: https://developer.nvidia.com/nvidia-video-codec-sdk
[Tesseract]: https://github.com/tesseract-ocr/tesseract/wiki
[HDF5]: https://support.hdfgroup.org/HDF5/
[lept]: http://www.leptonica.com/local-sources.html
[libjasper]: http://www.ece.uvic.ca/~frodo/jasper/
[VA-API]: https://freedesktop.org/wiki/Software/vaapi/
[VAAPI]: https://01.org/linuxmedia/vaapi
[Intel Media SDK]: https://github.com/Intel-Media-SDK/MediaSDK
[Intel Media Server Studio]: https://software.intel.com/en-us/intel-media-server-studio
[IPP]: https://software.intel.com/en-us/intel-ipp
[IPP-ICV]: https://software.intel.com/en-us/articles/intel-integrated-performance-primitives-intel-ipp-open-source-computer-vision-library-opencv-faq
[GDAL]: http://www.gdal.org/
[VTK]: https://www.vtk.org/
[ITK]: https://itk.org/
