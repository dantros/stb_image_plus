#include <stb_image_plus.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize2.h>

void DebugCheck(bool condition)
{
    #ifdef _DEBUG
    if (not condition)
        throw;
    #endif
}

namespace stb_image_plus
{

template <std::size_t DesiredChannels>
PixelT<DesiredChannels>::PixelT(std::initializer_list<std::uint8_t> values)
{
    DebugCheck(values.size() == DesiredChannels);
    std::size_t index = 0;
    for (auto& value : values)
    {
        mData[index] = value;
        index++;
    }
}

template <std::size_t DesiredChannels>
const std::uint8_t& PixelT<DesiredChannels>::operator[](std::size_t coord) const
{
    DebugCheck(coord < DesiredChannels);
    return mData[coord];
}

template <std::size_t DesiredChannels>
std::uint8_t& PixelT<DesiredChannels>::operator[](std::size_t coord)
{
    DebugCheck(coord < DesiredChannels);
    return mData[coord];
}

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
std::span<typename ImageData<DesiredChannels>::Pixel> ImageData<DesiredChannels>::pixelSpan()
{
    using Pixel = typename ImageData<DesiredChannels>::Pixel;
    Pixel* firstPixelPtr = reinterpret_cast<Pixel*>(mData);
    const std::size_t numberOfPixels = mWidth * mHeight;
    return std::span<Pixel>(firstPixelPtr, numberOfPixels);
}

template <std::size_t DesiredChannels>
std::span<const typename ImageData<DesiredChannels>::Pixel> ImageData<DesiredChannels>::pixelSpan() const
{
    using Pixel = typename ImageData<DesiredChannels>::Pixel;
    const Pixel* firstPixelPtr = reinterpret_cast<const Pixel*>(mData);
    const std::size_t numberOfPixels = mWidth * mHeight;
    return std::span<const Pixel>(firstPixelPtr, numberOfPixels);
}

template <std::size_t DesiredChannels>
const typename ImageData<DesiredChannels>::Pixel& ImageData<DesiredChannels>::at(std::size_t col, std::size_t row) const
{
    DebugCheck(isValid());
    DebugCheck(col < mWidth);
    DebugCheck(row < mHeight);
    const std::size_t indexOffset = mWidth * row + col;
    using Pixel = typename ImageData<DesiredChannels>::Pixel;
    std::span<const Pixel> pixels = pixelSpan();
    return pixels[indexOffset];
}

template <std::size_t DesiredChannels>
typename ImageData<DesiredChannels>::Pixel& ImageData<DesiredChannels>::at(std::size_t col, std::size_t row)
{
    DebugCheck(isValid());
    DebugCheck(col < mWidth);
    DebugCheck(row < mHeight);
    const std::size_t indexOffset = mWidth * row + col;
    using Pixel = typename ImageData<DesiredChannels>::Pixel;
    std::span<Pixel> pixels = pixelSpan();
    return pixels[indexOffset];
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

// template instantiations

template class ImageData<1>;
template class ImageData<2>;
template class ImageData<3>;
template class ImageData<4>;

template class PixelT<1>;
template class PixelT<2>;
template class PixelT<3>;
template class PixelT<4>;

}