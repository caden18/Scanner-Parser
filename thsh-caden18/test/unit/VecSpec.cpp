#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

/**
 * The purpose of these tests is to prove correctness of the
 * abstraction barrier from the user's point-of-view. You should
 * not access any member of the Vec directly.
 */

TEST(VecSpec, values_init_empty) {
    Vec v = Vec_value(10, sizeof(double));
    ASSERT_EQ(0, Vec_length(&v));
    Vec_drop(&v);
}

TEST(VecSpec, get_set_contract) {
    Vec v = Vec_value(1, sizeof(int16_t));
    int16_t x = 100;
    int16_t y = 200;
    ASSERT_EQ(0, Vec_length(&v));
    Vec_set(&v, 0, &x);
    ASSERT_EQ(1, Vec_length(&v));
    Vec_set(&v, 1, &y);
    ASSERT_EQ(2, Vec_length(&v));

    int16_t x_out = 0;
    int16_t y_out = 0;
    Vec_get(&v, 0, &x_out);
    ASSERT_EQ(x, x_out);
    Vec_get(&v, 1, &y_out);
    ASSERT_EQ(y, y_out);
    Vec_drop(&v);
}

TEST(VecSpec, set_equals_contract) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t x = 100;
    int16_t y = 200;
    ASSERT_EQ(0, Vec_length(&v));
    Vec_set(&v, 0, &x);
    ASSERT_EQ(1, Vec_length(&v));
    Vec_set(&v, 1, &y);
    ASSERT_EQ(2, Vec_length(&v));

    Vec w = Vec_value(2, sizeof(int16_t));
    ASSERT_EQ(0, Vec_length(&w));
    Vec_set(&w, 0, &x);
    ASSERT_EQ(1, Vec_length(&w));
    Vec_set(&w, 1, &y);
    ASSERT_EQ(2, Vec_length(&w));

    bool value = false;
    value = Vec_equals(&v, &w);
    ASSERT_EQ(true, value);
    Vec_drop(&v);
    Vec_drop(&w);
}

TEST(VecSpec, splice_get_contract) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t a = 1;
    int16_t b = 2;
    int16_t c = 3;
    int16_t d = 4;
    ASSERT_EQ(0, Vec_length(&v));
    Vec_set(&v, 0, &a);
    ASSERT_EQ(1, Vec_length(&v));
    Vec_set(&v, 1, &b);
    ASSERT_EQ(2, Vec_length(&v));
    Vec_set(&v, 2, &c);
    ASSERT_EQ(3, Vec_length(&v));
    Vec_set(&v, 3, &d);
    ASSERT_EQ(4, Vec_length(&v));

    int16_t *items = (int16_t*) malloc(1 * sizeof(int16_t));
    items[0] = 8;

    Vec_splice(&v,(size_t) 2, (size_t)0, items, (size_t)1); 
    int16_t x_out = 0;
    Vec_get(&v, 2, &x_out);
    ASSERT_EQ(8, x_out);

}

TEST(VecSpec, splice_get_death) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t a = 1;
    int16_t b = 2;
    int16_t c = 3;
    int16_t d = 4;
    ASSERT_EQ(0, Vec_length(&v));
    Vec_set(&v, 0, &a);
    ASSERT_EQ(1, Vec_length(&v));
    Vec_set(&v, 1, &b);
    ASSERT_EQ(2, Vec_length(&v));
    Vec_set(&v, 2, &c);
    ASSERT_EQ(3, Vec_length(&v));
    Vec_set(&v, 3, &d);
    ASSERT_EQ(4, Vec_length(&v));

    int16_t *items = (int16_t*) malloc(1 * sizeof(int16_t));
    items[0] = 8;

    Vec_splice(&v,(size_t) 3, (size_t)1, items, (size_t)0); 
    int16_t x_out = 0;
    ASSERT_DEATH({
            Vec_get(&v, (size_t)3, &x_out);
            }, ".* Out of Bounds");

    Vec_drop(&v);
}

TEST(VecSpec, splice_equals_contract) {
    Vec v = Vec_value(1, sizeof(int16_t));

    Vec w = Vec_value(4, sizeof(int16_t));
    int16_t e = 1;
    int16_t f = 2;
    int16_t g = 3;
    int16_t h = 4;
    ASSERT_EQ(0, Vec_length(&w));
    Vec_set(&w, 0, &e);
    ASSERT_EQ(1, Vec_length(&w));
    Vec_set(&w, 1, &f);
    ASSERT_EQ(2, Vec_length(&w));
    Vec_set(&w, 2, &g);
    ASSERT_EQ(3, Vec_length(&w));
    Vec_set(&w, 3, &h);
    ASSERT_EQ(4, Vec_length(&w));

    int16_t *items = (int16_t*) malloc(4 * sizeof(int16_t));
    items[0] = 1;
    items[1] = 2;
    items[2] = 3;
    items[3] = 4;

    Vec_splice(&v,(size_t) v.length, (size_t)0, items, (size_t)4); 
    ASSERT_EQ(true, Vec_equals(&v, &w));
    Vec_drop(&w);
    Vec_drop(&v);
}

