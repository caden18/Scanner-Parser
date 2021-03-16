#include <string.h>

#include "Str.h"
#include "Vec.h"

static char NULL_CHAR = '\0';

Str Str_value(size_t capacity)
{
    Str s = Vec_value(capacity + 1, sizeof(char));
    // TODO: Replace the below lines with a call below to Vec_set
    // once you have Vec_set correctly implemented
    Vec_set(&s, 0, &NULL_CHAR);
    return s;
}

void Str_drop(Str *self)
{
    Vec_drop(self);
}

size_t Str_length(const Str *self)
{
    return Vec_length(self) - 1;
}

const char* Str_cstr(const Str *self)
{
    return (char*) Vec_ref(self, 0);
}

char* Str_ref(const Str *self, const size_t index)
{
    return (char*) Vec_ref(self, index);
}

Str Str_from(const char *cstr) 
{

    size_t i = 0;
    size_t capacity = 0;
    while (cstr[i] != NULL_CHAR)
    {
        ++capacity;
        ++i;
    }
    Str s = Str_value(capacity);
    s.length = i + 1;
    memcpy(s.buffer, cstr, sizeof(char) * s.capacity);
    return s;
}

void Str_splice(
        Str *self, 
        size_t index,
        size_t delete_count, 
        const char* cstr, 
        size_t insert_count)
{
    Vec_splice(self, index, delete_count, (void*) cstr, insert_count);
}

void Str_append(Str *self, const char *cstr)
{
    size_t capacity = 0;
    size_t i = 0;
    while (cstr[i] != NULL_CHAR) {
        ++capacity;
        ++i;
    }
    Str_splice(self, Str_length(self), (size_t)0, cstr, capacity);
}

char Str_get(const Str *self, size_t index)
{
    char value = NULL_CHAR;
    if (index == self->length) {
        return '\0';
    }
    void *out = &value;
    Vec_get(self, index, out);
    return value;
    
}

void Str_set(Str *self, size_t index, const char value) 
{
    const void *out = &value;
    Vec_set(self, index, out);
}
