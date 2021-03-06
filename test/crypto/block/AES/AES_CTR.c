#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <AES.h>
#include <CRYPTO_LIB_block_modes.h>
#include "AES_CBC_test_vectors.h"
#include "AES_CTR_test_vectors.h"

enum AES_KEY_ROUNDS AES_get_key_rounds(enum AES_KEY_SZ AES_key_size)
{
	switch (AES_key_size)
	{
		case AES_KEY_SZ_16:
			return AES_KEY_ROUND_10;
		case AES_KEY_SZ_24:
			return AES_KEY_ROUND_12;
		case AES_KEY_SZ_32:
			return AES_KEY_ROUND_14;
	}
}

void AES_CTR_encrypt_test(struct CRYPTO_context *ctx)
{
	CRYPTO_LIB_CTR_encrypt(ctx);
}
void AES_CTR_decrypt_test(struct CRYPTO_context *ctx)
{
	//CRYPTO_LIB_CTR_decrypt(ctx);
}


int main(int argc, char *argv[])
{
	int i;
	/* Create context */
	struct CRYPTO_context *ctx = malloc(sizeof(struct CRYPTO_context));
	struct AES_test_vector *vector = &AES_ctr_test_vector_1;

	uint8_t plaintext[512];
	memcpy(plaintext, vector->plaintext.data, vector->plaintext.len);

	memset(ctx, 0, sizeof(*ctx));
	ctx->key_size = vector->key.len;
	ctx->key_rounds = AES_get_key_rounds(ctx->key_size);
	ctx->mode = vector->mode;
	ctx->key = vector->key.data;
	ctx->block_size = AES_BLOCK_SZ;
	ctx->crypto_encrypt_block = AES_encrypt_block;
	ctx->crypto_decrypt_block = AES_decrypt_block;
	ctx->iv = vector->iv;
	ctx->plaintext = plaintext;
	ctx->blocks_count = vector->plaintext.len / AES_BLOCK_SZ;

	AES_expand_keys((struct AES_context *)ctx);

	hex_dump("plaintext", ctx->plaintext, vector->plaintext.len, 16);

	AES_CTR_encrypt_test(ctx);

	hex_dump("ciphertext ", ctx->plaintext, vector->plaintext.len, 16);
	
	memcpy(plaintext, vector->ciphertext.data, vector->ciphertext.len);

	//AES_CTR_decrypt_test(ctx);

	//hex_dump("plaintext ", ctx->plaintext, vector->plaintext.len, 16);


	free(ctx);

	return 0;
}

