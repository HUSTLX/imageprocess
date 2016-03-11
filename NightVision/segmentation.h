void rgb_seg(unsigned char* des, const unsigned char* src, int width, int height, int nChannels);
void hsi_seg(unsigned char* des, const unsigned char* src, int width, int height, int nChannels);
void ImageDilation(unsigned char* des, const unsigned char* src, int width, int height,int nChannels,int M);
void ImageErosion(unsigned char* des, const unsigned char* src, int width, int height, int nChannels,int M);
void ImageReturn(unsigned char* des, const unsigned char* ocl, const unsigned char* src, int width, int height, int nChannels,int M);
void rgb_hsi(unsigned char* des, const unsigned char* src, int width, int height, int nChannels);
void hsi_rgb(unsigned char* des, const unsigned char* src, int width, int height, int nChannels);
void Imgfill(unsigned char* des, const unsigned char* src, int width, int height, int nChannels);
void win_enhance(unsigned char* des, const unsigned char* src, int width, int height, int nChannels);