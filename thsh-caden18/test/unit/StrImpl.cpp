#include "gtest/gtest.h"

extern "C" {
#include "Str.h"
#include "string.h"
}

TEST(StrImpl, value) {
    Str s = Str_value(2);
    ASSERT_EQ(1, s.length); // Vec's length includes null char
    ASSERT_EQ(3, s.capacity); // Requested space + 1 for null char
    char *buffer = (char*) s.buffer;
    ASSERT_EQ('\0', buffer[0]);
    Str_drop(&s);
}

TEST(StrImpl, drop) {
    Str s = Str_value(2);
    Str_drop(&s);
    ASSERT_EQ(0, s.capacity);
    ASSERT_EQ(0, s.length);
    ASSERT_EQ(nullptr, s.buffer);
}

/**
 * Helper fixture to setup a Str whose buffer
 * is prefilled with "abcd". 
 */
Str fixture_abcd()
{
    Str s = Str_value(4);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';
    buffer[3] = 'd';
    buffer[4] = '\0';
    s.length = 5; // Includes null char
    return s;
}

TEST(StrImpl, length) {
    Str s = Str_value(2);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);

    s = fixture_abcd();
    ASSERT_EQ(4, Str_length(&s));
    Str_drop(&s);
}

TEST(StrImpl, cstr) {
    Str s = fixture_abcd();
    ASSERT_STREQ("abcd", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrImpl, ref) {
    Str s = fixture_abcd();
    char *buffer = (char*) s.buffer;
    for (size_t i = 0; i < s.length; ++i) {
        ASSERT_EQ(&buffer[i], Str_ref(&s, i));
    }
    Str_drop(&s);
}

// TODO: Test remaining Str functions

TEST(StrImpl, empty) {
    Str s = Str_value(0);
    char *buffer = (char*) s.buffer;
    ASSERT_EQ('\0', buffer[0]);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);
}

TEST(StrImpl, from) {
    char *cstr = (char*) malloc(5 * sizeof(char));
    cstr[0] = 'a';
    cstr[1] = 'b';
    cstr[2] = 'c';
    cstr[3] = 'd';
    cstr[4] = '\0';


    Str s = Str_from(cstr);
    char *result = (char*) s.buffer;

    for (size_t i = 0; i < Str_length(&s); ++i) {
        ASSERT_EQ(result[i], cstr[i]);
    }
    ASSERT_EQ(Str_length(&s), 4);
    Str_drop(&s);
}

TEST(StrImpl, from_null_string) {
    char *cstr = (char*) malloc(1 * sizeof(char));
    cstr[0] = '\0';

    Str s = Str_from(cstr);
    char *result = (char*) s.buffer;

    ASSERT_EQ(result[0], cstr[0]);

    Str_drop(&s);
}

TEST(StrImpl, splice_insert_and_delete) {
    
    char *cstr = (char*) malloc(5 * sizeof(char));
    cstr[0] = 'b';
    cstr[1] = 'b';
    cstr[2] = 'b';

    Str s = fixture_abcd();
    char *s_buffer = (char*) s.buffer;
    Str s_out = Str_value(5);
    char *s_out_buffer = (char*) s_out.buffer;
    s_out_buffer[0] = 'b';
    s_out_buffer[1] = 'b';
    s_out_buffer[2] = 'b';
    s_out_buffer[3] = '\0';

    size_t insert_count = 3;
    size_t delete_count = 4;
    size_t index = 0;

    Vec_splice(&s, index, delete_count, cstr, insert_count);

   for (size_t i = 0; i < s.length; ++i) {
      ASSERT_EQ(s_buffer[i], s_out_buffer[i]);
   }

  Str_drop(&s);
  Str_drop(&s_out);
}

TEST(StrImpl, splice_delete_only) {
    char *cstr = (char*) malloc(5 * sizeof(char));
    cstr[0] = 'a';
    cstr[1] = 'b';
    cstr[2] = 'c';
    cstr[3] = '\0';

    Str s = fixture_abcd();
    char *s_buffer = (char*) s.buffer;

    Str s_out = Str_value(0);
    char *s_out_buffer = (char*) s_out.buffer;

    size_t insert_count = 0;
    size_t delete_count = 4;
    size_t index = 0;

    Vec_splice(&s, index, delete_count, cstr, insert_count);

    for (size_t i = 0; i < s.length; ++i) {
        ASSERT_EQ(s_buffer[i], s_out_buffer[i]);
    }

    Str_drop(&s);
    Str_drop(&s_out);
}


TEST(StrImpl, splice_at_length) {
    char *cstr = (char*) malloc(4 * sizeof(char));
    cstr[0] = 'a';
    cstr[1] = 'b';
    cstr[2] = 'c';
    cstr[3] = '\0';

    Str s = fixture_abcd();
    char *s_buffer = (char*) s.buffer;

    Str s_out = Str_value(8);
    char *s_out_buffer = (char*) s_out.buffer;
    s_out_buffer[0] = 'a';
    s_out_buffer[1] = 'b';
    s_out_buffer[2] = 'c';
    s_out_buffer[3] = 'd';
    s_out_buffer[4] = 'a';
    s_out_buffer[5] = 'b';
    s_out_buffer[6] = 'c';
    s_out_buffer[7] = '\0';

    size_t insert_count = 4;
    size_t delete_count = 0;

    Vec_splice(&s, Str_length(&s), delete_count, cstr, insert_count);

    for (size_t i = 0; i < s.length; ++i) {
        ASSERT_EQ(s_buffer[i], s_out_buffer[i]);
    }
    Str_drop(&s);
    Str_drop(&s_out);
}

