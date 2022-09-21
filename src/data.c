#include "data.h"
#include <stdio.h>
#include <stdlib.h>
/* Função que cria um novo elemento de dados data_t, reservando a memória
 * necessária para armazenar os dados, especificada pelo parâmetro size
 */
struct data_t *data_create(int size){
	struct data_t * data = (struct data_t*)malloc(size);
	return data;
}

/* Função que cria um novo elemento de dados data_t, inicializando o campo
 * data com o valor passado no parâmetro data, sem necessidade de reservar
 * memória para os dados.
 */
struct data_t *data_create2(int size, void *data){
	struct data_t *data2 = (struct data_t *)malloc(size);
	data2-> data = data;
	return data2;
}

/* Função que elimina um bloco de dados, apontado pelo parâmetro data,
 * libertando toda a memória por ele ocupada.
 */
void data_destroy(struct data_t *data){
	free(data->data);
	free(data);
}

/* Função que duplica uma estrutura data_t, reservando toda a memória
 * necessária para a nova estrutura, inclusivamente dados.
 */
struct data_t *data_dup(struct data_t *data){				//for later review
	/*struct data_t *data2 = (struct data_t *)malloc(data->datasize);
	data2->data = data->data;
	return data2; */
}

/* Função que substitui o conteúdo de um elemento de dados data_t.
 *  Deve assegurar que destroi o conteúdo antigo do mesmo.
 */
void data_replace(struct data_t *data, int new_size, void *new_data){
	/*free(data->data);
	data = (struct data_t *)malloc(new_size);
	data->data = new_data;*/
}
