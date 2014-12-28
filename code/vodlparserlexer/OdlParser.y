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
	odl::TOdlAstNode*							FOdlAstNode;

	odl::TOdlAstNodeNamespaceDeclaration*		FOdlAstNodeNamespaceDeclaration;
	odl::TOdlAstNodeNamedDeclaration*			FOdlAstNodeNamedDeclaration;

	odl::TOdlAstNodeIdentifier*					FOdlAstNodeIdentifier;

	odl::TOdlAstNodeOperator*					FOdlAstNodeOperator;

	odl::TOdlAstNodeExpression*					FOdlAstNodeExpression;
	odl::TOdlAstNodeValue*						FOdlAstNodeValue;
	odl::TOdlAstNodeValueVector*				FOdlAstNodeValueVector;
	odl::TOdlAstNodeOperation*					FOdlAstNodeOperation;

	odl::TOdlAstNodeTemplateParameterList*		FOdlAstNodeTemplateParameterList;
	odl::TOdlAstNodeTemplateExpressionList*		FOdlAstNodeTemplateExpressionList;

	odl::TOdlAstNodePropertyDeclarationList*    FOdlAstNodePropertyDeclarationList;
	odl::TOdlAstNodePropertyDeclaration*		FOdlAstNodePropertyDeclaration;
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
%token TOKEN_TRUE
%token TOKEN_FALSE

%token <FOdlAstNodeOperator> OPERATOR_PLUS OPERATOR_MINUS OPERATOR_MULTIPLY OPERATOR_DIVIDE OPERATOR_MODULO
%token <FOdlAstNodeValue> VALUE_INTEGER
%token <FOdlAstNodeValue> VALUE_FLOAT
%token <FOdlAstNodeValue> VALUE_STRING
%token <FOdlAstNodeValue> VALUE_BOOLEAN
%token <FOdlAstNodeIdentifier> IDENTIFIER
%token< FOdlAstNode> TOKEN_NULLPTR

// rules types

%type <FOdlAstNodeNamespaceDeclaration> named_declaration_list 
%type <FOdlAstNodeNamedDeclaration> named_declaration
%type <FOdlAstNodeExpression> anomymous_object_declaration_or_reference
%type <FOdlAstNodePropertyDeclarationList> property_declaration_list 
%type <FOdlAstNodePropertyDeclaration>	property_declaration
%type <FOdlAstNodeExpression> expression term factor
%type <FOdlAstNodeValueVector> vector_value_list vector_value
%type <FOdlAstNodeTemplateExpressionList> template_instanciation_parameter_list
%type <FOdlAstNodeTemplateParameterList> template_declaration_parameter_list

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
	odl::TOdlAstNodeNamespaceDeclaration* theNamespace = $1;
	theNamespace->AppendNamedDeclaration($2);
	$$ = theNamespace;
}
|
{
	odl::TOdlAstNodeNamespaceDeclaration* theNamespace = new odl::TOdlAstNodeNamespaceDeclaration();
	$$ = theNamespace;	
}
;

