#include <stdio.h>
#include <stdbool.h>

char const* stream = NULL;

bool accept(char c) {
    if (*stream == c) {
        stream++;
        return true;
    }
        
    return false;
}

bool notzero(void) {
    return accept('1') || accept('2') || accept('3') || accept('4') || accept('5') 
    || accept('6') || accept('7') || accept('8') || accept('9');
}

bool digit(void) {
    return accept('0') || notzero();
}

bool raw(void) {
    if (digit()) {
        raw();
        return true;
    }
    
    return false;
}

bool number(void) {
    if (notzero()) {
        raw();
        return true;
    }

    return accept('0');
}

bool decimal(void) {
    if(number()) {
        if (accept('.')) {
            raw();
        }

        return true;
    }
    else if(accept('.')) {
        return raw();
    }
    
    return false;
}

bool operand(void) {
    return accept('+') || accept('-') || accept('*');
}

bool arithmetic_expr(void) {
    if (decimal()) {
        if (operand()) {
            return arithmetic_expr();
        }

        return true;
    }

    return false;
}

bool str(void) {
    return arithmetic_expr() && accept(0);
}

void check(const char* string) {
    bool is_number = false;

    if (string != NULL) {
        stream = string;
        is_number = str();
    }

    printf("%s -> %d\n", string, is_number);
}

int main(void) {
    check("12345");
    check("hello12");
    check("0002");
    check("10dbd");
    check("0");
    check("13.5");
    check(".5");
    check("13.");
    check("0.");
    check("1758.267");
    check("00013.54");
    check("13000.0054");
    check("13.0");
    check("13.000");
    check("13.45000");
    check("15+45");
    check("15-45");
    check("15*45");
    check("15+0045");
    check("15+0");
    check("0-0.723");
    check("15.78*0.7854");
    check("15.13+");
    check("15.13+0.15*17.7");
    check("15.13+0.15*17.7-15.7");
    check("15.13+0.15*");
    check("15.13+0.15*17.7-");
    check(0);
    return 0;
}
