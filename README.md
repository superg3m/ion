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
<program> ::= (<function_decl> | <struct_decl> |  <enum_decl>  <variable_decl> | <statement>)*
<scope> ::= "{" (<declaration> | <statement>)* "}"

### DECLARATIONS
<variable_decl> ::= "var" <identifier> ":" ((<type>)? ("=" <expression>)) | ((<type>) ("=" <expression>)?) ";"

<function_decl> ::= "func" <identifier> "(" <param_list>? ")" "->" <return_type> <scope>
<param_list> ::= <parameter> ("," <parameter>)*
<parameter> ::= <identifier> ":" <type>
<return_type> ::= <type> | "(" <type_list> ")" | "void"
<type_list> ::= <type> ("," <type>)*

// var test: int;
// var test := 5;
// var test: int = 5;

<struct_decl> ::= "struct" <identifier> "{" (<struct_member>)* "}"
<struct_member> ::= <identifier> ":" <type> ";"

<enum_decl> ::= "enum" <identifier> "{" <enum_variant>? (";" <enum_variant>)* "}"
<enum_variant> ::= <identifier> ("=" <expression>)?


### TYPES
<type> ::= <primitive_type> | <struct_type> | <enum_type>
<primitive_type> ::= "s8" | "s16" | "s32" | "s64" | "u8" | "u16" | "u32" | "u64" | "f32" | "f64" | "bool" | "string"
<struct_type> ::= <identifier>
<enum_type> ::= <identifier>


### STATEMENTS
<statement> ::= <assignment> | <return_stmt> | <if_stmt> | <while_stmt> | <expr_stmt> |

<assignment> ::= <lhs> "=" <expression> ";"
<lhs> ::= <identifier> | <member_access> | <array_access>
<member_access> ::= <primary> "." <identifier>
<array_access> ::= <primary> "[" <expression> "]"

<return_stmt> ::= "return" <return_value>? ";"
<return_value> ::= <expression> | "(" <expression> ("," <expression>)* ")"

<if_stmt> ::= "if" "(" <expression> ")" <statement> ("else" <statement>)?
<while_stmt> ::= "while" "(" <expression> ")" <statement>
<expr_stmt> ::= <expression> ";"

### EXPRESSIONS (Operator Precedence)
// └── Logical (||, &&)
//      └── Comparison (==, !=, <, >, etc.)
//          └── Additive (+, -, |, ^)
//              └── Multiplicative (*, /, %, <<, >>)
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