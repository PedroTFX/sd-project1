#include <data.h>
#include <entry.h>
#include <stdio.h>
#include <stdlib.h>

/* Função que cria uma entry, reservando a memória necessária para a 
 * estrutura e inicializando os campos key e value, respetivamente, com a 
 * string e o bloco de dados passados como parâmetros, sem reservar 
 * memória para estes campos. 
 
 */ 
struct entry_t *entry_create(char *key, struct data_t *data){
    struct entry_t *entry = (struct entry_t*)malloc(sizeof (struct entry_t));

    if(entry == NULL){
        return NULL;
    }

    entry->key = key;
    entry->value = data;

    return entry;
} 
 
/* Função que elimina uma entry, libertando a memória por ela ocupada 
 */ 
void entry_destroy(struct entry_t *entry){
    //data_destroy(entry->value);   //double free
    free(entry);
} 
 
/* Função que duplica uma entry, reservando a memória necessária para a 
 * nova estrutura. 
 */ 
struct entry_t *entry_dup(struct entry_t *entry){
    return entry_create(entry->key, entry->value);
} 
 
/* Função que substitui o conteúdo de uma entrada entry_t. 
*  Deve assegurar que destroi o conteúdo antigo da mesma. 
*/ 
void entry_replace(struct entry_t *entry, char *new_key, struct data_t *new_value){
    data_destroy(entry->value);
    entry->key = new_key;
    entry->value = new_value;
} 
 
/* Função que compara duas entradas e retorna a ordem das mesmas. 
*  Ordem das entradas é definida pela ordem das suas chaves. 
*  A função devolve 0 se forem iguais, -1 se entry1<entry2, e 1 caso 
contrário. 
*/ 
int entry_compare(struct entry_t *entry1, struct entry_t *entry2){
    return (entry1->key == entry2->key) ? 0 : (entry1->key < entry2->key) ? -1 : 1;
}

int i = 0;

void print(struct entry_t *entry){
    printf("entry key: %s\n",entry->key);
    printf("value data (pointer): %p\n",entry->value->data);
    printf("value datasize: %d\n",entry->value->datasize);
}

int main(int argc, char const *argv[])
{
    struct data_t *data = data_create(4);
    struct entry_t *entry = entry_create("2", data);
    print(entry);

    //struct data_t *data2 = data_create(4);
    struct entry_t *entry2 = entry_dup(entry); // works
    print(entry2);

    printf("compare data: %d\n",entry_compare(entry, entry2));//doesnt work
    
    entry_replace(entry, "3", entry2->value); //works
    print(entry);

    entry_destroy(entry); //works
    print(entry);
    
    return 0;
}
