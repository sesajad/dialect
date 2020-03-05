# Dialect

## abstract

Dialect is a personal project, it's simply a find-and-replace tool that is customized to be used as a translator for programming langauges.

With Dialect you can define your own accent of a language and not be afraid of readability of code and co-workers which are not at the same page with you.

## introduction


## design

### requirements

### informal idea

Here we introduce language features one by one.

Dialect is a language made of simple replace rules.

```
#!/bin/dialect 

'#' with 'length'
```

with simple features like importing, commenting and introducing placeholders

in a pattern to match we can define a placeholder which can be used in both pattern as a reference and in replacing text as a content.

```
#!/bin/dialect
; some comments
include java

'#' id as v with 'length(' v ')'
```

```
; use functions and hide something

hide ['while ('expr as x')  {' statements] [id] as v '++' hide[statements '}'] with [id] as v ' += 1'

; can be rewritten as

[id] as v '++' with v ' += 1' in hide ['while ('expr as x')  {' statements] [id] as v '++' hide[statements '}']

and with declaration
while_context := hide ['while ('expr as x')  {' statements] [id] as v '++' hide[statements '}']

[id] as v '++' with v ' += 1' in while_context

; everything will executed in parallel and once

rule a 'one' with '1' 
rule b 'three' with '3'
rule c 'onethree' with '13'
run after [[a and b] or c] '13' with 'devil'
run always 'devil' with died
run 4 times 'x' -> 'xx'

; with algebraic power

rule first '' with '1 1'
run after first 4 times number as v1 number as v2 with v2 v1 + v2

; assertion is important
; proposal 1
assert id as v1 ' == ' id as v2 with v1 == v2
; prposal 2
assert all query in context
assert none query in context 

; ignoring is the best weapon
strings := '\'' anything '\''
'x' with 'y' ignoring strings

; factorizing, prefixing, and more
in strings [
    'a' with 'A'
    'b' with 'B'
]

; counting, may be useful
word := many [character]
EOF with 'words: ' number [word]

```

### complete reference

all possible commands are

Include: includes another file right at the written line.

```
include filename
include ./relative/file

```

Rule: a find-and-replace command
```
[rule name] [run [n times] [after other-rules]] expr with expr in expr ignoring expr
```


## grammar
###
```
escaped-char: \' and \\
valid-char: all except ' and \
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
include: include id
expr: val expr | id expr | [expr]
logical: id | [logical] | id or logical | id and logical
rule: rule id run expr times after logical expr with expr in expr ignoring expr
group: expr times after logical in expr ignoring expr [ rules ]
assert: assert?
groups as scopes? like C
```

we should consider prefix and postfix functions