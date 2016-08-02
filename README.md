# TypeScript Compiler

This is C typescript compiler. It can compile typescript file faster than normal tsc provided by Microsoft.

## Pre-requirements

* `libunistring-dev` for unicode support

## Build

```bash
git clone git@github.com:Eraden/tsc.git
cd tsc
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=$(which clang) -DCMAKE_C_FLAGS="-O0" -DCMAKE_BUILD_TYPE=Debug ..
make -j9
./tsc examples
```

## Progress

### Compiler

- [ ] Read file
- [X] Log files
- [ ] `-e environment`
- [ ] `tsconfig` file
- [ ] `--strong` mode
- [ ] `--strict` mode
- [ ] `--relax` mode
- [ ] `-o file` write to file
- [ ] `--tokens` print only types locations and names

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

- [ ] Parse single line comments
- [ ] Parse multi-line comments
- [ ] `--preserveComments` flag

### Variables

- [X] Parse `var` with value
- [X] Print `var` with value
- [X] Parse `let` with value
- [X] Print `let` with value
- [X] Parse `const` with value
- [X] Print `const` with value

### Loops

- [ ] Parse `while` loop
- [ ] Print `while` loop
- [ ] Parse `do {} while` loop
- [ ] Print `do {} while` loop
- [ ] Parse `for i` loop
- [ ] Print `for i` loop
- [ ] Parse `for in` loop
- [ ] Print `for in` loop
- [ ] Parse `for of` loop
- [ ] Print `for of` loop

### Logical gates

- [X] Parse `if` statement
- [X] Print `if` statement
- [X] Parse `if` conditions
- [X] Print `if` conditions
- [X] Parse `if` body
- [X] Print `if` body
- [X] Parse `else` statement
- [X] Print `else` statement
- [X] Parse `else` body
- [X] Print `else` body
- [ ] Parse `switch` statement
- [ ] Print `switch` statement
- [ ] Parse `case` statement
- [ ] Print `case` statement
- [ ] Parse `default` statement
- [ ] Print `default` statement
- [ ] Error on double `default` statement declaration

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

- [ ] Parse empty interface
- [ ] Parse interface field requirement
- [ ] Parse interface method prototype

### Class

- [X] Parse empty class
- [X] Print empty class
- [ ] Support `extends`
- [X] Print `extends` as `prototype = Object.create(Parent)`
- [ ] Support `implements`
- [ ] Support `friend`
- [ ] Parse class `constructor` without `arguments`
- [ ] Print class `constructor` without `arguments`
- [ ] Parse `super` in `constructor`
- [ ] Print `super` in `constructor` as `Parent.call(this)`
- [ ] Pass `arguments` to `super` in `constructor` as `Parent.call(this, a, b, c)`

### Methods

- [ ] Parse class method
- [ ] Print class method
- [ ] Parse class method arguments
- [ ] Print class method arguments
- [ ] Parse method `super`
- [ ] Print method `super` as `Parent.prototype.method.call(this)`
- [ ] Print method `super` with `arguments`

### Fields

- [ ] Parse class field
- [ ] Print class field

### Function

- [X] Parse empty `function`
- [X] Print empty `function`
- [X] Parse arguments
- [X] Print arguments
- [X] Parse `function` body
- [X] Print `function` body

### Validations

- [ ] Types register
- [ ] Print parse warnings
- [ ] Print warning about unused variables
- [ ] Print warning about unused functions

### Output

- [X] Generate single string for single file (large memory footprint)
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
