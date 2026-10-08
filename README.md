# C_SW — LISP Interpreter in C

A complete, self-contained LISP interpreter written in C (C89/ANSI C). The interpreter supports interactive and batch execution modes, arithmetic and comparison operations, list manipulation, variable binding, control flow, and more.

---

## Features

- **Interactive REPL** — enter expressions one at a time and see results immediately
- **Batch mode** — execute a `.lisp` file; only `PRINT` output is shown
- **Verbose batch mode** — execute a `.lisp` file and print the result of every expression
- **Arithmetic**: `+`, `-`, `*`, `/`, `MAX`, `MIN` (variadic)
- **Comparison**: `=`, `/=`, `<`, `>`, `<=`, `>=` (chainable)
- **List operations**: `LIST`, `CAR`, `CDR`, `NTH`, `LENGTH`, `ATOM`
- **Control flow**: `IF`, `WHILE`, `BRK`
- **Variables**: `SET`, `INC`, `DEC`
- **I/O**: `PRINT`
- **Quote**: `QUOTE` / `'` — suppress evaluation
- Comprehensive error handling and reporting

---

## Project Structure

```
C_SW/
├── src/
│   ├── main.c          # Entry point, execution modes
│   ├── tokenizer.c/h   # Lexical analysis (tokenizer)
│   ├── parser.c/h      # Recursive-descent parser → AST
│   ├── s_exp.c/h       # S-expression (AST) node types
│   ├── eval.c/h        # Expression evaluation engine
│   ├── value.c/h       # Runtime value representation
│   ├── env.c/h         # Environment / variable store
│   ├── buildins.c/h    # Built-in (primitive) functions
│   ├── errors.c/h      # Error codes and messages
│   ├── file_io.c/h     # File loading utilities
│   └── utils.c/h       # General helper functions
├── test.lisp           # General tests and examples
├── primitive_test.lisp # List primitive tests
├── test_bsort.lisp     # Bubble sort demo
├── test_quote.lisp     # Quote operator demo
├── makefile            # Linux/Unix build file
└── makefile.win        # Windows build file
```

---

## Building

### Prerequisites

- GCC (any recent version)
- GNU Make (Linux/macOS) or NMAKE / compatible Make (Windows)

### Linux / macOS

```bash
make        # compile to lisp.exe
make clean  # remove build artifacts
make rebuild # clean then compile
```

### Windows

```cmd
make -f makefile.win
```

The executable is created as `lisp.exe` in the project root.

---

## Usage

### Interactive Mode

```bash
./lisp.exe
```

A `> ` prompt is displayed. Type a LISP expression and press **Enter** to evaluate it.

```
> (+ 1 2 3)
6
> (set 'x 10)
10
> (print x)
10
> (quit)
```

### Batch Mode (file input)

```bash
./lisp.exe <file.lisp>
```

Executes all expressions in the file. Only output from explicit `PRINT` calls is shown.

### Verbose Batch Mode

```bash
./lisp.exe <file.lisp> -v
```

Executes the file and prints the result of **every** expression, not just `PRINT` calls.

---

## Built-in Reference

### Arithmetic

| Expression | Description |
|---|---|
| `(+ a b ...)` | Sum of arguments |
| `(- a b ...)` | Difference (or negation for one arg) |
| `(* a b ...)` | Product |
| `(/ a b)` | Integer division |
| `(MAX a b ...)` | Maximum value |
| `(MIN a b ...)` | Minimum value |

### Comparison

| Expression | Description |
|---|---|
| `(= a b ...)` | All equal |
| `(/= a b ...)` | All different |
| `(< a b ...)` | Strictly increasing |
| `(> a b ...)` | Strictly decreasing |
| `(<= a b ...)` | Non-decreasing |
| `(>= a b ...)` | Non-increasing |

Returns `T` (true) or `NIL` (false).

### List Operations

| Expression | Description |
|---|---|
| `(LIST a b ...)` | Create a list from arguments |
| `(CAR list)` | First element |
| `(CDR list)` | All elements except the first |
| `(NTH i list)` | Element at index `i` (0-based) |
| `(LENGTH list)` | Number of elements |
| `(ATOM x)` | `T` if `x` is not a list, `NIL` otherwise |

### Variables

| Expression | Description |
|---|---|
| `(SET 'var expr)` | Bind `var` to the value of `expr` |
| `(SET (NTH i list) expr)` | Update element `i` of `list` in-place |
| `(INC 'var)` | Increment integer variable by 1 |
| `(INC 'var n)` | Increment integer variable by `n` |
| `(DEC 'var)` | Decrement integer variable by 1 |
| `(DEC 'var n)` | Decrement integer variable by `n` |

### Control Flow

| Expression | Description |
|---|---|
| `(IF cond then)` | Evaluate `then` if `cond` is truthy |
| `(IF cond then else)` | Evaluate `then` or `else` based on `cond` |
| `(WHILE cond body ...)` | Repeat `body` while `cond` is truthy |
| `(BRK)` | Break out of the enclosing `WHILE` loop |
| `(QUIT)` | Exit the interpreter |

### I/O

| Expression | Description |
|---|---|
| `(PRINT expr)` | Print the value of `expr` followed by a newline |

### Quote

| Expression | Description |
|---|---|
| `(QUOTE expr)` or `'expr` | Return `expr` unevaluated |

---

## Examples

### Arithmetic

```lisp
(+ 2 5 (* 2 3))       ; => 13
(- 100 (* 5 10) 10)   ; => 40
(MAX 3 7 2)           ; => 7
```

### Variables and Loops

```lisp
(set 'i 0)
(while (< i 5)
  (print i)
  (inc 'i))
; prints 0 1 2 3 4
```

### Lists

```lisp
(set 'lst (list 10 20 30 40))
(print (car lst))        ; 10
(print (nth 2 lst))      ; 30
(print (length lst))     ; 4
```

### Bubble Sort

```lisp
(set 'arr '(5 1 7 3 2 6 4))
(set 'len (length arr))
(set 'swapped T)
(while swapped
  (set 'i 1)
  (set 'swapped nil)
  (while (< i len)
    (set 'a (nth (- i 1) arr))
    (set 'b (nth i arr))
    (if (> a b)
      (while T                       ; block: execute once then break
        (set (nth (- i 1) arr) b)
        (set (nth i arr) a)
        (set 'swapped T)
        (brk)))
    (inc 'i)))
(print arr)   ; (1 2 3 4 5 6 7)
```

---

## Architecture

The interpreter follows a classic pipeline:

```
Input (stdin / file)
      │
      ▼
 Tokenizer          lexical analysis → tokens
      │
      ▼
  Parser            syntax analysis  → AST (S-expressions)
      │
      ▼
 Evaluator          tree-walk evaluator → values
      │
      ▼
 Output (REPL / PRINT)
```

- **Memory management** is explicit (`malloc` / `free`). Every complex type has a dedicated cleanup function; deep copies are used where necessary to maintain value immutability.
- **Environment** is a simple dynamic array of name→value pairs, with `T` and `NIL` pre-loaded as global constants.
- **Error propagation** uses a dedicated `VALUE_ERROR` type so errors bubble up through evaluation naturally.

---

## License

This project is provided for educational purposes.