TEST(VecSpec, splice_equals_false) {
    Vec v = Vec_value(1, sizeof(int16_t));

    Vec w = Vec_value(4, sizeof(int16_t));
    int16_t e = 1;
    int16_t f = 2;
    int16_t g = 3;
    int16_t h = 4;
    ASSERT_EQ(0, Vec_length(&w));
    Vec_set(&w, 0, &e);
    ASSERT_EQ(1, Vec_length(&w));
    Vec_set(&w, 1, &f);
    ASSERT_EQ(2, Vec_length(&w));
    Vec_set(&w, 2, &g);
    ASSERT_EQ(3, Vec_length(&w));
    Vec_set(&w, 3, &h);
    ASSERT_EQ(4, Vec_length(&w));

    int16_t *items = (int16_t*) malloc(4 * sizeof(int16_t));
    items[0] = 1;
    items[1] = 2;
    items[2] = 3;

    Vec_splice(&v,(size_t) v.length, (size_t)0, items, (size_t)3); 
    ASSERT_EQ(false, Vec_equals(&v, &w));
    Vec_drop(&w);
    Vec_drop(&v);
}

TEST(VecSpec, splice_replace_equals) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t a = 1;
    int16_t b = 2;
    int16_t c = 3;
    int16_t d = 4;
    ASSERT_EQ(0, Vec_length(&v));
    Vec_set(&v, 0, &a);
    ASSERT_EQ(1, Vec_length(&v));
    Vec_set(&v, 1, &b);
    ASSERT_EQ(2, Vec_length(&v));
    Vec_set(&v, 2, &c);
    ASSERT_EQ(3, Vec_length(&v));
    Vec_set(&v, 3, &d);
    ASSERT_EQ(4, Vec_length(&v));

    Vec w = Vec_value(4, sizeof(int16_t));
    int16_t e = 1;
    int16_t f = 2;
    int16_t g = 3;
    int16_t h = 4;
    ASSERT_EQ(0, Vec_length(&w));
    Vec_set(&w, 0, &e);
    ASSERT_EQ(1, Vec_length(&w));
    Vec_set(&w, 1, &f);
    ASSERT_EQ(2, Vec_length(&w));
    Vec_set(&w, 2, &g);
    ASSERT_EQ(3, Vec_length(&w));
    Vec_set(&w, 3, &h);
    ASSERT_EQ(4, Vec_length(&w));

    int16_t *items = (int16_t*) malloc(4 * sizeof(int16_t));
    items[0] = 1;
    items[1] = 2;
    items[2] = 3;
    items[3] = 4;

    Vec_splice(&v,(size_t) v.length, (size_t)4, items, (size_t)4); 
    ASSERT_EQ(true, Vec_equals(&v, &w));
    Vec_drop(&w);
    Vec_drop(&v);
}

TEST(VecSpec, set_splice_dynamically) {
    Vec v = Vec_value(1, sizeof(int16_t));
    int16_t a = 1;
    ASSERT_EQ(0, Vec_length(&v));
    Vec_set(&v, 0, &a);
    ASSERT_EQ(1, Vec_length(&v));

    int16_t *items = (int16_t*) malloc(4 * sizeof(int16_t));
    items[0] = 2;
    items[1] = 3;
    items[2] = 4;

    Vec_splice(&v,(size_t) v.length, (size_t)0, items, (size_t)3); 
    ASSERT_EQ(Vec_length(&v), 4);
    Vec_drop(&v);
}

TEST(VecSpec, splice_set_contract) {
    Vec v = Vec_value(2, sizeof(int16_t));

    int16_t *items = (int16_t*) malloc(4 * sizeof(int16_t));
    items[0] = 1;
    items[1] = 2;
    items[2] = 3;
    items[3] = 4;

    Vec_splice(&v,(size_t) 0, (size_t)0, items, (size_t)4); 

    Vec w = Vec_value(4, sizeof(int16_t));
    int16_t e = 1;
    int16_t f = 2;
    int16_t g = 3;
    int16_t h = 4;
    ASSERT_EQ(0, Vec_length(&w));
    Vec_set(&w, 0, &e);
    ASSERT_EQ(1, Vec_length(&w));
    Vec_set(&w, 1, &f);
    ASSERT_EQ(2, Vec_length(&w));
    Vec_set(&w, 2, &g);
    ASSERT_EQ(3, Vec_length(&w));
    Vec_set(&w, 3, &h);
    ASSERT_EQ(4, Vec_length(&w));

    ASSERT_EQ(Vec_length(&v), Vec_length(&w));
    Vec_drop(&v);
    Vec_drop(&w);
}

TEST(VecSpec, splice_set_death) {
    Vec v = Vec_value(2, sizeof(int16_t));

    int16_t *items = (int16_t*) malloc(4 * sizeof(int16_t));
    items[0] = 1;
    items[1] = 2;
    items[2] = 3;
    items[3] = 4;

    Vec_splice(&v,(size_t) 0, (size_t)0, items, (size_t)2); 
    int16_t value = 5;

    ASSERT_DEATH({
            Vec_set(&v, 3, &value);
            }, ".* - Out of Bounds");
    Vec_drop(&v);
}
