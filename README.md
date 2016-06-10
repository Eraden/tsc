# TypeScript Compiler

This is C typescript compiler. It can compile typescript file faster than normal tsc provided by Microsoft.

## Build

```bash
git clone git@github.com:Eraden/tsc.git
cd tsc
mkdir build
cd build
cmake ..
make -j9
./tsc examples
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

### Typings

- [ ] Reading `typings` directory
- [ ] Process `.ts.d` files
- [ ] Process `///<reference source="" />`

### Pre-compiler

- [ ] `#if #else #endif`
- [ ] `#ifdefined`
- [ ] `-DVARIABLE=VALUE` flag

### Comments

- [ ] Parse single line comments
- [ ] Parse multi-line comments
- [ ] `--preserveComments` flag

### Decorators

- [X] Parse class decorator
- [X] Print class decorator
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
- [X] Support `extends`
- [ ] Support `implements`
- [ ] Support `friend`
- [ ] Use class constructor
- [ ] Use constructor `super`

### Methods

- [ ] Parse class method
- [ ] Print class method
- [ ] Parse class method arguments
- [ ] Print class method arguments
- [ ] Support method `super`

### Fields

- [X] Parse class field
- [X] Print class field

### Function

- [X] Parse empty function
- [X] Print empty function
- [X] Parse arguments
- [X] Print arguments

### Validations

- [ ] Types register
- [ ] Print parse warnings
- [ ] Print warning about unused variables
- [ ] Print warning about unused functions

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

class Food {}

interface Living {
  eat(food: Food): void;
  die(): boolean;
}

class Animal {
}

class Cat extends Animal implements Living {
  constructor() {
    super();
  }

  eat(food: Food): void {
  }

  die(): boolean {
    return true;
  }
}
```
