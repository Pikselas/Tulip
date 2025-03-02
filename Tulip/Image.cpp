#include "Image.h"

Image::Image(const std::filesystem::path& file) : bitmap(file.c_str())
{
	if (bitmap.GetLastStatus() == Gdiplus::Status::Ok)
	{
		height = bitmap.GetHeight();
		width = bitmap.GetWidth();
	}
	else
	{
		throw std::runtime_error("Failed to load image");
	}
}

Image::Image(unsigned int width,unsigned int height) : bitmap(width,height,PixelFormat32bppARGB)
{
	this->width = width;
	this->height = height;
}

unsigned int Image::GetHeight() const
{
	return height;
}

unsigned int Image::GetWidth() const 
{
    return width;
}

ColorType Image::GetPixel(unsigned int x, unsigned int y) const
{
	Gdiplus::Color c;
	auto bitmapPtr = const_cast<Gdiplus::Bitmap*>(&bitmap);
	if (bitmapPtr->GetPixel(x, y, &c) != Gdiplus::Ok)
	{
		return { 0,0,0,0 };
	}
	return static_cast<ColorType>(c.GetValue());
}

void Image::SetPixel(unsigned int x, unsigned int y, ColorType color)
{
	bitmap.SetPixel(x, y, Gdiplus::Color(color.a, color.r, color.g, color.b));
}

void Image::DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ColorType color)
{
	Gdiplus::Graphics graphics(&bitmap);
	Gdiplus::Pen pen(Gdiplus::Color(color.a, color.r, color.g, color.b));
	graphics.DrawLine(&pen, (int)x1, (int)y1, (int)x2, (int)y2);
}

const ColorType* Image::Raw() const
{
	Gdiplus::BitmapData data;
	auto bitmapPtr = const_cast<Gdiplus::Bitmap*>(&bitmap);
	Gdiplus::Rect rect(0, 0, width, height);
	bitmapPtr->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);
	bitmapPtr->UnlockBits(&data);
	return static_cast<ColorType*>(data.Scan0);
}