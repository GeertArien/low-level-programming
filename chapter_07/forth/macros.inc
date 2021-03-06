%define last_item 0

%macro native 3
section .data
w_%2:
    dq last_item
    db %1,0
    db %3
xt_%2:
    dq i_%2
section .text
i_%2:
%define last_item w_%2
%endmacro

%macro native 2
native %1, %2, 0
%endmacro

%macro colon 3
section .data
w_%2:
    dq last_item
    db %1,0
    db %3
xt_%2:
    dq i_docol
%define last_item w_%2
%endmacro

%macro colon 2
colon %1, %2, 0
%endmacro

%macro branch 1 
dq xt_branch
dq %1 - $ - 8
%endmacro

%macro branch0 1 
dq xt_0branch
dq %1 - $ - 8
%endmacro
