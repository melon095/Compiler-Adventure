grammar Glyph;

// Parser rules

program
    : statement+ EOF
    ;

statement
    : functionDefinition
    | expressionStatement
    | conditional
    | loop
    ;

expressionStatement
    : expression SEMICOLON
    ;

variableDeclaration
    : '@' Identifier '->' expression SEMICOLON
    ;

functionDefinition
    : 'fun' Identifier '(' parameters? ')' '->' block
    ;

functionCall
    : expression* '@' Identifier '<->'
    ;

argumentList
    : expression (',' expression)*
    ;

conditional
    : expression '?' '->' expression ('->' expression)?
    ;

loop
    : '^' expression block
    ;

block
    : '{' statement* '}'
    ;

parameters
    : Identifier (',' Identifier)*
    ;

// Expression rules

expression
    : assignment
    | binaryOperation
    | unaryOperation
    | primary
    ;

assignment
    : Identifier '->' expression
    ;

binaryOperation
    : expression operator expression
    ;

unaryOperation
    : operator expression
    ;

primary
    : '(' expression ')'        // Parenthesized expression
    | functionCall              // Function call
    | variableDeclaration       // Variable declaration  
    | Identifier                // Variable reference
    | Number                    // Number literal
    | String                    // String literal
    ;

operator
    : '+'
    | '-'
    | '×'
    | '÷'
    | '<'
    | '>'
    | '>='
    | '<='
    | '=='
    | '!='
    ;

// Lexer rules

Identifier: [a-zA-Z_][a-zA-Z_0-9]*;
Number: [0-9]+;
String: '"' .*? '"';
SEMICOLON: ';';

WS: [ \t\r\n]+ -> skip;
COMMENT: '//' ~[\r\n]* -> skip;
