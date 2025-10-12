#include <stb_image_plus.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize2.h>

namespace stb_image_plus
{

template <std::size_t DesiredChannels>
ImageData<DesiredChannels>::ImageData() :
    mData(nullptr),
    mWidth(0),
    mHeight(0),
    mInternalChannels(0)
{
}

template <std::size_t DesiredChannels>
ImageData<DesiredChannels>::ImageData(const std::string &filename)
{
    read(filename);
}

template <std::size_t DesiredChannels>
bool ImageData<DesiredChannels>::read(const std::string &filename)
{
    int width = 0, height = 0, internalChannels = 0;
    mData = stbi_load(filename.c_str(), &width, &height, &internalChannels, DesiredChannels);
    mWidth = static_cast<std::size_t>(width);
    mHeight = static_cast<std::size_t>(height);
    mInternalChannels = static_cast<std::size_t>(internalChannels);
    return mData != nullptr;
}

template <std::size_t DesiredChannels>
bool ImageData<DesiredChannels>::write(const std::string &filename)
{
    int result = stbi_write_bmp(filename.c_str(), width(), height(), DesiredChannels, mData);
    return result == 0;
}

template <std::size_t DesiredChannels>
bool ImageData<DesiredChannels>::isValid() const
{
    return mData != nullptr;
}

template <std::size_t DesiredChannels>
unsigned char* getDataPtr(const ImageData<DesiredChannels>& image, std::size_t col, std::size_t row)
{
    if (!image.isValid()) throw;
    if (col >= image.width() || row >= image.height()) throw;
    const std::size_t indexOffset = image.width() * row + col;
    const std::size_t dataOffset = indexOffset * DesiredChannels;
    return image.data() + dataOffset;
}

template <>
typename ImageData<1>::Pixel ImageData<1>::at(std::size_t col, std::size_t row) const
{
    unsigned char* dataPtr = getDataPtr(*this, col, row);
    typename ImageData<1>::Pixel output;
    output.mData[0] = *dataPtr;
    return output;
}

template <>
typename ImageData<2>::Pixel ImageData<2>::at(std::size_t col, std::size_t row) const
{
    unsigned char* dataPtr = getDataPtr(*this, col, row);
    typename ImageData<2>::Pixel output;
    output.mData[0] = *dataPtr;
    output.mData[1] = *(dataPtr+1);
    return output;
}

template <>
typename ImageData<3>::Pixel ImageData<3>::at(std::size_t col, std::size_t row) const
{
    unsigned char* dataPtr = getDataPtr(*this, col, row);
    typename ImageData<3>::Pixel output;
    output.mData[0] = *dataPtr;
    output.mData[1] = *(dataPtr+1);
    output.mData[2] = *(dataPtr+2);
    return output;
}

template <>
typename ImageData<4>::Pixel ImageData<4>::at(std::size_t col, std::size_t row) const
{
    unsigned char* dataPtr = getDataPtr(*this, col, row);
    typename ImageData<4>::Pixel output;
    output.mData[0] = *dataPtr;
    output.mData[1] = *(dataPtr+1);
    output.mData[2] = *(dataPtr+2);
    output.mData[3] = *(dataPtr+3);
    return output;
}

template <>
void ImageData<1>::set(std::size_t col, std::size_t row, const ImageData<1>::Pixel& pixel)
{
    unsigned char* dataPtr = getDataPtr(*this, col, row);
    *dataPtr = pixel.mData[0];
}

template <>
void ImageData<2>::set(std::size_t col, std::size_t row, const ImageData<2>::Pixel& pixel)
{
    unsigned char* dataPtr = getDataPtr(*this, col, row);
    *dataPtr = pixel.mData[0];
    *(dataPtr + 1) = pixel.mData[1];
}

template <>
void ImageData<3>::set(std::size_t col, std::size_t row, const ImageData<3>::Pixel& pixel)
{
    unsigned char* dataPtr = getDataPtr(*this, col, row);
    *dataPtr = pixel.mData[0];
    *(dataPtr + 1) = pixel.mData[1];
    *(dataPtr + 2) = pixel.mData[2];
}

template <>
void ImageData<4>::set(std::size_t col, std::size_t row, const ImageData<4>::Pixel& pixel)
{
    unsigned char* dataPtr = getDataPtr(*this, col, row);
    *dataPtr = pixel.mData[0];
    *(dataPtr + 1) = pixel.mData[1];
    *(dataPtr + 2) = pixel.mData[2];
    *(dataPtr + 3) = pixel.mData[3];
}

template <std::size_t DesiredChannels>
ImageData<DesiredChannels> ImageData<DesiredChannels>::resize(std::size_t width, std::size_t height)
{
    ImageData<DesiredChannels> output;
    output.mData = nullptr;
    output.mWidth = width;
    output.mHeight = height;
    output.mInternalChannels = DesiredChannels;

    // This may not work as expected for pixel layouts different than STBIR_1CHANNEL, STBIR_2CHANNEL, STBIR_RGB, STBIR_RGBA
    stbir_pixel_layout pixelLayout = static_cast<stbir_pixel_layout>(DesiredChannels);

    output.mData = stbir_resize_uint8_srgb(
        mData, mWidth, mHeight, DesiredChannels * mWidth,
        NULL, width, height, DesiredChannels * width, pixelLayout);

    return output;
}

template <std::size_t DesiredChannels>
ImageData<DesiredChannels> ImageData<DesiredChannels>::resizeToWidth(std::size_t width)
{
    std::size_t height = mHeight * width / mWidth;
    return resize(width, height);
}

template <std::size_t DesiredChannels>
ImageData<DesiredChannels> ImageData<DesiredChannels>::resizeToHeight(std::size_t height)
{
    std::size_t width = mWidth * height / mHeight;
    return resize(width, height);
}

template <std::size_t DesiredChannels>
ImageData<DesiredChannels>::~ImageData()
{
    if (mData == nullptr)
        return;
    stbi_image_free(mData);
}

template class ImageData<1>;
template class ImageData<2>;
template class ImageData<3>;
template class ImageData<4>;

}