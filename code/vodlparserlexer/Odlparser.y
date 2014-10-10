%name OdlParserBase
%define LSP_NEEDED
%define ERROR_BODY = 0
%define LEX_BODY = 0

%define PARSE_PARAM odl::TOdlAstNode*& outputAstNode

%define MEMBERS public: int a;
%define CONSTRUCTOR_INIT : a(1)

%header{

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

#include <iostream>
#include <fstream>
#include <string>

#include "vOdl/OdlAstNode.h"

%}
 
%union
{
	odl::TOdlAstNode* FAstNode;
}
 
%token UNKNOWN

%token TOKEN_IS
%token TOKEN_OPEN_BRACE
%token TOKEN_CLOSE_BRACE
%token TOKEN_OPEN_BRACKET
%token TOKEN_CLOSE_BRACKET
%token TOKEN_OPEN_PARENTHESIS
%token TOKEN_CLOSE_PARENTHESIS
%token TOKEN_EQUALS
%token TOKEN_COMMA
%token TOKEN_NAMESPACE
%token TOKEN_TEMPLATE
%token TOKEN_NULLPTR
%token TOKEN_COLON // {TODO} unused

%token <FAstNode> OPERATOR_PLUS OPERATOR_MINUS OPERATOR_MULTIPLY OPERATOR_DIVIDE OPERATOR_MODULO
%token <FAstNode> VALUE_INTEGER
%token <FAstNode> VALUE_FLOAT
%token <FAstNode> VALUE_STRING
%token <FAstNode> IDENTIFIER
%token< FAstNode> TOKEN_NULLPTR

// rules types
%type <FAstNode> named_declaration_list named_declaration
%type <FAstNode> anomymous_object_declaration_or_reference
%type <FAstNode> property_declaration_list property_declaration
%type <FAstNode> expression term factor
%type <FAstNode> vector_value_list vector_value
%type <FAstNode> identifier_list typed_identifier_list typed_identifier_list_rec typed_identifier

%start odl_ast
 
%%

odl_ast
: named_declaration_list
{
	outputAstNode = $1;
}
;

named_declaration_list
: named_declaration_list named_declaration
{
	odl::TOdlAstNode* theNamespace = $1;
	theNamespace->Namespace_AppendNamedDeclaration($2);
	$$ = theNamespace;
}
|
{
	odl::TOdlAstNode* theNamespace = new odl::TOdlAstNode();
	theNamespace->SetAsNamespace();
	$$ = theNamespace;	
}
;

named_declaration
: IDENTIFIER TOKEN_IS expression
{
	odl::TOdlAstNode* namedDeclaration = new odl::TOdlAstNode();
	namedDeclaration->SetAsNamedDeclaration($1, $3);
	$$ = namedDeclaration;
}
| TOKEN_NAMESPACE IDENTIFIER TOKEN_OPEN_BRACE named_declaration_list TOKEN_CLOSE_BRACE
{
	odl::TOdlAstNode* theNamespace = $4;
	theNamespace->SetIdentifierPointer($2);
	$$ = theNamespace;
}
| TOKEN_TEMPLATE IDENTIFIER TOKEN_IS IDENTIFIER TOKEN_OPEN_PARENTHESIS typed_identifier_list TOKEN_CLOSE_PARENTHESIS TOKEN_OPEN_BRACE property_declaration_list TOKEN_CLOSE_BRACE
{
	// {TODO}
	// not a good idea to put template declaration here.
	// it avoid the shift-reduce conflict but need a semantical analysis after.
//	delete $1;
//	delete $3;
//	delete $6;

	odl::TOdlAstNode* templateDeclaration = new odl::TOdlAstNode();
	templateDeclaration->SetAsTemplateDeclaration();
	templateDeclaration->SetIdentifierPointer($2);

	$$ = templateDeclaration;
}
;

