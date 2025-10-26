#pragma once
#include <string>
#include <array>
#include <cstddef>
#include <initializer_list>
#include <span>

namespace stb_image_plus
{
template <std::size_t DesiredChannels>
struct PixelT
{
public:
    PixelT() : mData() {};
    PixelT(std::initializer_list<std::uint8_t> values);
    const std::uint8_t& operator[](std::size_t coord) const;
    std::uint8_t& operator[](std::size_t coord);

private:
    std::array<std::uint8_t, DesiredChannels> mData;
};

template <std::size_t DesiredChannels>
class ImageData
{
public:
    using Pixel = PixelT<DesiredChannels>;

    ImageData();
    ImageData(const std::string& filename);
    bool read(const std::string& filename);
    bool write(const std::string& filename);
    bool isValid() const;
    std::span<Pixel> pixelSpan();
    std::span<const Pixel> pixelSpan() const;
    std::size_t width() const { return mWidth; }
    std::size_t height() const { return mHeight; }
    std::size_t internalChannels() const { return mInternalChannels; }
    std::size_t desiredChannels() const { return DesiredChannels; }
    const Pixel& at(std::size_t col, std::size_t row) const;
    Pixel& at(std::size_t col, std::size_t row);
    ImageData<DesiredChannels> resize(std::size_t width, std::size_t height);
    ImageData<DesiredChannels> resizeToWidth(std::size_t width);
    ImageData<DesiredChannels> resizeToHeight(std::size_t height);
    ~ImageData();

private:
    unsigned char* mData;
    std::size_t mWidth, mHeight, mInternalChannels;
};

// Only these types are defined.
using Pixel1 = PixelT<1>;
using Pixel2 = PixelT<2>;
using Pixel3 = PixelT<3>;
using Pixel4 = PixelT<4>;

using ImageData1 = ImageData<1>;
using ImageData2 = ImageData<2>;
using ImageData3 = ImageData<3>;
using ImageData4 = ImageData<4>;

}