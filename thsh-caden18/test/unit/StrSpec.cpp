#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Str.h"
}

/**
 * The purpose of these tests is to prove correctness of the Str
 * abstraction barrier from the user's point-of-view.
 */

TEST(StrSpec, values_init_empty) {
    Str s = Str_value(10);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);
}

TEST(StrSpec, values_init_cstr) {
    Str s = Str_value(10);
    ASSERT_STREQ("", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrSpec, get_set_contract) {
    Str s = Str_value(0);
    ASSERT_EQ(0, Str_length(&s));
    Str_set(&s, 0, 'a');
    ASSERT_EQ(1, Str_length(&s) + 1);
    Str_set(&s, 1, 'b');
    ASSERT_EQ(2, Str_length(&s) + 1);

    char x_out = 'a';
    char y_out = 'b';
    ASSERT_EQ(x_out, Str_get(&s, 0));
    ASSERT_EQ(y_out, Str_get(&s, 1));
    Str_drop(&s);
}

TEST(StrSpec, splice_get_contract) {
    Str s = Str_value(3);
    ASSERT_EQ(0, Str_length(&s));
    Str_set(&s, 0, 'a');
    ASSERT_EQ(1, Str_length(&s) + 1);
    Str_set(&s, 1, 'b');
    ASSERT_EQ(2, Str_length(&s)+ 1);
    Str_set(&s, 2, 'c');
    ASSERT_EQ(3, Str_length(&s) + 1);

    char *cstr = (char*) malloc(1 * sizeof(char));
    cstr[0] = 'f';

   Str_splice(&s, (size_t)1, (size_t)0, cstr, (size_t)1); 
   char x_out = 'f';
   ASSERT_EQ(x_out, Str_get(&s, 1));
   Str_drop(&s);

}

TEST(StrSpec, splice_get_death) {
    Str s = Str_value(3);
    ASSERT_EQ(0, Str_length(&s));
    Str_set(&s, 0, 'a');
    ASSERT_EQ(1, Str_length(&s) + 1);
    Str_set(&s, 1, 'b');
    ASSERT_EQ(2, Str_length(&s) + 1);
    Str_set(&s, 2, 'c');
    ASSERT_EQ(3, Str_length(&s) + 1);

    char *cstr = (char*) malloc(1 * sizeof(char));
    cstr[0] = 'f';

   Str_splice(&s, (size_t)2, (size_t)1, cstr, (size_t)0); 
   ASSERT_DEATH({
           Str_get(&s, (size_t)3);
           }, ".* - Out of Bounds");
   Str_drop(&s);
}

