; /** defines bool y puntero **/
%define NULL 0
%define TRUE 1
%define FALSE 0

section .data

section .text

global string_proc_list_create_asm
global string_proc_node_create_asm
global string_proc_list_add_node_asm
global string_proc_list_concat_asm

; FUNCIONES auxiliares que pueden llegar a necesitar:
extern malloc
extern free
extern str_concat
extern strdup        


; Tamaños en bytes para estructuras (64-bit pointers)
%define SIZE_LIST 16      ; string_proc_list: 2 punteros (first, last)
%define SIZE_NODE 32      ; string_proc_node: next(8)+prev(8)+type(1)+padding(7)+hash(8)



; =============================================================================
; string_proc_list_create_asm:
;   Crea una lista vacía (string_proc_list) en heap.
;   - Asigna SIZE_LIST bytes 
;   - Inicializa first y last a NULL (0)
; Parámetros: ninguno
; Retorno: RAX = puntero a lista o NULL en fallo
; =============================================================================
string_proc_list_create_asm:
    push    rbp
    mov     rbp, rsp

    mov     rdi, SIZE_LIST       ; tamaño a malloc
    call    malloc               ; RAX = nueva lista

    test    rax, rax
    jz      .list_end            ; si NULL, retornamos NULL

    mov     qword [rax], NULL    ; list->first = NULL
    mov     qword [rax+8], NULL  ; list->last  = NULL

.list_end:
    pop     rbp
    ret



; =============================================================================
; string_proc_node_create_asm:
;   Crea un nodo (string_proc_node) en heap.
;   - Asigna SIZE_NODE bytes:
;       next(0), previous(8), type at offset 16, padding hasta 24, hash(24)
;   - Inicializa next y previous a NULL
;   - Guarda el campo type (dil) y el puntero hash (rsi)
; Parámetros:
;   RDI: type (uint8_t)
;   RSI: hash (char*)
; Retorno: RAX = puntero a nodo o NULL en fallo
; =============================================================================
string_proc_node_create_asm:
    push    rbp
    mov     rbp, rsp
    push    r12       ; preserva R12 y R13 (registros no volátiles)
    push    r13

    ; Guardar parámetros en callee-saved
    movzx   r12d, dil  ; R12b = type (uint8_t)
    mov     r13, rsi   ; R13  = hash (char*)

    ; Reservar memoria para el nodo
    mov     rdi, SIZE_NODE  ; bytes a malloc
    call    malloc          ; RAX = nuevo nodo

    test    rax, rax
    je      .node_end       ; si NULL, sale con RAX=0

    ; Inicializar campos
    mov     qword [rax], NULL      ; node->next = NULL
    mov     qword [rax+8], NULL    ; node->previous = NULL
    mov     byte  [rax+16], r12b    ; node->type = type
    ; padding opcional, no necesario
    mov     qword [rax+24], r13     ; node->hash = hash pointer

.node_end:
    pop     r13
    pop     r12
    pop     rbp
    ret



; =============================================================================
; string_proc_list_add_node_asm:
;   Agrega un nodo al final de la lista.
; Parámetros:
;   RDI: lista (string_proc_list*)
;   RSI: type (uint8_t)
;   RDX: hash (char*)
; Retorno: ninguno
; =============================================================================
string_proc_list_add_node_asm:
    push    rbp
    mov     rbp, rsp
    push    rbx
    push    r12
    push    r13
    push    r14

    mov     rbx, rdi       ; RBX = lista
    movzx   r12d, sil      ; R12b = type
    mov     r13, rdx       ; R13 = hash pointer

    ; Crear nuevo nodo: args en RDI/RSI
    mov     edi, r12d      ; edi = type
    mov     rsi, r13       ; rsi = hash
    call    string_proc_node_create_asm

    test    rax, rax
    jz      .add_done      ; si falla, salir sin tocar lista
    mov     r14, rax       ; R14 = new_node

    ; Si lista vacía (first==NULL)
    mov     rax, [rbx]     ; rax = list->first
    test    rax, rax
    jnz     .add_nonempty

    mov     [rbx], r14     ; list->first = new_node
    mov     [rbx+8], r14   ; list->last  = new_node
    jmp     .add_done

.add_nonempty:
    mov     rax, [rbx+8]   ; rax = list->last
    mov     [r14+8], rax   ; new_node->previous = last
    mov     [rax], r14     ; last->next = new_node
    mov     [rbx+8], r14   ; list->last = new_node

.add_done:
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    ret



; =============================================================================
; string_proc_list_concat_asm:
;   Concatena el hash inicial con los hashes de nodos de tipo dado.
;   Se usa strdup para copia inicial y str_concat para cada concatenación.
; Parámetros:
;   RDI: lista (string_proc_list*)
;   RSI: type (uint8_t)
;   RDX: hash inicial (char*)
; Retorno: RAX = puntero a cadena resultante (heap) o NULL en fallo
; =============================================================================
string_proc_list_concat_asm:
    push    rbp
    mov     rbp, rsp
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15

    mov     rbx, rdi       ; RBX = lista
    movzx   r12d, sil      ; R12b = type
    mov     r13, rdx       ; R13 = hash inicial

    ; Copia inicial: strdup(hash)
    mov     rdi, r13
    call    strdup         ; RAX = nueva copia en heap
    test    rax, rax
    jz      .cleanup       ; si falla strdup, return NULL
    mov     r14, rax       ; R14 = acumulado

    ; Si lista NULL, saltar directamente a retorno
    test    rbx, rbx
    jz      .done

    mov     r15, [rbx]     ; R15 = primer nodo
.concat_loop:
    test    r15, r15
    jz      .done

    ; si node->type == type buscado
    movzx   eax, byte [r15+16]
    cmp     al, r12b
    jne     .next

    ; concatenar acumulado + node->hash
    mov     rdi, r14       ; string antigua
    mov     rsi, [r15+24]  ; string del nodo
    call    str_concat     ; RAX = nueva cadena
    test    rax, rax
    jz      .error         ; en fallo, liberamos y NULL

    ; free del antiguo acumulado
    mov     rdi, r14
    mov     r14, rax       ; R14 = nuevo acumulado
    call    free

.next:
    mov     r15, [r15]     ; siguiente nodo
    jmp     .concat_loop

.error:
    ; liberar acumulado y preparar NULL
    mov     rdi, r14
    call    free
    xor     r14, r14       ; R14 = NULL

.done:
    mov     rax, r14       ; retorno
.cleanup:
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    ret
