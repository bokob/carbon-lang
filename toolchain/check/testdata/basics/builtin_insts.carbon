// Part of the Carbon Language project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// ARGS: compile --phase=check --dump-raw-sem-ir --builtin-sem-ir %s
//
// AUTOUPDATE

// CHECK:STDOUT: ---
// CHECK:STDOUT: filename:        builtin_insts.carbon
// CHECK:STDOUT: sem_ir:
// CHECK:STDOUT:   cross_ref_irs_size: 1
// CHECK:STDOUT:   bind_names:      {}
// CHECK:STDOUT:   functions:       {}
// CHECK:STDOUT:   classes:         {}
// CHECK:STDOUT:   types:
// CHECK:STDOUT:     type0:           {inst: instNamespaceType, value_rep: {kind: copy, type: type0}}
// CHECK:STDOUT:   type_blocks:     {}
// CHECK:STDOUT:   insts:
// CHECK:STDOUT:     instTypeType:    {kind: CrossRef, arg0: ir0, arg1: instTypeType, type: typeTypeType}
// CHECK:STDOUT:     instError:       {kind: CrossRef, arg0: ir0, arg1: instError, type: typeError}
// CHECK:STDOUT:     instBoolType:    {kind: CrossRef, arg0: ir0, arg1: instBoolType, type: typeTypeType}
// CHECK:STDOUT:     instIntType:     {kind: CrossRef, arg0: ir0, arg1: instIntType, type: typeTypeType}
// CHECK:STDOUT:     instFloatType:   {kind: CrossRef, arg0: ir0, arg1: instFloatType, type: typeTypeType}
// CHECK:STDOUT:     instStringType:  {kind: CrossRef, arg0: ir0, arg1: instStringType, type: typeTypeType}
// CHECK:STDOUT:     instFunctionType: {kind: CrossRef, arg0: ir0, arg1: instFunctionType, type: typeTypeType}
// CHECK:STDOUT:     instBoundMethodType: {kind: CrossRef, arg0: ir0, arg1: instBoundMethodType, type: typeTypeType}
// CHECK:STDOUT:     instNamespaceType: {kind: CrossRef, arg0: ir0, arg1: instNamespaceType, type: typeTypeType}
// CHECK:STDOUT:     inst+0:          {kind: Namespace, arg0: name_scope0, type: type0}
// CHECK:STDOUT:   inst_blocks:
// CHECK:STDOUT:     empty:           {}
// CHECK:STDOUT:     exports:         {}
// CHECK:STDOUT:     block2:
// CHECK:STDOUT:       0:               inst+0
// CHECK:STDOUT: ...
