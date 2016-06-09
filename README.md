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
- [ ] -e environment
- [ ] .tsc file

### Precompiler

- [ ] #if #else #endif
- [ ] #ifdefined
- [ ] -D flag

### Decorators

- [X] Parse class decorator
- [X] Print class decorator
- [ ] Parse funtion decorator
- [ ] Print function decorator
- [ ] Parse class method decorator
- [ ] Print class method decorator
- [ ] Parse class field decorator
- [ ] Print class field decorator
- [ ] Parse argument decorator
- [ ] Print argument decorator

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

### Function

- [X] Parse empty function
- [X] Print empty function
- [X] Parse arguments
- [X] Print arguments

# Typescript

```typescript
#if dev
function log(...args) {
  console.log.apply(console, ...args);
}
#else
declare var log;
#endif

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
