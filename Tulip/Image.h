#pragma once
#include<memory>
#include<filesystem>
#include<type_traits>
#include"GDIPlusManager.h"

struct ColorType
{
	unsigned char a = 255;
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
};

class Image
{
private:
	unsigned int height = 0;
	unsigned int width = 0;
	GDIPlusManager manager;
	Gdiplus::Bitmap bitmap;
public:
	Image(const std::filesystem::path& file);
	Image(unsigned int width, unsigned height);
	unsigned int GetHeight() const;
	unsigned int GetWidth() const;
	ColorType GetPixel(unsigned int x , unsigned int y) const;
	void SetPixel(unsigned int x, unsigned int y, ColorType color);
	void DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ColorType color);
	const ColorType* Raw() const;
};