named_declaration
: IDENTIFIER TOKEN_IS expression
{
	odl::TOdlAstNodeIdentifier* identifier = $1;
	odl::TOdlAstNodeExpression* expression = $3;

	odl::TOdlAstNodeNamedDeclaration* namedDeclaration = nullptr;
	// Paul(2014/12/21) hum, little tricky here.
	if (expression->AstNodeType() == odl::TOdlAstNodeType::NAMED_DECLARATION)
	{
		namedDeclaration = expression->CastNode<odl::TOdlAstNodeNamedDeclaration>();
		namedDeclaration->SetIdentifierPointer($1);
	}
	else
	{
		namedDeclaration = new odl::TOdlAstNodeNamedDeclaration($1, $3, nullptr);
	}

	$$ = namedDeclaration;
}
| TOKEN_NAMESPACE IDENTIFIER TOKEN_OPEN_BRACE named_declaration_list TOKEN_CLOSE_BRACE
{
	odl::TOdlAstNodeNamespaceDeclaration* theNamespace = $4;
	theNamespace->SetIdentifierPointer($2);
	$$ = theNamespace;
}
| TOKEN_NAMESPACE IDENTIFIER TOKEN_OPEN_PARENTHESIS template_declaration_parameter_list TOKEN_CLOSE_PARENTHESIS TOKEN_OPEN_BRACE named_declaration_list TOKEN_CLOSE_BRACE
{
    // namespace template declaration

	odl::TOdlAstNodeNamespaceDeclaration* theNamespace = $7;
	theNamespace->SetIdentifierPointer($2);
	theNamespace->SetTemplateParameterListPointer($4);
	$$ = theNamespace;
}
| TOKEN_NAMESPACE IDENTIFIER TOKEN_EQUALS IDENTIFIER TOKEN_OPEN_PARENTHESIS template_instanciation_parameter_list TOKEN_CLOSE_PARENTHESIS
{
	// namespace template instanciation
	
	odl::TOdlAstNodeIdentifier* identifier = $2;
	odl::TOdlAstNodeTemplateExpressionList* expressionList = $6;
	odl::TOdlAstNodeIdentifier* targetNamespaceDeclarationIdentifier = $4;

	targetNamespaceDeclarationIdentifier->SetAsReferenceToResolve();

	odl::TOdlAstNodeTemplateNamespaceInstanciation* templateNamespaceInstanciation = new odl::TOdlAstNodeTemplateNamespaceInstanciation(targetNamespaceDeclarationIdentifier, expressionList);
	odl::TOdlAstNodeNamedDeclaration* namedDeclaration = new odl::TOdlAstNodeNamedDeclaration(identifier, templateNamespaceInstanciation, nullptr);
	templateNamespaceInstanciation->SetNamedDeclarationWeakReference(namedDeclaration);

	$$ = namedDeclaration;
}
| TOKEN_TEMPLATE IDENTIFIER TOKEN_IS IDENTIFIER TOKEN_OPEN_PARENTHESIS template_declaration_parameter_list TOKEN_CLOSE_PARENTHESIS TOKEN_OPEN_BRACE property_declaration_list TOKEN_CLOSE_BRACE
{
	// object template declaration
	odl::TOdlAstNodeIdentifier* typeIdentifier = $4;
	odl::TOdlAstNodeTemplateParameterList* templateParameterList = $6;

	odl::TOdlAstNodeTemplateObjectDeclaration* templateDeclaration = new odl::TOdlAstNodeTemplateObjectDeclaration(typeIdentifier, $9);
	odl::TOdlAstNodeNamedDeclaration* namedDeclaration = new odl::TOdlAstNodeNamedDeclaration($2, templateDeclaration, templateParameterList);
	templateDeclaration->SetNamedDeclarationWeakReference(namedDeclaration);

	$$ = namedDeclaration;
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
	// object declaration

	odl::TOdlAstNodeObjectDeclaration* objectDeclaration = new odl::TOdlAstNodeObjectDeclaration($1, $3);
	odl::TOdlAstNodeNamedDeclaration* anonymousObject = new odl::TOdlAstNodeNamedDeclaration(nullptr, objectDeclaration, nullptr);
	objectDeclaration->SetNamedDeclarationWeakReference(anonymousObject);
	$$ = anonymousObject;
}
| IDENTIFIER TOKEN_OPEN_PARENTHESIS template_instanciation_parameter_list TOKEN_CLOSE_PARENTHESIS
{
	// template instanciation.
	
	odl::TOdlAstNodeIdentifier* typeIdentifier = $1;
	typeIdentifier->SetAsReferenceToResolve(); // find the required template declaration.

	odl::TOdlAstNodeTemplateExpressionList* expressionList = $3;
	odl::TOdlAstNodeTemplateObjectInstanciation* templateDeclaration = new odl::TOdlAstNodeTemplateObjectInstanciation(typeIdentifier, expressionList);
	odl::TOdlAstNodeNamedDeclaration* anonymousObject = new odl::TOdlAstNodeNamedDeclaration(nullptr, templateDeclaration, nullptr);
	templateDeclaration->SetNamedDeclarationWeakReference(anonymousObject);

	$$ = anonymousObject;
}
| TOKEN_NULLPTR
{
	// nullptr instanciation.
	odl::TOdlAstNodeObjectDeclaration* theNullptr = new odl::TOdlAstNodeObjectDeclaration();
	$$ = theNullptr;
}
;

