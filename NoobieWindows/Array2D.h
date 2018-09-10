#pragma once

//template<typename _Ty>
//class Array2D
//{
//	_Ty * data;
//	size_t width, height;
//public:
//	Array2D(size_t width, size_t height) : width(width), height(height)
//	{
//		data = new _Ty[width * height];
//	}
//	~Array2D()
//	{
//		delete[] data;
//	}
//
//	_Ty Get(size_t x, size_t y){
//		return data[y * height + x];
//	}
//
//	_Ty * operator[](size_t i)
//	{
//		// to be used as [][]
//		// first gets the address of the first column, second addresses the array direectally
//		return &data[i];
//	}
//
//};
//
