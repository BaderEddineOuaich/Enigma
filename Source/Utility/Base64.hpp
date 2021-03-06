#pragma once
#ifndef ENIGMA_BASE64_H
#define ENIGMA_BASE64_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
class ENIGMA_API Base64
{
public:
    /*
     *	Encodes string bytes into Base64 string
     */
	static String Encode(const std::string_view& buffer);

    /*
    *   Decodes a Base64 string
    */
	static String Decode(const std::string_view& encoded_buffer);

	/*
	*	Checks if a byte is base64
	*/
	static inline const bool IsBase64(const byte& c) noexcept;

private:
	inline static constexpr const byte BASE64_CHARACTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

};
NS_ENIGMA_END

#endif // !ENIGMA_BASE64_H