template_declaration_parameter_list
: template_declaration_parameter_list TOKEN_COMMA IDENTIFIER
{
	odl::TOdlAstNodeTemplateParameterList* templateParameterList = $1;
		
	odl::TOdlAstNodeIdentifier* identifier = $3;
	odl::TOdlAstNodeExpression* expression = nullptr;
	odl::TOdlAstNodeTemplateParameter* templateParameter = new odl::TOdlAstNodeTemplateParameter(identifier, expression);
	templateParameterList->AppendTemplateParameter(templateParameter);

	$$ = templateParameterList;
}
| IDENTIFIER
{
	odl::TOdlAstNodeTemplateParameterList* templateParameterList = new odl::TOdlAstNodeTemplateParameterList();
	
	odl::TOdlAstNodeIdentifier* identifier = $1;
	odl::TOdlAstNodeExpression* expression = nullptr;
	odl::TOdlAstNodeTemplateParameter* templateParameter = new odl::TOdlAstNodeTemplateParameter(identifier, expression);

	templateParameterList->AppendTemplateParameter(templateParameter);
	$$ = templateParameterList;
}
|
{
	odl::TOdlAstNodeTemplateParameterList* templateParameterList = new odl::TOdlAstNodeTemplateParameterList();
	$$ = templateParameterList;
}
;

template_instanciation_parameter_list
: template_instanciation_parameter_list TOKEN_COMMA expression
{
	odl::TOdlAstNodeTemplateExpressionList* templateExpressionParameterList = $1;
	odl::TOdlAstNodeExpression* expression = $3;
	templateExpressionParameterList->AppendTemplateExpressionParameter(expression);
	$$ = templateExpressionParameterList;
}
| expression
{
	odl::TOdlAstNodeTemplateExpressionList* templateExpressionParameterList = new odl::TOdlAstNodeTemplateExpressionList();
	templateExpressionParameterList->AppendTemplateExpressionParameter($1);
	$$ = templateExpressionParameterList;
}
|
{
	odl::TOdlAstNodeTemplateExpressionList* templateExpressionParameterList = new odl::TOdlAstNodeTemplateExpressionList();
	$$ = templateExpressionParameterList;
}
;

property_declaration_list
: property_declaration_list property_declaration
{
	odl::TOdlAstNodePropertyDeclarationList* propertyDeclarationList = $1;
	odl::TOdlAstNodePropertyDeclaration* propertyDeclaration = $2;
	propertyDeclarationList->AppendPropertyDeclaration(propertyDeclaration);
	$$ = propertyDeclarationList;
}
|
{
	odl::TOdlAstNodePropertyDeclarationList* propertyDeclarationList = new odl::TOdlAstNodePropertyDeclarationList();
	$$ = propertyDeclarationList;
}
;

property_declaration
: IDENTIFIER TOKEN_EQUALS expression
{
	odl::TOdlAstNodePropertyDeclaration* node = new odl::TOdlAstNodePropertyDeclaration($1, $3);
	$$ = node;
}
;

expression
: expression OPERATOR_PLUS term
{
	odl::TOdlAstNodeOperation* node = new odl::TOdlAstNodeOperation($1, $2, $3);
	$$ = node;
}
| expression OPERATOR_MINUS term
{
	odl::TOdlAstNodeOperation* node = new odl::TOdlAstNodeOperation($1, $2, $3);
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
	odl::TOdlAstNodeOperation* node = new odl::TOdlAstNodeOperation($1, $2, $3);
	$$ = node;
}
| term OPERATOR_DIVIDE factor
{
	odl::TOdlAstNodeOperation* node = new odl::TOdlAstNodeOperation($1, $2, $3);
	$$ = node;
}
| term OPERATOR_MODULO factor
{
	odl::TOdlAstNodeOperation* node = new odl::TOdlAstNodeOperation($1, $2, $3);
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
	odl::TOdlAstNodeOperation* node = new odl::TOdlAstNodeOperation(nullptr, $1, $2);
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
| VALUE_BOOLEAN
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
	odl::TOdlAstNodeValueVector* vector = $2;
	$$ = vector;
}
;

vector_value_list
: expression TOKEN_COMMA vector_value_list 
{
	odl::TOdlAstNodeValueVector* vector = $3;
	odl::TOdlAstNodeExpression* expression = $1;
	vector->Vector_AppendItem(expression);
	$$ = vector;
}
| expression 
{
	odl::TOdlAstNodeValueVector* node = new odl::TOdlAstNodeValueVector();
	odl::TOdlAstNodeExpression* expression = $1;
	node->Vector_AppendItem(expression);
	$$ = node;
}
|
{
	odl::TOdlAstNodeValueVector* node = new odl::TOdlAstNodeValueVector();
	$$ = node;
}
;

 
%%
