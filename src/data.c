#include <data.h>
#include <stdio.h>
#include <stdlib.h>

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
		//unable to reserve that amount of memory
	if(size <= 0){ 
		return NULL;
	}
	
	struct data_t *data_st = (struct data_t*)malloc(sizeof(struct data_t));

	//error on init
	if(data == NULL){
		return NULL;
	}

	data_st->data = malloc(size);
	data_st->datasize = size;
	
	//error on memory reserve
	if(data_st->data == NULL){
		free(data_st);
		return NULL;
	}

	//copy mem to data
	memcpy(data, data_st, size);
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
	//clean prev data
	volatile char *ucptr = data;
    if(ucptr) while(data->datasize--) *ucptr++ = 0;
    asm("");
    free(data);

	//put new data
	data->datasize = new_size;
	data->data = malloc(new_size);

	if(data->data == NULL){
		free(data);
		return NULL;
	}

	memcpy(data, new_data, new_size);
}

void print(){
	printf("%d", 3);
}

int main(int argc, char const *argv[])
{
	struct data_t *data = data_create(0);
	printf("%p", data->data);
	return 0;
}

