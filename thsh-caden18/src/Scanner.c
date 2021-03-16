#include <ctype.h>
#include <stdio.h>

#include "CharItr.h"

#include "Scanner.h"

Token get_token(CharItr *char_itr);

Scanner Scanner_value(CharItr char_itr)
{
    Token next = get_token(&char_itr);

    Scanner itr = {
        char_itr,
        next
    };

    return itr;
}

bool Scanner_has_next(const Scanner *self)
{
    
    if (self->next.type != END_TOKEN) {
        return true;
    }
    CharItr temp = self->char_itr;
    int k = 0;
    while (CharItr_has_next(&temp)) {
        char tok = CharItr_has_next(&temp);
        if (tok == EOF) {
            return true;
        }
        if (tok == '|') {
            return true;
        }
        if (tok == ' ' || tok == '\t' || tok == '\n' || tok == '\0') {
            if (k == 0) {
                continue;
            }
            return true;
        }
        k++;
        if (!CharItr_has_next(&temp)) {
            return true;
        }
    }
    return false;
}

Token Scanner_peek(const Scanner *self)
{
    if (Scanner_has_next(self)) {
        return self->next;
    } else {
        Token next = {
            END_TOKEN, 
            Str_from("")
        };
        return next;
    }
}

Token Scanner_next(Scanner *self)
{
    if (Scanner_has_next(self)) {
        Token next = self->next;
        Token second = get_token(&self->char_itr);
        self->next = second;
        return next;
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

Token get_token(CharItr *char_itr)
{
    char *token = malloc(2000 * sizeof(char));
    int k = 0;
    while (CharItr_has_next(char_itr)) {
        char tok = CharItr_next(char_itr);
        if (tok == EOF) {
            Token pipe = {
                PIPE_TOKEN,
                Str_from("")
            };
            free(token);
            return pipe;
        }
        if (tok == '|') {
            Token pipe = {
                PIPE_TOKEN,
                Str_from("|")
            };
            free(token);
            return pipe;
        }
        if (tok == ' ' || tok == '\t' || tok == '\n' || tok == '\0') {
            if (k == 0) {
                continue;
            }
            Token pipe = { 
                WORD_TOKEN,
                Str_from(token)
            };
            free(token);
            return pipe;
        }
        token[k] = tok;
        k++;
        if (!CharItr_has_next(char_itr)) {
            Token pipe = {
                WORD_TOKEN,
                Str_from(token)
            };
            free(token);
            return pipe;
        }
    }
    free(token);
    Token pipe = {
        END_TOKEN,
        Str_from("")
    };
    return pipe;
}
