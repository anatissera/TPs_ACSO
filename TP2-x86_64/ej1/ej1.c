#include "ej1.h"


/**
 * Inicializa una estructura de lista. 
*/
string_proc_list* string_proc_list_create(void){
	// Asigna memoria para la lista
	string_proc_list* list = malloc(sizeof(string_proc_list));
	if (list == NULL) {
		// En caso de error en la asignación de memoria
		return NULL;
	}
	
	// Inicializa los punteros a NULL
	list->first = NULL;
	list->last = NULL;
	
	// Retorna la lista inicializada
	return list;
}


/**
 * Inicializa un nodo con el tipo y el hash dado. 
 * El nodo tiene que apuntar al hash pasado por parámetro (no hay que copiarlo).
*/
string_proc_node* string_proc_node_create(uint8_t type, char* hash){
	string_proc_node* node = malloc(sizeof(string_proc_node));
	if (node == NULL){
		return NULL;
	}

	// Inicializa el nodo con los valores dados
	node->type = type;
	node->hash = hash;      // No se copia el hash, solo se asigna la dirección.
	
	// Inicializa los punteros a NULL
	node->next = NULL;
	node->previous = NULL;
	
	return node;
}

/**
 * Agrega un nodo nuevo al final de la lista con el tipo y el hash dado.
*/
void string_proc_list_add_node(string_proc_list* list, uint8_t type, char* hash){
	// Verifica que la lista no sea NULL
    if (list == NULL) {
        return;
    }
    
    // Crea un nuevo nodo
    string_proc_node* new_node = string_proc_node_create(type, hash);
    if(new_node == NULL){
        return;
    }
    
    // Si la lista está vacía, el nuevo nodo se convierte en el primero y el último
    if (list->first == NULL) {
        list->first = new_node;
        list->last = new_node;
    } else {
        // Vincula el nuevo nodo al final de la lista
        new_node->previous = list->last;
        list->last->next = new_node;
        list->last = new_node;
    }
}

/**
 * Genera un nuevo hash concatenando el pasado por parámetro con todos los hashes 
   de los nodos de la lista cuyos tipos coinciden con el pasado por parámetro.
*/
char* string_proc_list_concat(string_proc_list* list, uint8_t type , char* hash){
	// Si la lista es NULL, retornamos una copia del hash inicial.
    if (list == NULL) {
        char* copy = malloc(strlen(hash) + 1);
        if(copy != NULL) {
            strcpy(copy, hash);
        }
        return copy;
    }
    
    // Inicializa el resultado con una copia del hash recibido.
    char* result = malloc(strlen(hash) + 1);
    if(result == NULL) {
        return NULL;
    }
    strcpy(result, hash);
    
    // Recorre la lista y concatena los hash de los nodos que cumplan la condición.
    string_proc_node* current = list->first;
    while(current != NULL) {
        if(current->type == type) {
            char* new_result = str_concat(result, current->hash);
            free(result); // Libera el buffer anterior para evitar fugas.
            result = new_result;
        }
        current = current->next;
    }
    
    return result;
}


/** AUX FUNCTIONS **/

void string_proc_list_destroy(string_proc_list* list){

	string_proc_node* current_node	= list->first;
	string_proc_node* next_node		= NULL;
	while(current_node != NULL){
		next_node = current_node->next;
		string_proc_node_destroy(current_node);
		current_node	= next_node;
	}
	/*borro la lista:*/
	list->first = NULL;
	list->last  = NULL;
	free(list);
}
void string_proc_node_destroy(string_proc_node* node){
	node->next      = NULL;
	node->previous	= NULL;
	node->hash		= NULL;
	node->type      = 0;			
	free(node);
}


char* str_concat(char* a, char* b) {
	int len1 = strlen(a);
    int len2 = strlen(b);
	int totalLength = len1 + len2;
    char *result = (char *)malloc(totalLength + 1); 
    strcpy(result, a);
    strcat(result, b);
    return result;  
}

void string_proc_list_print(string_proc_list* list, FILE* file){
        uint32_t length = 0;
        string_proc_node* current_node  = list->first;
        while(current_node != NULL){
                length++;
                current_node = current_node->next;
        }
        fprintf( file, "List length: %d\n", length );
		current_node    = list->first;
        while(current_node != NULL){
                fprintf(file, "\tnode hash: %s | type: %d\n", current_node->hash, current_node->type);
                current_node = current_node->next;
        }
}

