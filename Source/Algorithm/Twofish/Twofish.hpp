#pragma once
#ifndef ENIGMA_TWOFISH_H
#define ENIGMA_TWOFISH_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/*
*	Twofish-GCM Algorithm Encryptor/Decryptor
*/
class Twofish : public Algorithm
{
public:
	/*
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit Twofish(Algorithm::Intent intent) noexcept;
	virtual ~Twofish() noexcept;

public:
	String Encrypt(const String& password, const String& buffer) noexcept(false) override;
	String Decrypt(const String& password, const String& iv_cipher) noexcept(false) override;

private:
	std::unique_ptr<CryptoPP::GCM<CryptoPP::Twofish>::Encryption> m_twofish_encryption; // Twofish-GCM encryptor
	std::unique_ptr<CryptoPP::GCM<CryptoPP::Twofish>::Decryption> m_twofish_decryption; // Twofish-GCM decryptor
};

NS_ENIGMA_END
#endif // !ENIGMA_TWOFISH_H

