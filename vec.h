#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

#define VEC_DEFAULT_CAPACITY 4
#define VEC_RESIZE_FACTOR 2

typedef struct Vec {
    char* data;
    size_t element_size; // bytes per element
    size_t len; // number of elements rather than bytes
    size_t capacity; // number of elements rather than bytes
} Vec;

Vec vec_new(size_t element_size) {
    void* data = malloc(element_size * VEC_DEFAULT_CAPACITY );
    Vec vec = { .data = data, .element_size = element_size, .len = 0, .capacity = VEC_DEFAULT_CAPACITY };
    return vec;
}

Vec vec_new_with_capacity(size_t element_size, size_t capacity) {
    void* data = malloc(element_size * capacity);
    Vec vec = { .data = data, .element_size = element_size, .len = 0, .capacity = capacity };
    return vec;
}

void vec_grow(Vec* vec, size_t minimum_size) {
    int new_capacity = vec->capacity; 
    while (new_capacity <= minimum_size) {
        new_capacity *= VEC_RESIZE_FACTOR; 
    }
    if ((vec->data = realloc(vec->data, new_capacity * vec->element_size)) == NULL) {
        printf("buy more RAM lol\n"); 
        exit(1);
    } 
    vec->capacity = new_capacity;
}

void vec_shrink(Vec* vec, size_t maximum_size) {
    int new_capacity = vec->capacity;
    while (new_capacity > maximum_size && new_capacity > 1) {
        new_capacity /= VEC_RESIZE_FACTOR;
    }
    if ((vec->data = realloc(vec->data, new_capacity * vec->element_size)) == NULL) {
        printf("buy more RAM lol\n"); 
        exit(1);
    } 
    vec->capacity = new_capacity;
}

void vec_push(Vec* vec, void* element) {
    if (vec->len == vec->capacity) {
        vec_grow(vec, vec->len + 1);
    }
    /* TODO: btw there are no error values reserved for memcpy so this [checking if NULL] is retarded */
    if (memcpy(
        vec->data + vec->len * vec->element_size, 
        element,
        vec->element_size
    ) == NULL) {
        printf("hellow\n");
        exit(1);
    }

    vec->len += 1;
}

void vec_pushi(Vec* vec, int element) {
    if (vec->element_size != sizeof(int)) {
        printf("Vec Error: wrong size\n");
        exit(1);
    }
    if (vec->len == vec->capacity) {
        vec_grow(vec, vec->len + 1);
    }
    vec->data[vec->len] = element;
    vec->len += 1;
}

void vec_pop(Vec* vec) {
    if (vec->len > 0) {
        vec->len -= 1;
        if (vec->len <= vec->capacity / VEC_RESIZE_FACTOR) {
            vec_shrink(vec, vec->len);
        }
    } 
} 

void* vec_get(Vec* vec, size_t index) {
    if (index >= 0 && index < vec->len) {
        return vec->data + index * vec->element_size;
    }
    return NULL;
    // look you really should check if the result is NULL unless you are absolutely sure your index is within [0, len). Like pretend it's a Option<T>
}

int vec_insert_many(Vec* vec, size_t index, void* elements, size_t num_elements) {
    if (index >= 0 && index <= vec->len && num_elements > 0) {
        if (vec->len + num_elements > vec->capacity) {
            vec_grow(vec, vec->len + num_elements);    
        }
        // make space for new elements
        if (memmove(
            vec->data + (index + num_elements) * vec->element_size,
            vec->data + index * vec->element_size,
            (vec->len - index) * vec->element_size
        ) == NULL) {
            printf("errm what the skibidy\n");
            exit(1);
        } 
        // copy in new elements
        if (memcpy(
            vec->data + index * vec->element_size,
            elements, 
            num_elements * vec->element_size
        ) == NULL) {
            printf("errm what the skibidy\n");
            exit(1);
        } 
        vec->len += num_elements;
        return 0;
    }
    return 1;
}

int vec_insert(Vec* vec, size_t index, void* element) {
    return vec_insert_many(vec, index, element, 1);
}

void vec_remove_many(Vec* vec, size_t index, size_t num_elements) {
    if (num_elements >= 0 && index >= 0 && index + num_elements <= vec->len) {
        if (memmove(
            vec->data + index * vec->element_size,
            vec->data + (index + num_elements) * vec->element_size,
            (vec->len - index) * vec->element_size
        ) == NULL) {
            printf("errm what the skibidy\n");
            exit(1);
        }
        vec->len -= num_elements;
    } 
}

void vec_remove(Vec* vec, size_t index) {
    vec_remove_many(vec, index, 1);
}

#endif

// int main() {
//     Vec vec = vec_new(sizeof(int));
//     int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//     vec_insert_many(&vec, 0, &a, 10);
//     printf("Vector\n");
//     for (int i = 0; i < vec.len; i++) {
//         printf("%d\n", *(int *) vec_get(&vec, i));
//     }
//     vec_remove_many(&vec, 3, 2);
//     vec_remove(&vec, 5);
// 
//     printf("Vector after Removing\n");
//     for (int i = 0; i < vec.len; i++) {
//         printf("%d\n", *(int *) vec_get(&vec, i));
//     }
// }

// int main() {
//     Vec vec = vec_new(sizeof(int));
//     int a[10] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
//     printf("Push\n");
//     for (int i = 0; i < 10; i++) {
//         vec_push(&vec, &a[i]);
//         printf("len: %ld, capacity: %ld\n", vec.len, vec.capacity);
//     }
//     printf("Pop\n");
//     while (vec.len > 0) {
//         vec_pop(&vec);
//         printf("len: %ld, capacity: %ld\n", vec.len, vec.capacity);
//     }
// }

// int main() {
//     Vec vec = vec_new_with_capacity(sizeof(int), 4);
//     
//     int a[10] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
//     int b = 0;
//     vec_insert(&vec, 0, &b);
//     vec_insert_many(&vec, 1, &a, 10);
//     for (int i = 0; i < vec.len; i++) {
//         printf("%d\n", *(int *) vec_get(&vec, i));
//     }
// }


// int main() {
//     Vec vec = vec_new_with_capacity(sizeof(int), 4);
//     int a = 10;
//     int b = 21;
//     int c = 314;
// 
//     int d[3] = {-c, -b, -a};
// 
//     vec_push(&vec, &a);
//     vec_push(&vec, &b);
//     vec_push(&vec, &b);
//     vec_push(&vec, &b);
//     vec_insert(&vec, 2, &c);
//     vec_insert_many(&vec, 0, &d, 3);
//     vec_push(&vec, &a);
// 
//     for (int i = 0; i < vec.len; i++) {
//         printf("%d\n", *(int *) vec_get(&vec, i));
//     }
// 
//     return 0;
// }