TEST(StrImpl, append) {
    Str s = fixture_abcd();

    char *cstr = (char*)malloc(6 * sizeof(char));
    cstr[0] = 'e';
    cstr[1] = 'f';
    cstr[2] = 'g';
    cstr[3] = 'h';
    cstr[4] = 'i';
    cstr[5] = '\0';

    Str s_out = Str_value(10);
    char *s_out_buffer = (char*) s_out.buffer;
    s_out_buffer[0] = 'a';
    s_out_buffer[1] = 'b';
    s_out_buffer[2] = 'c';
    s_out_buffer[3] = 'd';
    s_out_buffer[4] = 'e';
    s_out_buffer[5] = 'f';
    s_out_buffer[6] = 'g';
    s_out_buffer[7] = 'h';
    s_out_buffer[8] = 'i';
    s_out_buffer[9] = '\0';

    char *s_buffer = (char*) s.buffer;

   Str_append(&s, cstr);

  for (size_t i = 0; i < s.length; ++i) {
     ASSERT_EQ(s_buffer[i], s_out_buffer[i]);
  } 

  Str_drop(&s_out);
  Str_drop(&s);
}


TEST(StrImpl, append_to_null) {
    Str s = Str_value(0);

    char *cstr = (char*)malloc(3 * sizeof(char));
    cstr[0] = 'e';
    cstr[1] = 'f';
    cstr[2] = '\0';

    Str s_out = Str_value(3);
    char *s_out_buffer = (char*) s_out.buffer;
    s_out_buffer[0] = 'e';
    s_out_buffer[1] = 'f';
    s_out_buffer[2] = '\0';

    char *s_buffer = (char*) s.buffer;

   Str_append(&s, cstr);

  for (size_t i = 0; i < s.length; ++i) {
     ASSERT_EQ(s_buffer[i], s_out_buffer[i]);
  } 

  Str_drop(&s_out);
  Str_drop(&s);
}


TEST(StrImpl, append_a_null) {
    Str s = fixture_abcd();;

    char *cstr = (char*)malloc(sizeof(char));
    cstr[0] = '\0';

    Str s_out = Str_value(3);
    char *s_out_buffer = (char*) s_out.buffer;
    s_out_buffer[0] = 'a';
    s_out_buffer[1] = 'b';
    s_out_buffer[2] = 'c';
    s_out_buffer[3] = 'd';
    s_out_buffer[4] = '\0';

    char *s_buffer = (char*) s.buffer;

   Str_append(&s, cstr);

  for (size_t i = 0; i < s.length; ++i) {
     ASSERT_EQ(s_buffer[i], s_out_buffer[i]);
  } 

  Str_drop(&s_out);
  Str_drop(&s);
}

TEST(StrImpl, get) {
    Str s = fixture_abcd();
    ASSERT_EQ(Str_get(&s, 2), 'c');
    Str_drop(&s);
}


TEST(StrImpl, get_null_str) {
    Str s = Str_value(0);
    ASSERT_EQ(Str_get(&s, 0), '\0');
    Str_drop(&s);
}


TEST(StrImpl, get_null_character) {
    Str s = fixture_abcd();
    ASSERT_EQ(Str_get(&s, 5), '\0');
    Str_drop(&s);
}

TEST(StrImpl, set) {
    Str s = fixture_abcd();
    char *s_buffer = (char*) s.buffer;
    Str_set(&s, 1, 'c');

    ASSERT_EQ(s_buffer[1], 'c');
    Str_drop(&s);
}

TEST(StrImpl, set_null) {
    Str s = Str_value(0);
    char *s_buffer = (char*) s.buffer;
    Str_set(&s, 0, 'c');

    ASSERT_EQ(s_buffer[0], 'c');
    Str_drop(&s);
}


TEST(StrImpl, set_null_character) {
    Str s = fixture_abcd();
    char *s_buffer = (char*) s.buffer;
    Str_set(&s, 2, '\0');

    ASSERT_EQ(s_buffer[2], '\0');
    Str_drop(&s);
}

TEST(StrImpl, set_length) {
    Str s = fixture_abcd();
    char *s_buffer = (char*) s.buffer;

    Str s_out = Str_value(5);
    char *s_out_buffer = (char*) s_out.buffer;
    s_out_buffer[0] = 'a';
    s_out_buffer[1] = 'b';
    s_out_buffer[2] = 'c';
    s_out_buffer[3] = 'd';
    s_out_buffer[4] = 'e';
    s_out_buffer[5] = '\0';

    Str_set(&s, Str_length(&s), 'e');

    for (size_t i = 0; i < s.length; ++i) {
        ASSERT_EQ(s_buffer[i], s_out_buffer[i]);
    }
    Str_drop(&s);
    Str_drop(&s_out);
}
