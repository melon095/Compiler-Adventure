grammar Glyph;

// Parser

program: statement+ EOF;

statement
    : variableDeclaration
    | assignment
    | functionDefinition
    | functionCall
    | conditional
    | loop
    | expression
    ;

variableDeclaration
    : '@' Identifier '->' expression SEMICOLON
    ;

assignment
    : Identifier '->' expression SEMICOLON
    ;

functionDefinition
    : 'fun' Identifier '(' parameters? ')' '->' block SEMICOLON
    ;

functionCall
    : expression (expression)* Identifier '<->' '->'? Identifier?
    ;

conditional
    : expression '?' '->' expression ('->' expression)?
    ;

loop
    : '^' expression block
    ;

block
    : '{' statement* '}'
    | statement
    ;

parameters
    : Identifier (',' Identifier)*
    ;

expression
    : '(' expression ')'
    | binaryOperation
    | functionCall
    | Number
    | Identifier
    | String
    ;

binaryOperation
    : expression operator expression
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
