# ion
The ion programming language

1. FrontEnd: lexing + parsing + AST
2. Analysis: semantic analysis, optimization (optional)
3. Backend: codegen (bytecode or native code)
4. Interpreter: interpreting bytecode or walking the AST

BNF Key
--------------------------------
<foo>       Nonterminal (rule defined elsewhere)
"text"      Terminal (literal keyword or symbol)
::=         Defines a rule
|           Choice (OR)
()          Grouping

?           Optional (0 or 1 occurrence)
*           Repetition (0 or more occurrences)
+           Repetition (1 or more occurrences)
--------------------------------------------------

### HIGH-LEVEL STRUCTURE
<program> ::= (<function_decl> | <struct_decl> |  <enum_decl> | <variable_decl> | <statement>)*
<scope> ::= "{" (<variable_decl> | <statement>)* "}"

### DECLARATIONS
<variable_decl> ::= "var" <identifier> ":" ((<type>)? ("=" <expression>)) | ((<type>) ("=" <expression>)?) ";"
    // var test: int;
    // var test := 5;
    // var test: int = 5;

<function_decl> ::= "func" <identifier> "(" <param_list>? ")" "->" <return_type> <scope>
<param_list> ::= <parameter> ("," <parameter>)*
<parameter> ::= <identifier> ":" <type>
<return_type> ::= <type> | "(" <type_list> ")" | "void"
<type_list> ::= <type> ("," <type>)*
/*
    func get_value(a: int, b: int) -> void {}
*/

<struct_decl> ::= "struct" <identifier> "{" (<struct_member>)* "}"
<struct_member> ::= <identifier> ":" <type> ";"

<enum_decl> ::= "enum" <identifier> ":" <type> "{" <enum_variant>? ("," <enum_variant>)* "}"
<enum_variant> ::= <identifier> ("=" <expression>)?
enum TokenType : u32 {
    INTEGER = 0,
    FLOAT
}


### TYPES
<type> ::= <primitive_type> | <struct_type> | <enum_type>
<primitive_type> ::= "s8" | "s16" | "s32" | "s64" | "u8" | "u16" | "u32" | "u64" | "f32" | "f64" | "b8" | "b16" | "b32"  | "string"
<struct_type> ::= <identifier>
<enum_type> ::= <identifier>


### STATEMENTS
<statement> ::= <assignment> | <return_stmt> | <if_stmt> | <while_stmt> | <expr_stmt>

/*
TODO:
ARK pointed out that lhs doens't work with arr[]. longer chains
*/

<assignment> ::= <lhs> "=" <expression> ";"
<lhs> ::= <identifier> | <member_access> | <array_access>
// test = 4

<member_access> ::= <identifier> "." <identifier>
<array_access> ::= <identifier> "[" <expression> "]"

<return_stmt> ::= "return" <return_value>? ";"
<return_value> ::= <expression> | "(" <expression> ("," <expression>)* ")"

<if_stmt> ::= "if" "(" <expression> ")" <statement> ("else" <statement>)?
<while_stmt> ::= "while" "(" <expression> ")" <statement>
<expr_stmt> ::= <expression> ";"

### EXPRESSIONS (Operator Precedence)
// └── Logical (||, &&)
//      └── Comparison (==, !=, <, >, etc.)
//          └── Additive (+, -, &, |, ^) (BinaryOp)
//              └── Multiplicative (*, /, %, <<, >>) (BinaryOp)
//                  └── Unary (+, -, !, ~, &, *)
//                      └── Primary (literals, identifiers, etc.) 
<expression> ::= <logical>
<logical> ::= <comparison> (("||" | "&&") <comparison>)*
<comparison> ::= <additive> (("==" | "!=" | "<" | "<=" | ">" | ">=") <additive>)*
<additive> ::= <multiplicative> (("+" | "-" | "|" | "^") <multiplicative>)*
<multiplicative> ::= <unary> (("*" | "/" | "%" | "<<" | ">>") <unary>)*
<unary> ::= ("+" | "-" | "!" | "~" | "&" | "*") <unary> | <primary>
<primary> ::= <literal> | <identifier> | "(" <expression> ")" | <function_call> | <member_access> | <array_access>

<function_call> ::= <identifier> "(" <expression_list>? ")"
<expression_list> ::= <expression> ("," <expression>)*

### MOST GRANULAR COMPONENTS
<literal> ::= <integer_literal> | <float_literal> | <string_literal> | <bool_literal>
<integer_literal> ::= e.g (-1, 0, 1, 2, 3, ...)
<float_literal> ::= e.g (-1.01, 0.00, 1.01, 2.02, 3.03, ...)
<string_literal> ::= e.g ("Hello", "World")
<bool_literal> ::= "true" | "false"
<identifier> ::= e.g(name, test, foo, bar)