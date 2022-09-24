#include <data.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Função que cria um novo elemento de dados data_t, reservando a memória
 * necessária para armazenar os dados, especificada pelo parâmetro size
 */
struct data_t *data_create(int size){
	
	struct data_t *data = (struct data_t*)malloc(sizeof(struct data_t));

	//error on init
	if(data == NULL){
		return NULL;
	}

	data->data = malloc(size);
	data->datasize = size;
	
	//error on memory reserve
	if(data->data == NULL){
		free(data);
		return NULL;
	}

	return data;
}

/* Função que cria um novo elemento de dados data_t, inicializando o campo
 * data com o valor passado no parâmetro data, sem necessidade de reservar
 * memória para os dados.
 */
struct data_t *data_create2(int size, void *data){
	struct data_t *data_st = (struct data_t*)malloc(sizeof(struct data_t));
	data_st->data = data;
	data_st->datasize = size;
	return data_st;
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
	return data_create2(data->datasize, data->data);
}

/* Função que substitui o conteúdo de um elemento de dados data_t.5
 *  Deve assegurar que destroi o conteúdo antigo do mesmo.
 */
void data_replace(struct data_t *data, int new_size, void *new_data){
	//clean prev data->data
    free(data->data);

	//put new data
	data->datasize = new_size;
	data->data = new_data;

	if(data->data == NULL){
		free(data);
		return; // a func é um void?? n se rpepara erros?
	}
}
/*
int main(int argc, char const *argv[])
{
	//test data_create
	struct data_t *data = data_create(0);
	printf("datasize: %d\n", data->datasize);
	printf("data(pointer): %p\n", data->data);
	
	//test data_create2
	struct data_t *data1 = data_create2(4, data);
	printf("datasize: %d\n", data1->datasize);
	printf("data(pointer): %p\n", data1->data);
	
	//test data_replace
	data_replace(data, data1->datasize, data1);
	printf("datasize: %d\n", data->datasize);
	printf("data(pointer): %p\n", data->data);
	
	//test data_duplicate
	struct data_t *data2 = data_dup(data1);
	printf("datasize: %d\n", data2->datasize);
	printf("data(pointer): %p\n", data2->data);
	
	//test data_destroy
	data_destroy(data2);
	printf("datasize: %d\n", data2->datasize);
	printf("data(pointer): %p\n", data2->data);
	
	return 0;
}
*/
