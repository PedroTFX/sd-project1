#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data.h"
#include "entry.h"
#include "serialization-private.h"

/* Serializa todas as keys presentes no array de strings keys
 * para o buffer keys_buf que será alocado dentro da função.
 * O array de keys a passar em argumento pode ser obtido através
 * da função tree_get_keys. Para além disso, retorna o tamanho do
 * buffer alocado ou -1 em caso de erro.
 */
// char *keys_buf;
// keyArray_to_buffer(keys, &keys_buf);
// Aqui o keys_buf já aponta para uma posição de memória preenchida
int keyArray_to_buffer(char **keys, char **keys_buf)
{
	//[10]["qwertyuiop"]	[2]["ab"]	[3]["qwe"]	[0]
	// Descobrir quantos bytes temos que alocar para o buffer

	/* int size = 4; //calc_buffer_size()
	for (int i = 0; keys[i] != NULL; i++) {
		size += 4 + strlen(keys[i]);
	} */

	int size = calc_buffer_size(keys);
	for (int i = 0; keys[i] != NULL; i++)
	{
		size += 4 + strlen(keys[i]);
	}

	// Alocar esse espaço para o buffer
	*keys_buf = (char *)malloc(size);

	// create_buffer
	//  Preencher o buffer com as strings
	create_buffer(keys, keys_buf);
	/* 	for (int i = 0; keys[i] != NULL; i++) {
			int strlength = strlen(keys[i]);
			memcpy(*keys_buf, &strlength, sizeof(int));
			*keys_buf += sizeof(int);
			memcpy(*keys_buf, keys[i], strlength);
			*keys_buf += strlength;
		}

		// e meter 0 final (create_buffer)
		int f = 0;
		memcpy(*keys_buf, &f, sizeof(int)); */

	// Retornar a quantidade de bytes alocados
	return size;
}

/* De-serializa a mensagem contida em keys_buf, com tamanho
 * keys_buf_size, colocando-a e retornando-a num array char**,
 * cujo espaco em memória deve ser reservado. Devolve NULL
 * em caso de erro.
 */
char **buffer_to_keyArray(char *keys_buf, int keys_buf_size)
{
	// Calcular quantidade de strings no buffer
	int num_strings = num_strings_in_buffer(keys_buf);

	//  Alocar memória para o array de ponteiros para strings
	char **keys_array = (char **)malloc(num_strings * sizeof(char *));

	// Preencher o array com os ponteiros para essas strings
	copy_strings_from_buffer_to_keys_array(keys_buf, keys_array, num_strings);

	return keys_array;
}

int num_strings_in_buffer(char *keys_buf)
{

	// ATENNCAO AO DESLAINDAMENTO DOS UMERO INTEIROS E MEORIA. LER 4 DE CADA VEZ.

	//  Inicializar num_strings a zero
	int num_strings = 0;
	//  Inicialiar strlength com o valor inicial que está no keys_buf
	// int a = *keys_buf | *(keys_buf + 1) << 8 | *(keys_buf + 2) << 16 | *(keys_buf + 3) << 24;
	int str_length = *keys_buf | *(keys_buf + 1) << 8 | *(keys_buf + 2) << 16 | *(keys_buf + 3) << 24;
	printf("str_length = %d\n", str_length);

	if (str_length != 0)
	{
		num_strings++;
	}
	else
	{
		return 0;
	}
	//  Enquanto strlength != 0
	while (str_length != 0)
	{
		//  Incrementar num_strings
		num_strings++;
		//  Ler tamanho da próxima string
		*keys_buf + str_length;
		printf("%d\n", *keys_buf);
		str_length = *keys_buf | *(keys_buf + 1) << 8 | *(keys_buf + 2) << 16 | *(keys_buf + 3) << 24;
	}
	// [][][][]
	//											0101 1111
	// |							0110 1110	0000 0000
	// |				1011 0000	0000 0000	0000 0000
	// |	1111 1000	0000 0000	0000 0000	0000 0000
	// 		1111 1000	1011 0000	0110 1110	0101 1111
	// int a = keys_buf + 3 << 24 | keys_buf + 2 << 16 | keys_buf + 1 << 8 | keys_buf;

	// Little Endian
	/**
	 * 100[0101 1111] <- LSB
	 * 101[0110 1110]
	 * 102[1011 0000]
	 * 103[1111 1000] <- MSB
	 */

	return num_strings;
}

void copy_strings_from_buffer_to_keys_array(char *keys_buf, char **keys_array, int num_strings)
{
	int str_length /* = *keys_buf | *(keys_buf + 1) << 8 | *(keys_buf + 2) << 16 | *(keys_buf + 3) << 24 */;
	int count;
	char *str_to_cpy;
	// Percorrer keys_buf,
	while (count <= num_strings)
	{
		//  Ler tamanho da próxima string
		str_length = *keys_buf | *(keys_buf + 1) << 8 | *(keys_buf + 2) << 16 | *(keys_buf + 3) << 24;
		// string alocada temporariamente
		char *str_to_cpy = malloc(str_length);
		for (int i = 0; i < str_length; i++)
		{
			// criar cópias das string pretendida
			strcat(str_to_cpy, keys_buf[i]);
		}
		// passar a frente a string que foi lida
		*keys_buf + str_length;
		// colocar os seus ponteiros no keys_array
		strcpy(keys_array[count], str_to_cpy);
		// aumentar contagem para preencher o proximo indice
		count++;
		// libertar memoria
		free(str_to_cpy);
	}
}

int calc_buffer_size(char **keys)
{
	int size = 0;
	for (int i = 0; keys[i] != NULL; i++)
	{
		size += 4 + strlen(keys[i]);
	}
	return size /* + 4 */;
}

void create_buffer(char **keys, char *keys_buf)
{
	// create_buffer
	// Preencher o buffer com as strings
	for (int i = 0; keys[i] != NULL; i++)
	{
		int strlength = strlen(keys[i]);
		memcpy(*keys_buf, &strlength, sizeof(int));
		*keys_buf += sizeof(int);
		memcpy(*keys_buf, keys[i], strlength);
		*keys_buf += strlength;
	}

	// e meter 0 final (create_buffer)
	int f = 0;
	memcpy(*keys_buf, &f, sizeof(int));
}
