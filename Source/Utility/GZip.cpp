#include <pch.hpp>
#include "GZip.hpp"

NS_ENIGMA_BEGIN

String GZip::Compress(const std::string_view& buffer) noexcept(false)
{
	String compressed{};
	m_zipper.reset(new CryptoPP::Gzip(new CryptoPP::StringSink(compressed), CryptoPP::Gzip::MAX_DEFLATE_LEVEL));
	m_zipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_zipper->MessageEnd();
	return compressed;
}

String GZip::Decompress(const std::string_view& buffer) noexcept(false)
{
	String decompressed{};
	m_unzipper.reset(new CryptoPP::Gunzip(new CryptoPP::StringSink(decompressed)));
	m_unzipper->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	m_unzipper->MessageEnd();
	return decompressed;
}

NS_ENIGMA_END

