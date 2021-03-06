#include "pch.hpp"
#include "ChaCha20.hpp"


NS_ENIGMA_BEGIN

ChaCha20::ChaCha20(Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::ChaCha20, intent),
	m_chacha_encryption(intent == Algorithm::Intent::Encrypt ? std::make_unique<CryptoPP::ChaCha::Encryption>() : nullptr),
	m_chacha_decryption(intent == Algorithm::Intent::Decrypt ? std::make_unique<CryptoPP::ChaCha::Decryption>() : nullptr)
{
}

ChaCha20::~ChaCha20() noexcept
{
}

String ChaCha20::Encrypt(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized
	{
		ENIGMA_ASSERT_OR_THROW(m_chacha_encryption, "ChaCha20 Encryption is not initialized properly");
		ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "ChaCha20 Encryption seeder is not initialized properly");
	}

	// Validate Arguments
	{
		// ChaCha20 password length must be at least 9 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "ChaCha20 Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	String iv = this->GenerateRandomIV(CryptoPP::ChaCha::IV_LENGTH); // Randomly generated 8 bytes ChaCha20 IV
	String cipher{}; // Final encrypted buffer
	String output(sizeof(Algorithm::Type), static_cast<const byte>(this->GetType())); // return value will be (AlgoType + IV + Cipher)

	// Prepare key
	CryptoPP::SecByteBlock key(CryptoPP::ChaCha::MAX_KEYLENGTH + CryptoPP::ChaCha::IV_LENGTH);
		
	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key,
		key.size(),
		reinterpret_cast<const byte*>(password.data()),
		password.size(),
		reinterpret_cast<const byte*>(iv.data()),
		iv.size(),
		nullptr,
		0);

	// Set Key and IV to the encryptor
	m_chacha_encryption->SetKeyWithIV(key, CryptoPP::ChaCha::MAX_KEYLENGTH, key + CryptoPP::ChaCha::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Encrypt
	std::unique_ptr<CryptoPP::StringSource> encryptor = std::make_unique<CryptoPP::StringSource>(
		buffer,
		true,
		new CryptoPP::StreamTransformationFilter(
			*m_chacha_encryption,
			new CryptoPP::StringSink(cipher)
		)
		);
	//NOTE: StringSource will auto clean the allocated memory

	// Output (AlgoType + IV + Cipher) since we need IV and Algorithm used for encryption later for decryption
	output.append(std::move(iv + cipher));

	return output;
}

String ChaCha20::Decrypt(const String& password, const String& iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT_OR_THROW(m_chacha_decryption, "ChaCha20 Decryption is not initialized properly");

	// Split IV and Cipher from buffer (we output encrypted buffers as String(AlgoType + IV + Cipher))
	const String iv = iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::ChaCha::IV_LENGTH);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV from cipher");
	const String cipher = iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::ChaCha::IV_LENGTH, iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher from cipher");

	// Recovered buffer
	String decrypted{};

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::ChaCha::MAX_KEYLENGTH + CryptoPP::ChaCha::IV_LENGTH);
	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key,
		key.size(),
		reinterpret_cast<const byte*>(password.data()),
		password.size(),
		reinterpret_cast<const byte*>(iv.data()),
		iv.size(),
		nullptr,
		0);

	// Set Key and IV to the decryptor
	m_chacha_decryption->SetKeyWithIV(key, CryptoPP::ChaCha::MAX_KEYLENGTH, key + CryptoPP::ChaCha::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Decrypt
	std::unique_ptr<CryptoPP::StringSource> decryptor = std::make_unique<CryptoPP::StringSource>(
		cipher,
		true,
		new CryptoPP::StreamTransformationFilter(
			*m_chacha_decryption,
			new CryptoPP::StringSink(decrypted)
		)
		);

	//NOTE: StringSource will auto clean the allocated memory

	return decrypted;
}

NS_ENIGMA_END