anomymous_object_declaration_or_reference
: IDENTIFIER
{
	odl::TOdlAstNode* reference = $1;
	$$ = reference;
}
| IDENTIFIER TOKEN_OPEN_BRACE property_declaration_list TOKEN_CLOSE_BRACE
{
	odl::TOdlAstNode* objectDeclaration = new odl::TOdlAstNode();
	objectDeclaration->SetAsObjectDeclaration($1, $3);
	$$ = objectDeclaration;
}
| IDENTIFIER TOKEN_OPEN_PARENTHESIS identifier_list TOKEN_CLOSE_PARENTHESIS
{
	// {TODO}
	delete $1;
	delete $3;

	// template instanciation.
	odl::TOdlAstNode* templateDeclaration = new odl::TOdlAstNode();
	templateDeclaration->SetAsTemplateInstanciation();
	$$ = templateDeclaration;
}
| TOKEN_NULLPTR
{
	odl::TOdlAstNode* theNullptr = new odl::TOdlAstNode();
	theNullptr->SetAsNullPtr();
	$$ = theNullptr;
}
;


identifier_list
: expression TOKEN_COMMA identifier_list
{
	// {TODO}
	delete $1;
	delete $3;
	$$ = nullptr;
}
| expression
{
	// {TODO}
	delete $1;
	$$ = nullptr;
}
|
{
	$$ = nullptr;
}
;


typed_identifier_list
: typed_identifier_list_rec
{
	$$ = nullptr;
}
| 
{
	$$ = nullptr;
}
;

typed_identifier_list_rec
: typed_identifier_list_rec TOKEN_COMMA typed_identifier 
{
	// {TODO}
	delete $1;
	delete $3;
	$$ = nullptr;
}
| typed_identifier
{
	// {TODO}
	delete $1;
	$$ = nullptr;
}
;

typed_identifier
: IDENTIFIER IDENTIFIER
{
	delete $1;
	delete $2;
	$$ = nullptr;
}
;

property_declaration_list
: property_declaration_list property_declaration
{
	odl::TOdlAstNode* propertyDeclList = $1;
	propertyDeclList->PropertyDeclarationList_AppendPropertyDeclaration($2);
	$$ = propertyDeclList;
}
|
{
	odl::TOdlAstNode* propertyListNode = new odl::TOdlAstNode();
	propertyListNode->SetAsPropertyDeclarationList();
	$$ = propertyListNode;
}
;

property_declaration
: IDENTIFIER TOKEN_EQUALS expression
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsPropertyDeclaration($1, $3);
	$$ = node;
}
;

expression
: expression OPERATOR_PLUS term
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression($1, $2, $3);
	$$ = node;
}
| expression OPERATOR_MINUS term
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression($1, $2, $3);
	$$ = node;
}
| term
{
	$$ = $1;
}
;

term
: term OPERATOR_MULTIPLY factor
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression($1, $2, $3);
	$$ = node;
}
| term OPERATOR_DIVIDE factor
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression($1, $2, $3);
	$$ = node;
}
| term OPERATOR_MODULO factor
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression($1, $2, $3);
	$$ = node;
}
| factor
{
	$$ = $1;
}
;

factor
: TOKEN_OPEN_PARENTHESIS expression TOKEN_CLOSE_PARENTHESIS
{
	$$ = $2;
}
| OPERATOR_MINUS factor
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsExpression(nullptr, $1, $2);
	$$ = node;
}
| VALUE_INTEGER
{
	$$ = $1;
}
| VALUE_FLOAT
{
	$$ = $1;
}
| VALUE_STRING
{
	$$ = $1;
}
| vector_value
{
	$$ = $1;
}
| anomymous_object_declaration_or_reference
{
	$$ = $1;
}
;

vector_value
: TOKEN_OPEN_BRACKET vector_value_list TOKEN_CLOSE_BRACKET
{
	odl::TOdlAstNode* vector = $2;
	$$ = vector;
}
;

vector_value_list
: expression TOKEN_COMMA vector_value_list 
{
	odl::TOdlAstNode* vector = $3;
	odl::TOdlAstNode* expression = $1;
	vector->Vector_AppendItem(expression);
	$$ = vector;
}
| expression 
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsVector();
	node->Vector_AppendItem($1);
	$$ = node;
}
|
{
	odl::TOdlAstNode* node = new odl::TOdlAstNode();
	node->SetAsVector();
	$$ = node;
}
;

 
%%
