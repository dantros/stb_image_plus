#pragma once
#include <string>
#include <array>

namespace stb_image_plus
{

template <std::size_t DesiredChannels>
class ImageData
{
public:
    struct Pixel
    {
        std::array<std::uint8_t, DesiredChannels> mData;
    };

    ImageData();
    ImageData(const std::string& filename);
    bool read(const std::string& filename);
    bool write(const std::string& filename);
    bool isValid() const;
    unsigned char* data() const { return mData; }
    std::size_t width() const { return mWidth; }
    std::size_t height() const { return mHeight; }
    std::size_t internalChannels() const { return mInternalChannels; }
    std::size_t desiredChannels() const { return DesiredChannels; }
    typename ImageData<DesiredChannels>::Pixel at(std::size_t col, std::size_t row) const;
    void set(std::size_t col, std::size_t row, const Pixel& pixel);
    ImageData<DesiredChannels> resize(std::size_t width, std::size_t height);
    ImageData<DesiredChannels> resizeToWidth(std::size_t width);
    ImageData<DesiredChannels> resizeToHeight(std::size_t height);
    ~ImageData();

private:
    unsigned char* mData;
    std::size_t mWidth, mHeight, mInternalChannels;
};

// Only these types are defined.
using ImageData1 = ImageData<1>;
using ImageData2 = ImageData<2>;
using ImageData3 = ImageData<3>;
using ImageData4 = ImageData<4>;

}