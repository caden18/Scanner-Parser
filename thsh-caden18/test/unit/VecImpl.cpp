#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

/*
 * Unit tests established during the implementation of Vec.c
 * These tests are permitted to reach through the abstraction
 * and access members of a Vec struct directly.
 */

TEST(VecImpl, value) {
    Vec v = Vec_value(2, sizeof(int16_t));
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(2, v.capacity);
    ASSERT_STREQ((char*)v.buffer, "");
    Vec_drop(&v);
}

TEST(VecImpl, drop) {
    Vec v = Vec_value(2, sizeof(int16_t));
    Vec_drop(&v);
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(0, v.capacity);
    ASSERT_EQ(nullptr, v.buffer);
}

TEST(VecImpl, ref) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    v.length = 2;
    ASSERT_EQ(&buffer[0], Vec_ref(&v, 0));
    ASSERT_EQ(&buffer[1], Vec_ref(&v, 1));
    Vec_drop(&v);
}

TEST(VecImpl, ref_out_of_bounds) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    ASSERT_DEATH({
       Vec_ref(&v, 3);
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, get) {
    Vec v = Vec_value(1, sizeof(int16_t));
    int16_t *buffer = (int16_t*)v.buffer;
    buffer[0] = 1;
    v.length = 1;

    int16_t value = 0;
    Vec_get(&v, 0, &value);
    ASSERT_EQ(1, value);
}

TEST(VecImpl, get_out_of_bounds) {
    Vec v = Vec_value(3, sizeof(int64_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;

    int16_t value = 0;
    ASSERT_DEATH({
            Vec_get(&v, 3, &value);
            }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, set) {
    Vec v = Vec_value(1, sizeof(int16_t));

    int16_t x = 1;
    int16_t *buffer = (int16_t*) v.buffer;

    buffer[0] = 0;
    v.length = 1;
    Vec_set(&v, 0, &x);

    ASSERT_EQ(1, v.length);
    ASSERT_EQ(x, buffer[0]);
    Vec_drop(&v);

}

TEST(VecImpl, set_length) {
    Vec v = Vec_value(2, sizeof(int16_t));

    int16_t x = 1;
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    v.length = 1;
    Vec_set(&v, 1, &x);
    v.length = 2;
    ASSERT_EQ(2, v.length);

    ASSERT_EQ(x, buffer[1]);
    Vec_drop(&v);
}

TEST(VecImpl, set_out_of_bounds) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;

    int16_t value = 0;
    ASSERT_DEATH({
            Vec_set(&v, 4, &value);
            }, ".* - Out of Bounds");
    Vec_drop(&v);
}


TEST(VecImpl, equal) {
    Vec v_1 = Vec_value(1, sizeof(int16_t));
    int16_t *buffer_1 = (int16_t*) v_1.buffer;
    Vec v_2 = Vec_value(1, sizeof(int16_t));
    int16_t *buffer_2 = (int16_t*) v_2.buffer;
    bool x = true;
    bool y = false;
    buffer_1[0] = 1;
    v_1.length = 1;
    buffer_2[0] = 1;
    v_2.length = 1;

    x = Vec_equals(&v_1, &v_2);
    y = Vec_equals(&v_2, &v_1);
    ASSERT_EQ(x, y);
    ASSERT_EQ(v_1.length, v_2.length);
    Vec_drop(&v_1);
    Vec_drop(&v_2);
}

TEST(VecImpl, equal_1) {
   
    Vec v_1 = Vec_value(1, sizeof(int16_t));
    int16_t *buffer_1 = (int16_t*) v_1.buffer;
    Vec v_2 = Vec_value(1, sizeof(int16_t));
    int16_t *buffer_2 = (int16_t*) v_2.buffer;
    bool x = true;
    bool y = false;
    buffer_1[0] = 1;
    v_1.length = 1;
    buffer_2[0] = 1;
    v_2.length = 1;

    x = Vec_equals(&v_1, &v_2);
    y = Vec_equals(&v_2, &v_1);
    ASSERT_EQ(x, y);
    ASSERT_EQ(v_1.length, v_2.length);
    Vec_drop(&v_1);
    Vec_drop(&v_2);
}

TEST(VecImpl, not_equal_1) {
    Vec v_1 = Vec_value(2, sizeof(int16_t));
    int16_t *buffer_1 = (int16_t*) v_1.buffer;
    Vec v_2 = Vec_value(1, sizeof(int16_t));
    int16_t *buffer_2 = (int16_t*) v_2.buffer;
    bool x = true;
    bool y = true;
    buffer_1[0] = 1;
    buffer_1[1] = 3;
    v_1.length = 2;
    buffer_2[0] = 2;
    v_2.length = 1;

    x = Vec_equals(&v_1, &v_2);
    y = Vec_equals(&v_2, &v_1);
    ASSERT_EQ(false, x);
    ASSERT_EQ(false, y);
    ASSERT_EQ(2, v_1.length);
    ASSERT_EQ(1, v_2.length);
    Vec_drop(&v_1);
    Vec_drop(&v_2);
}

TEST(VecImpl, not_equal_2) {
    Vec v_1 = Vec_value(2, sizeof(int16_t));
    int16_t *buffer_1 = (int16_t*) v_1.buffer;
    Vec v_2 = Vec_value(1, sizeof(int16_t));
    int16_t *buffer_2 = (int16_t*) v_2.buffer;
    bool x = true;
    bool y = true;
    buffer_1[0] = 1;
    buffer_1[1] = 3;
    v_1.length = 2;
    buffer_2[0] = 1;
    buffer_2[1] = 3;
    buffer_2[2] = 5;
    v_2.length = 3;

    x = Vec_equals(&v_1, &v_2);
    y = Vec_equals(&v_2, &v_1);
    ASSERT_EQ(false, x);
    ASSERT_EQ(false, y);
    ASSERT_EQ(2, v_1.length);
    ASSERT_EQ(3, v_2.length);
    Vec_drop(&v_1);
    Vec_drop(&v_2);
}

TEST(VecImpl, splice_just_insert) {
    Vec v = Vec_value(6, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    buffer[2] = 3;
    buffer[3] = 4;
    v.length = 4;

    size_t delete_count = 0;
    size_t insert_count = 2;

    int16_t *items = (int16_t*) malloc(2 * sizeof(int16_t));
    items[0] = 8;
    items[1] = 9;

    Vec v_out = Vec_value(6, sizeof(int16_t));
    int16_t *result = (int16_t*) v_out.buffer;
    result[0] = 1;
    result[1] = 2;
    result[2] = 8;
    result[3] = 9;
    result[4] = 3;
    result[5] = 4;
    v_out.length = 6;

    Vec_splice(&v, 2, delete_count, items, insert_count);
    buffer = (int16_t*)v.buffer;
    ASSERT_EQ(buffer[0], result[0]);
    ASSERT_EQ(buffer[1], result[1]);
    ASSERT_EQ(buffer[2], result[2]);
    ASSERT_EQ(buffer[3], result[3]);
    ASSERT_EQ(buffer[4], result[4]);
    ASSERT_EQ(buffer[5], result[5]);
    ASSERT_EQ(v.length, v_out.length);
    Vec_drop(&v);
    Vec_drop(&v_out);
}

TEST(VecImpl, splice_just_delete) {
    Vec v = Vec_value(6, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    buffer[2] = 3;
    buffer[3] = 4;
    v.length = 4;

    size_t delete_count = 1;
    size_t insert_count = 0;

    int16_t *items = (int16_t*) malloc(2 * sizeof(int16_t));
    items[0] = 8;
    items[1] = 9;

    Vec v_out = Vec_value(6, sizeof(int16_t));
    int16_t *result = (int16_t*) v_out.buffer;
    result[0] = 1;
    result[1] = 2;
    result[2] = 4;
    v_out.length = 3;

    Vec_splice(&v, 2, delete_count, items, insert_count);
    buffer = (int16_t*)v.buffer;

    for (size_t i = 0; i < v.length; ++i) {
        ASSERT_EQ(buffer[i], result[i]);
    }
    ASSERT_EQ(v.length, v_out.length);
    Vec_drop(&v);
    Vec_drop(&v_out);
}

TEST(VecImpl, splice_insert_and_delete) {
    Vec v = Vec_value(6, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    buffer[2] = 3;
    buffer[3] = 4;
    v.length = 4;

    size_t delete_count = 1;
    size_t insert_count = 2;

    int16_t *items = (int16_t*) malloc(2 * sizeof(int16_t));
    items[0] = 8;
    items[1] = 9;

    Vec v_out = Vec_value(6, sizeof(int16_t));
    int16_t *result = (int16_t*) v_out.buffer;
    result[0] = 1;
    result[1] = 2;
    result[2] = 8;
    result[3] = 9;
    result[4] = 4;
    v_out.length = 5;

    Vec_splice(&v, 2, delete_count, items, insert_count);
    buffer = (int16_t*)v.buffer;
    ASSERT_EQ(buffer[0], result[0]);
    ASSERT_EQ(buffer[1], result[1]);
    ASSERT_EQ(buffer[2], result[2]);
    ASSERT_EQ(buffer[3], result[3]);
    ASSERT_EQ(buffer[4], result[4]);
    ASSERT_EQ(v.length, v_out.length);
    Vec_drop(&v);
    Vec_drop(&v_out);
}

TEST(VecImpl, splice_at_length) {
    Vec v = Vec_value(6, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    buffer[2] = 3;
    buffer[3] = 4;
    v.length = 4;

    size_t delete_count = 0;
    size_t insert_count = 2;

    int16_t *items = (int16_t*) malloc(2 * sizeof(int16_t));
    items[0] = 8;
    items[1] = 9;

    Vec v_out = Vec_value(6, sizeof(int16_t));
    int16_t *result = (int16_t*) v_out.buffer;
    result[0] = 1;
    result[1] = 2;
    result[2] = 3;
    result[3] = 4;
    result[4] = 8;
    result[5] = 9;
    v_out.length = 6;

    Vec_splice(&v, v.length, delete_count, items, insert_count);
    buffer = (int16_t*)v.buffer;

    ASSERT_EQ(buffer[0], result[0]);
    ASSERT_EQ(buffer[1], result[1]);
    ASSERT_EQ(buffer[2], result[2]);
    ASSERT_EQ(buffer[3], result[3]);
    ASSERT_EQ(buffer[4], result[4]);
    ASSERT_EQ(buffer[5], result[5]);
    ASSERT_EQ(v.length, v_out.length);

    Vec_drop(&v);
    Vec_drop(&v_out);
}

TEST(VecImpl, splice_delete_all) {
    Vec v = Vec_value(6, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    buffer[2] = 3;
    buffer[3] = 4;
    v.length = 4;

    size_t delete_count = 4;
    size_t insert_count = 2;

    int16_t *items = (int16_t*) malloc(2 * sizeof(int16_t));
    items[0] = 8;
    items[1] = 9;

    Vec v_out = Vec_value(6, sizeof(int16_t));
    int16_t *result = (int16_t*) v_out.buffer;
    result[0] = 8;
    result[1] = 9;
    v_out.length = 2;

    Vec_splice(&v, 0, delete_count, items, insert_count);
    buffer = (int16_t*)v.buffer;

    ASSERT_EQ(buffer[0], result[0]);
    ASSERT_EQ(buffer[1], result[1]);
    ASSERT_EQ(v.length, v_out.length);

    Vec_drop(&v);
    Vec_drop(&v_out);
}


