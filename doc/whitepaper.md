# Dialect

## Abstract

Dialect is a personal project, it's simply a find-and-replace tool that is customized to be used as a translator for programming langauges.

With Dialect you can define your own accent of a language and not be afraid of readability of code and co-workers which are not at the same page with you.

## Introduction
### Need
We are talking about the preprocessing of codes. Preprocessing itself it's not a bad idea and code generation and template languages for markup langauges (such as HTML) are now mainly used. But why macros are bad ideas? The point is that, code generation, or meta programming is simply the process that leads to a generated code that you should never edit/debug/read it. just use it. But in the other hand, using macros is somehow in between, you are generating a part of a code which is your main code, this makes code unreadable, undebugable and more. in a related term, overusing DSLs (Domain Specific Languages) or templates or operator overloading are another cases of so called **partial meta-programming** which is forbidden too.

But another growing trend is visually compressing code in IDEs which is basically another **partial** effort to make code short. In this case we see the main code is the main code and only a visualized. 

TODO, talk about template languages

we are combining this two ideas to make a **partial meta-programming tool** which makes the main code still the core and also revisiting old macro commands with a new grammar-based commands.

### previous efforts

GP Macro processors
https://en.wikipedia.org/wiki/General-purpose_macro_processor

- M4 https://www.gnu.org/savannah-checkouts/gnu/m4/manual/m4-1.4.18/m4.html
- GPP
- GEMA http://gema.sourceforge.net/new/gema_man.shtml
- GMP
- GPM

GP Template Languages

https://en.wikipedia.org/wiki/XSLT
https://mustache.github.io/mustache.5.html

GP Parsers


## design

### requirements

### informal idea

**This language uses shabang `#!/bin/dialect`**

Here we introduce language features one by one.

Dialect is a language made of simple replace rules.

```
'#' with 'length'
```

but it's a language, you can define things and then use it

```
digit := '0' to '9'

digit '.' digit '.' digit with 'latest version'
```

with simple features like importing, commenting and introducing placeholders

in a pattern to match, we can define a placeholder which can be used in both pattern as a reference and in replacing text as a content.

```
#!/bin/dialect
; some comments
include 'java'

'#' ['0' to '9'] as v with 'length(' v ')'
```

```
digit := '0' to '9'
```

it supports functions that makes it support regular expression, but in a readable manner.

```
hide ['while (' expr as x ')  {' statements] [id] as v '++;' hide[statements '}'] with [id] as v ' += 1;'
```

and, the power of CFG grammars
```
E := something
E := ['a' E 'b'] or none
```

and introducing context which make everything easier.

applying find and replace in a context, can simplify rules.
```
[id] as v '++' with v ' += 1' in hide ['while ('expr as x')  {' statements] [id] as v '++' hide[statements '}']
```

everything will executed in parallel and once, unless you want.
```
rule a 'one' with '1' 
rule b 'three' with '3'
rule c 'onethree' with '13'
run after [[a and b] or c] '13' with 'devil'
run always 'devil' with died
run 4 times 'x' -> 'xx'
```

with algebraic power

```
rule first '' with '1 1'
run after first 4 times number as v1 number as v2 with v2 v1 + v2
```

and assertion

```
assert all query in context
assert exists query in context 
```

ignoring is the best weapon

```
strings := '\'' anything '\''
'x' with 'y' ignoring strings
```

scopes, that can be nested

```
in strings [
    'a' with 'A'
    'b' with 'B'
]
```

counting, may be useful

```
word := many [character]
EOF with 'words: ' number [word]

```

### complete reference

all possible commands are

Include: includes another file right at the written line.

```
include 'filename'
include './relative/file' ; relative to current file
```

assignment
```
id := expr
id := something ; forward decleration
```

Rule: a find-and-replace command
```
(rule name) (run (n times) (after other-rules)) expr with expr in expr ignoring expr
```

scopes
```
(scope name) (in context) (ignoring ign-context) [
    statements
]
```

assertion
```
assert [name] all query [in context]
assert [name] exists query [in context]
```

## grammar

```
escaped-char: ( \\' | \\\\ )
valid-char: ['\\]
id-char: A-Z and a-z and _
id: id-char []
digits: 0-9
text: 'escaped-chars or valid-chars'
keywords: with, in, ignoring, after, run, rule, and, or, times
id: \w(\w\d)+
    rule funcs: as, hide, number, ...
    rule consts: none, EOF, SOF
    placeholder funcs: uppercase, lowercase, ...
    defs
    placeholders

val operators: == * / 
assign: :=
```

```
INC: include text
EXPR: text EXPR
EXPR: id EXPR
EXPR: [EXPR]
LOGICAL: id
LOGICAL: [LOGICAL]
LOGICAL: id or LOGICAL
LOGICAL: id and LOGICAL
RULENAME: rule id
RULENAME: 
RUNTIMES: digits times
RUNTIMES: always
RUNTIMES: 
RUNAFTER: after LOGICAL
RUNAFTER: 
CONTEXT: in EXPR
CONTEXT:
INGORE: ignoring EXPR
INGORE:
RULE: RULENAME RUNTIMES RUNAFTER EXPR with EXPR CONTEXT IGNORE
SCOPENAME: scope id
SCOPENAME: 
SCOPE: SCOPENAME RUNTIMES RUNAFTER CONTEXT IGNORE { STATEMENTS }
ASSERTNAME: id
ASSERTNAME:
ASSERTTYPE: all
ASSERTTYPE: exists
assert: assert ASSERTNAME ASSERTTYPE EXPR CONTEXT INGORE
```

#### Appendix A, function defintion

a tuple with a bold name inside it, is a fuction

#### Appendix B, expression types
```
id types: rule, scope, assert, matchfunc, logical, valfunc, pattern

expr types: matchexpr,  valexpr, ruleexpr, numexpr

ruleexpr: scope.scope.scope.rule
ruleexpr: ruleexpr logical ruleexpr 
matchexpr: text matchexpr
matchexpr: pattern
valuexpr: matchexpr


```

we should consider prefix and postfix functions