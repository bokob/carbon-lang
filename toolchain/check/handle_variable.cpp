// Part of the Carbon Language project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "toolchain/check/context.h"
#include "toolchain/check/convert.h"
#include "toolchain/check/modifiers.h"
#include "toolchain/sem_ir/inst.h"

namespace Carbon::Check {

auto HandleVariableIntroducer(Context& context,
                              Parse::VariableIntroducerId parse_node) -> bool {
  // No action, just a bracketing node.
  context.node_stack().Push(parse_node);
  context.decl_state_stack().Push(DeclState::Var);
  return true;
}

auto HandleReturnedModifier(Context& context,
                            Parse::ReturnedModifierId parse_node) -> bool {
  // No action, just a bracketing node.
  context.node_stack().Push(parse_node);
  return true;
}

auto HandleVariableInitializer(Context& context,
                               Parse::VariableInitializerId parse_node)
    -> bool {
  SemIR::InstId init_id = context.node_stack().PopExpr();
  context.node_stack().Push(parse_node, init_id);
  return true;
}

auto HandleVariableDecl(Context& context, Parse::VariableDeclId parse_node)
    -> bool {
  // Handle the optional initializer.
  std::optional<SemIR::InstId> init_id =
      context.node_stack().PopIf<Parse::NodeKind::VariableInitializer>();

  if (context.node_stack().PeekIs<Parse::NodeKind::TuplePattern>()) {
    return context.TODO(parse_node, "tuple pattern in var");
  }

  // Extract the name binding.
  auto value_id = context.node_stack().PopPattern();
  if (auto bind_name =
          context.insts().Get(value_id).TryAs<SemIR::AnyBindName>()) {
    // Form a corresponding name in the current context, and bind the name to
    // the variable.
    auto name_context = context.decl_name_stack().MakeUnqualifiedName(
        bind_name->parse_node,
        context.bind_names().Get(bind_name->bind_name_id).name_id);
    context.decl_name_stack().AddNameToLookup(name_context, value_id);
    value_id = bind_name->value_id;
  }
  // TODO: Handle other kinds of pattern.

  // Pop the `returned` specifier if present.
  context.node_stack()
      .PopAndDiscardSoloParseNodeIf<Parse::NodeKind::ReturnedModifier>();

  // If there was an initializer, assign it to the storage.
  if (init_id.has_value()) {
    if (context.GetCurrentScopeAs<SemIR::ClassDecl>()) {
      // TODO: In a class scope, we should instead save the initializer
      // somewhere so that we can use it as a default.
      context.TODO(parse_node, "Field initializer");
    } else {
      init_id = Initialize(context, parse_node, value_id, *init_id);
      // TODO: Consider using different instruction kinds for assignment versus
      // initialization.
      context.AddInst(SemIR::Assign{parse_node, value_id, *init_id});
    }
  }

  context.node_stack()
      .PopAndDiscardSoloParseNode<Parse::NodeKind::VariableIntroducer>();

  // Process declaration modifiers.
  CheckAccessModifiersOnDecl(context, Lex::TokenKind::Var);
  LimitModifiersOnDecl(context, KeywordModifierSet::Access,
                       Lex::TokenKind::Var);
  auto modifiers = context.decl_state_stack().innermost().modifier_set;
  if (!!(modifiers & KeywordModifierSet::Access)) {
    context.TODO(context.decl_state_stack().innermost().saw_access_modifier,
                 "access modifier");
  }

  context.decl_state_stack().Pop(DeclState::Var);

  return true;
}

}  // namespace Carbon::Check
