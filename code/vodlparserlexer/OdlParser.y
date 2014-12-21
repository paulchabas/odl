%name OdlParserBase
%define LSP_NEEDED
%define ERROR_BODY = 0
%define LEX_BODY = 0

%define PARSE_PARAM odl::TOdlAstNode*& outputAstNode

%define MEMBERS public: int a;
%define CONSTRUCTOR_INIT : a(1)

%header{

#include "BasicTypes.h"
#include "OdlAstNode.h"

%}
 
%union
{
	odl::TOdlAstNode*				FAstNode;
	odl::TOdlAstNodeIdentifier*		FAstNodeIdentifier;
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

%token <FAstNode> OPERATOR_PLUS OPERATOR_MINUS OPERATOR_MULTIPLY OPERATOR_DIVIDE OPERATOR_MODULO
%token <FAstNode> VALUE_INTEGER
%token <FAstNode> VALUE_FLOAT
%token <FAstNode> VALUE_STRING
%token <FAstNodeIdentifier> IDENTIFIER
%token< FAstNode> TOKEN_NULLPTR

// rules types

%type <FAstNode> named_declaration_list named_declaration
%type <FAstNode> anomymous_object_declaration_or_reference
%type <FAstNode> property_declaration_list property_declaration
%type <FAstNode> expression term factor
%type <FAstNode> vector_value_list vector_value
%type <FAstNode> template_instanciation_parameter_list template_declaration_parameter_list

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
| TOKEN_NAMESPACE IDENTIFIER TOKEN_OPEN_PARENTHESIS template_declaration_parameter_list TOKEN_CLOSE_PARENTHESIS TOKEN_OPEN_BRACE named_declaration_list TOKEN_CLOSE_BRACE
{
    // namespace template declaration

	odl::TOdlAstNode* theNamespace = $7;
	theNamespace->SetIdentifierPointer($2);
	theNamespace->Namespace_SetTemplateParameterList($4);
	$$ = theNamespace;
}
| TOKEN_NAMESPACE IDENTIFIER TOKEN_EQUALS IDENTIFIER TOKEN_OPEN_PARENTHESIS template_instanciation_parameter_list TOKEN_CLOSE_PARENTHESIS
{
	// namespace template instanciation
	odl::TOdlAstNode* theNamespace = new odl::TOdlAstNode();
	theNamespace->SetAsNamespaceTemplateInstanciation($2, $4, $6);
	$$ = theNamespace;
}
| TOKEN_TEMPLATE IDENTIFIER TOKEN_IS IDENTIFIER TOKEN_OPEN_PARENTHESIS template_declaration_parameter_list TOKEN_CLOSE_PARENTHESIS TOKEN_OPEN_BRACE property_declaration_list TOKEN_CLOSE_BRACE
{
	// object template declaration

	odl::TOdlAstNode* templateDeclaration = new odl::TOdlAstNode();
	templateDeclaration->SetAsObjectTemplateDeclaration($2, $4, $6, $9);
	$$ = templateDeclaration;
}
;

anomymous_object_declaration_or_reference
: IDENTIFIER
{
	odl::TOdlAstNodeIdentifier* reference = $1;
	reference->SetAsReferenceToResolve();	
	$$ = reference;
}
| IDENTIFIER TOKEN_OPEN_BRACE property_declaration_list TOKEN_CLOSE_BRACE
{
	odl::TOdlAstNode* objectDeclaration = new odl::TOdlAstNode();
	objectDeclaration->SetAsObjectDeclaration($1, $3);
	$$ = objectDeclaration;
}
| IDENTIFIER TOKEN_OPEN_PARENTHESIS template_instanciation_parameter_list TOKEN_CLOSE_PARENTHESIS
{
	// template instanciation.
	
	odl::TOdlAstNodeIdentifier* identifier = $1;
	identifier->SetAsReferenceToResolve(); // find the required template declaration.
	odl::TOdlAstNode* templateDeclaration = new odl::TOdlAstNode();
	templateDeclaration->SetAsTemplateInstanciation($1, $3);
	$$ = templateDeclaration;
}
| TOKEN_NULLPTR
{
	odl::TOdlAstNode* theNullptr = new odl::TOdlAstNode();
	theNullptr->SetAsNullPtr();
	$$ = theNullptr;
}
;

template_declaration_parameter_list
: template_declaration_parameter_list TOKEN_COMMA IDENTIFIER
{
	odl::TOdlAstNode* templateParameterList = $1;
	odl::TOdlAstNodeIdentifier* templateDeclarationParameter = $3;
	templateDeclarationParameter->SetAsTemplateDeclarationParameter();
	templateParameterList->TemplateDeclarationParameterList_AppendParameter(templateDeclarationParameter);
	$$ = templateParameterList;
}
| IDENTIFIER
{
	odl::TOdlAstNode* templateParameterList = new odl::TOdlAstNode();
	templateParameterList->SetAsTemplateDeclarationParameterList();
	odl::TOdlAstNodeIdentifier* templateDeclarationParameter = $1;
	templateDeclarationParameter->SetAsTemplateDeclarationParameter();
	templateParameterList->TemplateDeclarationParameterList_AppendParameter(templateDeclarationParameter);
	$$ = templateParameterList;
}
|
{
	odl::TOdlAstNode* templateParameterList = new odl::TOdlAstNode();
	templateParameterList->SetAsTemplateDeclarationParameterList();
	$$ = templateParameterList;
}
;

template_instanciation_parameter_list
: template_instanciation_parameter_list TOKEN_COMMA expression
{
	odl::TOdlAstNode* templateParameterList = $1;
	templateParameterList->TemplateInstanciationParameterList_AppendParameter($3);
	$$ = templateParameterList;
}
| expression
{
	odl::TOdlAstNode* templateParameterList = new odl::TOdlAstNode();
	templateParameterList->SetAsTemplateInstanciationParameterList();
	templateParameterList->TemplateInstanciationParameterList_AppendParameter($1);
	$$ = templateParameterList;
}
|
{
	odl::TOdlAstNode* templateParameterList = new odl::TOdlAstNode();
	templateParameterList->SetAsTemplateInstanciationParameterList();
	$$ = templateParameterList;
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
