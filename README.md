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

- [ ] Read file
- [X] Log files
- [ ] `-e environment`
- [ ] `tsconfig` file
- [ ] `--strong` mode
- [ ] `--strict` mode
- [ ] `--relax` mode
- [ ] `-o file` write to file

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

### Variables

- [ ] Parse `var` with value
- [ ] Parse `let` with value
- [ ] Parse `const` with value

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

- [ ] Parse empty class
- [ ] Print empty class
- [ ] Support `extends`
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

- [ ] Parse class field
- [ ] Print class field

### Function

- [ ] Parse empty function
- [ ] Print empty function
- [X] Parse arguments
- [X] Print arguments

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
