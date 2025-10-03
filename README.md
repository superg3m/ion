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
<program> ::= (<function_decl> | <struct_decl> | <variable_decl>)*
<scope> ::= "{" (<node>)* "}"

<node> ::= (<statement> | <decleration> | <expression>)

### DECLARATIONS
<variable_decl> ::= "var" <identifier> ":" ((<type>)? ("=" <expression>)) | ((<type>) ("=" <expression>)?) ";"
// var test: int;
// var test := 5;
// var test: int = 5;

<function_decl> ::= "fn" <identifier> "(" <param_list>? ")" "->" <return_type> <scope>
<param_list> ::= <parameter> ("," <parameter>)*
<parameter> ::= <identifier> ":" <type>
<return_type> ::= <type> | "(" <type_list> ")" | "void"
<type_list> ::= <type> ("," <type>)*
/*
func get_value(a: int, b: int) -> void {}
*/

<struct_decl> ::= "struct" <identifier> "{" (<struct_member>)* "}"
<struct_member> ::= <identifier> ":" <type> ";"

### TYPES
<primitive_type> ::= "int" | "float" | "bool" | "string"

### STATEMENTS
<statement> ::= <assignment> |<return> | <if_else> | <while> |
                <continue> | <break>


<assignment> ::= <lhs> "=" <expression> ";"
<lhs> ::= <identifier> | <member_access> | <array_access>
// test = 4

<member_access> ::= <member_access> "." <member_access> | <identifier>
<array_access> ::= <identifier> ("[" <expression> "]")+

<return_stmt> ::= "return" <return_value>? ";"
<return_value> ::= <expression> | "(" <expression> ("," <expression>)* ")"

<if_stmt> ::= "if" "(" <expression> ")" <scope> ("else" <scope>)?
<while_stmt> ::= "while" "(" <expression> ")" <statement>

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

- Language Features
    - [] Out of order compilation (remove the need for function prototypes and forward declares)
    - [] Parse multiple files (How do you even build the ast? Do you build it seperately and just connect it somehow?)
    - [] Defer
    - [] Functions
    - [] Structs
    - [] LHS Exprsesion Access
    - [] Arrays
    - [] Proper Copying of data when passed to function or assignment
    - [] Typechecking
        - [] Type Inference

    - Generic Parameters


// Ultimately I think a simple is just a IonToken and structure kind an bool if its resolved yet
// Basically you want to recorrd all instances of a symbol being used 
and the context in which its being used.

/*
// This is for non-declarations of course
enum IonSymbolKind {
    ION_SYMBOL_IDENT // the symbol in quesiton is being used as an identifer
    ION_SYMBOL_TYPE  // the symbol in quesiton is being used as a type
    ION_SYMBOL_FUNCTION_CALL // the symbol in quesiton is being used as a type
}

// So the last thing you have to do is just have a mapping:
StructDeclaration -> ION_SYMBOL_TYPE
Variable Declaration -> ION_SYMBOL_IDENT
FunctionCall_SE -> SYMBOL_FUNCITON_CALL

typedef struct IonSymbol {
    IonSymbolKind kind;
    IonNode* decl; // NULLPTR if not resolved yet
    
    // This is resolved if and only if I can lookup in a symbol and that declaration matches the context.
    // One nice thing about the symbols is that you can only ever have once instance of a symbol because
    // we only allow for one symbol per context for example a symbol can't be marked as having
    // a TYPE context, but later we resolve the definition and its a function decl. This is not allowed.
} IonSymbol;

typedef struct IonSymbolTable {
    CKG_HashMap(CKG_StringView, IonSymbol)* symbols;
    struct IonSymbolTable* parent;
} IonSymbolTable;

at the end of the second pass iterate through the symbols
IonSymbolTable table = ...;
for (int i = 0; i < table.symbols.meta.capacity; i++) {
    if (entries[i].filled && !entries[i].dead) {
        CKG_StringView str = entries[i].key;
        Symbol s = entries[i].value;
        if (s.decl == NULLPTR) {
            printf("Unresolved symbol: %.*s", str.length, str.data);
            exit(-1);
        }
    }
}
*/

- Backend Features
    - [] Tree walk interpreter
    
    - [?] Transpile to C
    - [?] FFI to call native functions
        - [] If a function definition is marked as foreign then when you encounter a call you know that you
        have to do a proc address lookup into a dll of a c library.

        foreign fn printf(fmt: string) -> int;

    - [?] ByteCode
    - [?] LLVM