#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Guards.h"

#include "Vec.h"

/* Constructor / Destructor */

Vec Vec_value(size_t capacity, size_t item_size)
{
    Vec vec = {
        item_size,
        0,
        capacity,
        calloc(capacity, item_size)
    };
    OOM_GUARD(vec.buffer, __FILE__, __LINE__);
    return vec;
}

void Vec_drop(Vec *self)
{
    free(self->buffer);
    self->buffer = NULL;
    self->capacity = 0;
    self->length = 0;
}

/* Accessors */

size_t Vec_length(const Vec *self)
{
    return self->length;
}

void* Vec_ref(const Vec *self, size_t index)
{
    if (index < self->length) {
        return self->buffer + (index * self->item_size);
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void Vec_get(const Vec *self, size_t index, void *out)
{
    memcpy(out, Vec_ref(self, index), self->item_size);
}

void Vec_set(Vec *self, size_t index, const void *value)
{
    ensure_capacity(self, index + 1);
    if (index > self->length) {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    if (index == self->length) {
        Vec_splice(self, index, 0, value, 1);
        return;
    }
    Vec_splice(self, index, 1, value, 1);
}

bool Vec_equals(const Vec *self, const Vec *other)
{
    if (self->length != other->length) {
        return false;
       }
    for (size_t i = 0; i < self->length; ++i) {
        if (memcmp(&self->buffer[i], &other->buffer[i], self->item_size)) {
            return false;
        }
    }
    return true;
}

void Vec_splice(Vec *self, size_t index, size_t delete_count, const void *items, size_t insert_count)
{
    ensure_capacity(self, self->length + (insert_count - delete_count));
    memcpy(self->buffer + (index + insert_count) * self->item_size, self->buffer + (index + delete_count) * self->item_size, self->item_size * (self->length - index - delete_count));
    memcpy(self->buffer + (index * self->item_size), items, self->item_size * insert_count);
    self->length += insert_count - delete_count;
}

static void ensure_capacity(Vec *self, size_t n)
{
    if (n > self->capacity) {
        self->buffer = realloc(self->buffer, n * self->item_size);
    }

}
