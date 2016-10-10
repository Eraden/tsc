# TypeScript Compiler [![CircleCI](https://circleci.com/gh/Eraden/tsc.svg?style=shield&circle-token=14ccc05cdfb02c1568196ff10be25121a8bddaf7)](https://circleci.com/gh/Eraden/tsc)

This is C typescript compiler. It can compile typescript file faster than normal tsc provided by Microsoft.

## Soft requirements

* `check` - if build with `-DTSC_TEST=1` requires test framework for c language, this creates tests which check internal pre-compiler functionalities

## Build

```bash
git clone git@github.com:Eraden/tsc.git
cd tsc
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=$(which clang) -DCMAKE_BUILD_TYPE=Debug ..
make -j9
ctest
./tsc_test
```

## Progress

### Compiler

- [X] Read file
- [X] Log files
- [ ] `-e environment`
- [ ] `tsconfig` file
- [ ] `--strong` mode
- [ ] `--strict` mode
- [ ] `--relax` mode
- [X] `-o file` write to file
- [ ] `--tokens` print only types locations and names
- [X] `-i` reading from standard input

### Typings

- [ ] Reading `typings` directory
- [ ] Process `.ts.d` files
- [ ] Process `///<reference source="" />`

### Pre-compiler

- [ ] `//#if`
- [ ] `//#else`
- [ ] `//#endif`
- [ ] `//#ifdefined`
- [ ] `-DVARIABLE=VALUE` flag

### Comments

- [X] Parse single line comments
- [X] Parse multi-line comments
- [ ] `--preserveComments` flag

### Variables

- [X] Parse `var` with value
- [ ] Print `var` with value
- [X] Parse `let` with value
- [ ] Print `let` with value
- [X] Parse `const` with value
- [ ] Print `const` with value

### Loops

- [ ] Parse `while` loop
- [ ] Print `while` loop
- [ ] Parse `do {} while` loop
- [ ] Print `do {} while` loop
- [X] Parse `for i` loop
- [ ] Print `for i` loop
- [X] Parse `for in` loop
- [ ] Print `for in` loop
- [X] Parse `for of` loop
- [ ] Print `for of` loop

### Logical gates

- [X] Parse `if` statement
- [ ] Print `if` statement
- [X] Parse `if` conditions
- [ ] Print `if` conditions
- [X] Parse `if` body
- [ ] Print `if` body
- [X] Parse `else` statement
- [ ] Print `else` statement
- [X] Parse `else` body
- [ ] Print `else` body
- [X] Parse `switch` statement
- [ ] Print `switch` statement
- [X] Parse `case` statement
- [ ] Print `case` statement
- [X] Parse `default` statement
- [ ] Print `default` statement
- [X] Error on double `default` statement declaration

### Decorators

- [ ] Parse class decorator
- [ ] Print class decorator
- [ ] Parse function decorator
- [ ] Print function decorator
- [ ] Parse class method decorator
- [ ] Print class method decorator
- [ ] Parse class field decorator
- [ ] Print class field decorator
- [ ] Parse argument decorator
- [ ] Print argument decorator

### Interface

- [X] Parse empty interface
- [ ] Parse interface field requirement
- [ ] Parse interface method prototype

### Class

- [X] Parse empty class
- [ ] Print empty class
- [X] Support `extends`
- [ ] Print `extends` as `prototype = Object.create(Parent)`
- [ ] Support `implements`
- [ ] Support `friend`
- [ ] Parse class `constructor` without `arguments`
- [ ] Print class `constructor` without `arguments`
- [ ] Parse `super` in `constructor`
- [ ] Print `super` in `constructor` as `Parent.call(this)`
- [ ] Pass `arguments` to `super` in `constructor` as `Parent.call(this, a, b, c)`

### Methods

- [X] Parse class method
- [ ] Print class method
- [X] Parse class method arguments
- [ ] Print class method arguments
- [ ] Parse method `super`
- [ ] Print method `super` as `Parent.prototype.method.call(this)`
- [ ] Print method `super` with `arguments`

### Fields

- [X] Parse class field
- [ ] Print class field

### Function

- [X] Parse empty `function`
- [ ] Print empty `function`
- [X] Parse arguments
- [ ] Print arguments
- [X] Parse `function` body
- [ ] Print `function` body

### Validations

- [X] Types register
- [ ] Print parse warnings
- [ ] Print warning about unused variables
- [ ] Print warning about unused functions

### Output

- [ ] Generate single string for single file (large memory footprint)
- [ ] Generate single string for all fields (large memory footprint)
- [ ] Print immediately every code part (single file mode)
- [ ] Print immediately every code part (multiple-file mode with sync)
- [ ] Write to single file to single file with generating string (large memory footprint)
- [ ] Write to single file to multiple files with generating string (sync mode + large memory footprint)
- [ ] Write to single file from single file without generating string
- [ ] Write to single file from multiple files without generating string

# Modes

## Relax

* Accept `any` as type.
* Accept undefined types as `any`.
* JavaScript will be generated regardless of errors and warnings.

## Strict

* Accept `any` as type.
* Unknown types will not be accepted.
* JavaScript will be generated regardless of errors and warnings.

## Strong

* Does not accept `any` as type.
* Unknown types will not be accepted.
* JavaScript will not be generated if errors or warnings occurred.


# Typescript

```typescript
//#if dev
function log(...args) {
  console.log.apply(console, ...args);
}
//#else
declare var log;
//#endif

class Food {
  public energy: number = 20;
}

interface Living {
  eat(food: Food): void;
  die(): boolean;
}

class Animal {
}

class Cat extends Animal implements Living {
  private alive: boolean = true;
  private energy: number = 100;
  constructor() {
    super();
  }

  eat(food: Food): void {
    this.energy += food.energy;
  }

  die(): boolean {
    return !(this.alive = false);
  }
}
```
