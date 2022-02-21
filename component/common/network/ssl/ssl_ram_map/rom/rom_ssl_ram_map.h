#ifndef ROM_SSL_RAM_MAP_H
#define ROM_SSL_RAM_MAP_H

#include "basic_types.h"

struct _rom_ssl_ram_map {
	/* OS interface */
	void *(*ssl_malloc)(unsigned int sz);
	void (*ssl_free)(void *);
	int (*ssl_printf)(const char *, ...);

	//AES HW CRYPTO
	int (*hw_crypto_aes_ecb_init)(const uint8_t* key, const uint32_t keylen);
	int (*hw_crypto_aes_ecb_decrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_aes_ecb_encrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_aes_cbc_init)(const uint8_t* key, 	const uint32_t keylen);
	int (*hw_crypto_aes_cbc_decrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_aes_cbc_encrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);

	//DES HW CRYPTO
	int (*hw_crypto_des_cbc_init)(const uint8_t* key, const uint32_t keylen);
	int (*hw_crypto_des_cbc_decrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_des_cbc_encrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_3des_cbc_init)(const uint8_t* key, const uint32_t keylen);
	int (*hw_crypto_3des_cbc_decrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_3des_cbc_encrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);

	/* Variables */
	uint32_t use_hw_crypto_func;
};

struct _rom_mbedtls_ram_map {
	/* OS interface */
	void *(*ssl_calloc)(unsigned int, unsigned int);
	void (*ssl_free)(void *);
	int (*ssl_printf)(const char *, ...);

	//AES HW CRYPTO
	int (*hw_crypto_aes_ecb_init)(const uint8_t* key, const uint32_t keylen);
	int (*hw_crypto_aes_ecb_decrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_aes_ecb_encrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_aes_cbc_init)(const uint8_t* key, const uint32_t keylen);
	int (*hw_crypto_aes_cbc_decrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_aes_cbc_encrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);

	//DES HW CRYPTO
	int (*hw_crypto_des_cbc_init)(const uint8_t* key, const uint32_t keylen);
	int (*hw_crypto_des_cbc_decrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_des_cbc_encrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_3des_cbc_init)(const uint8_t* key, const uint32_t keylen);
	int (*hw_crypto_3des_cbc_decrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);
	int (*hw_crypto_3des_cbc_encrypt)(
		const uint8_t* message, 	const uint32_t msglen, 
		const uint8_t* iv, 		const uint32_t ivlen, 
		uint8_t* pResult);

	/* Variables */
	uint32_t use_hw_crypto_func;
};

extern struct _rom_ssl_ram_map rom_ssl_ram_map;
extern struct _rom_mbedtls_ram_map *p_rom_ssl_ram_map; //=&rom_ssl_ram_map init in mbedtls_platform_set_calloc_free
#endif	/* ROM_SSL_RAM_MAP_H */
