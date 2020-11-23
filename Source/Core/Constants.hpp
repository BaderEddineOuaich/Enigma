#pragma once
#ifndef ENIGMA_CONSTANTS_H
#define ENIGMA_CONSTANTS_H

#include "Types.hpp"

#include <math.h> // M_PI
#include <aes.h> // MAX_KEYLENGTH, BLOCKSIZE

namespace Enigma
{
	namespace Constants
	{
		namespace Algorithm
		{
			namespace AES
			{
				static constexpr const ui16 AES_MINIMUM_PASSWORD_LENGTH = 9; // at least 6 to 64 characters
			}
		}

		namespace Utility
		{
			static constexpr const ui8 BASE64_CHARACTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		}

		namespace Fonts
		{
			/*
			 Enigma.ttf
			 Pieces of Eight.ttf
			 NunitoSans-Regular.ttf
			 Goldman-Bold.ttf
			 Goldman-Regular.ttf
			 Ubuntu-Bold.ttf
			 Ubuntu-BoldItalic.ttf
			 Ubuntu-Italic.ttf
			 Ubuntu-Light.ttf
			 Ubuntu-LightItalic.ttf
			 Ubuntu-Medium.ttf
			 Ubuntu-MediumItalic.ttf
			 Ubuntu-Regular.ttf
			*/
			static constexpr const auto ENIGMA_FONT_PATH = "./Resources/Fonts/Enigma.ttf";
			static constexpr const auto PIECES_OF_EIGHT_FONT_PATH = "./Resources/Fonts/Pieces of Eight.ttf";
			static constexpr const auto NUNITO_SANS_REGULAR_FONT_PATH = "./Resources/Fonts/NunitoSans-Regular.ttf";
			static constexpr const auto GOLDMAN_BOLD_FONT_PATH = "./Resources/Fonts/Goldman-Bold.ttf";
			static constexpr const auto GOLDMAN_REGULAR_FONT_PATH = "./Resources/Fonts/Goldman-Regular.ttf";
			static constexpr const auto UBUNTU_BOLD_FONT_PATH = "./Resources/Fonts/Ubuntu-Bold.ttf";
			static constexpr const auto UBUNTU_BOLD_ITALIC_FONT_PATH = "./Resources/Fonts/Ubuntu-BoldItalic.ttf";
			static constexpr const auto UBUNTU_ITALIC_FONT_PATH = "./Resources/Fonts/Ubuntu-Italic.ttf";
			static constexpr const auto UBUNTU_LIGHT_FONT_PATH = "./Resources/Fonts/Ubuntu-Light.ttf";
			static constexpr const auto UBUNTU_LIGHT_ITALIC_FONT_PATH = "./Resources/Fonts/Ubuntu-LightItalic.ttf";
			static constexpr const auto UBUNTU_MEDIUM_FONT_PATH = "./Resources/Fonts/Ubuntu-Medium.ttf";
			static constexpr const auto UBUNTU_MEDIUM_ITALIC_FONT_PATH = "./Resources/Fonts/Ubuntu-MediumItalic.ttf";
			static constexpr const auto UBUNTU_REGULAR_FONT_PATH = "./Resources/Fonts/Ubuntu-Regular.ttf";


		}

		namespace Colors
		{
			//#define VEC_RGBA(r, g, b, a) ImVec4(r/255.0f, g/255.0f, b/255.0f, a/255.0f)
			#define FLOAT_RGBA(r, g, b, a) {r/255.0f, g/255.0f, b/255.0f, a/255.0f}
			static const float BACKGROUND_COLOR[] = FLOAT_RGBA(46, 33, 59, 255);

		}

	}
}

#endif // !ENIGMA_CONSTANTS_H
