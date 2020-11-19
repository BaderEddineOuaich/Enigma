#pragma once
#ifndef ENIGMA_TYPES_H
#define ENIGMA_TYPES_H

#include <cstdint>	// std::int*_t
#include <cstddef>	// std::size_t
#include <memory>	// smart pointers
#include <string>	//std::string, std::string_view
#include <sstream>	//std::stringstream

namespace Enigma
{
	using i8	= std::int8_t;
	using i16	= std::int16_t;
	using i32	= std::int32_t;
	using i64	= std::int64_t;
	using ui8	= std::uint8_t;
	using ui16	= std::uint16_t;
	using ui32	= std::uint32_t;
	using ui64	= std::uint64_t;
	using f32	= float;
	using f64	= double;
	using byte	= ui8;
	using size_t	= std::size_t;
	using String	= std::string;
	using StringStream	= std::stringstream;
	using StringView	= std::string_view;


	/*
	*	Smart pointers
	*/
	template<typename T>
	using UniquePtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr UniquePtr<T> MakeUnique(Args && ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using SharedPtr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr SharedPtr<T> MakeShared(Args && ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}



	/*
	*	Some Literal Size Convertors
	*/
	// bytes to kilobytes
	inline constexpr size_t operator ""_KB(const size_t bytes) noexcept
	{
		return bytes * 1024ULL;
	}
	// bytes to megabytes
	inline constexpr size_t operator ""_MB(const size_t bytes) noexcept
	{
		return bytes * 1024ULL * 1024ULL;
	}
	// bytes to gigabytes
	inline constexpr size_t operator ""_GB(const size_t bytes) noexcept
	{
		return bytes * 1024ULL * 1024ULL * 1024ULL;
	}
	// bytes to terabytes
	inline constexpr size_t operator ""_TB(const size_t bytes) noexcept
	{
		return bytes * 1024ULL * 1024ULL * 1024ULL * 1024ULL;
	}

}

#endif // !ENIGMA_TYPES_H