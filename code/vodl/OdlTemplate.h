#pragma once

#include "vodlparserlexer/OdlAstNode.h"

namespace odl
{
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
TOdlAstNodeTemplateObjectDeclaration const* GetTemplateObjectInstanciationDeclaration(TOdlAstNodeTemplateObjectInstanciation const* parTemplateObjectInstanciationNode);
std::string const& GetTemplateObjectInstanciationDeclarationTypeAsString(TOdlAstNodeTemplateObjectInstanciation const* parTemplateObjectInstanciationNode);
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
} // odl



