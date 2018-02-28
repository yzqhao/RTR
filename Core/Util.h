
#ifndef _CORE_UTIL_H_
#define _CORE_UTIL_H_

#include "type.h"
#include <sstream>

#define SafeDelete(ptr) if (ptr) { delete ptr; ptr = 0; }
#define SafeDeleteArray(ptr) if (ptr) { delete [] ptr; ptr = 0; }
#define Abs(a) ((a) > 0 ? (a) : -(a))

// 角度变弧度转换, 弧度便角度转换
#define Degree_TO_Radian(deg) (deg * 0.017453f)
#define Radian_To_Degree(rad) (rad * 57.295780f)

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

namespace RTR
{
	//! Rounding error constant often used when comparing f32 values.
	const f32 ROUNDING_ERROR = 0.0001f;

	//! returns minimum of two values. Own implementation to get rid of the STL.
	template<class T>
	inline const T Min(const T a, const T b)
	{
		return a < b ? a : b;
	}

	//! returns minimum of two values. Own implementation to get rid of the STL
	template<class T>
	inline T Max(const T a, const T b)
	{
		return a < b ? b : a;
	}

	//! returns if a float equals the other one, taking floating 
	//! point rounding errors into account
	inline bool equals(f32 a, f32 b)
	{
		return (a + ROUNDING_ERROR > b) && (a - ROUNDING_ERROR < b);
	}

	const std::string	DEFAULT_NAME = "E3D_Default";

	const std::string	DefaultPath = "Media/";
	static std::string GetPath(const std::string& file){ return DefaultPath + file;}

	inline s32 RandomInt(s32 from = 0, s32 to = 10)
	{
		s32 ran = rand() % (to - from + 1) + from;
		return ran;
	}

	inline std::string IntToString(f32 num)
	{
		std::stringstream ss;
		ss << num;
		std::string re;
		ss >> re;
		return re;
	}

	inline s32 StringToInt(const std::string &str)
	{
		return atoi(str.c_str());
	}

	inline f32 StringToFloat(const std::string &str)
	{
		return (f32)atof(str.c_str());
	}

	// 将给定的字符串两端的空白字符删除
	inline std::string Trim(const std::string &msg)
	{
		const static std::string SPACE_CHAR = " \t\f\v\n\r";
		std::size_t beg = msg.find_first_not_of(SPACE_CHAR);

		// 防止空字符串出现
		if (beg > msg.length())
			return std::string();
		std::string result = msg.substr(beg);
		std::size_t end = result.find_last_not_of(SPACE_CHAR);
		if (end != std::string::npos)
			end++;

		return result.substr(0, end);
	}

	// 从路径里面截取文件名称, 如果只有文件名称,则直接返回(不包含后缀)
	inline std::string GetNameFromPath(const std::string &path)
	{
		std::size_t beg = path.find_last_of("\\/");
		std::size_t end = path.find_last_of(".");

		if (beg == std::string::npos)
			beg = -1;
		return path.substr(beg + 1, end - beg - 1);
	}
}

#endif

