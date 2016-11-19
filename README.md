# TypeScript Compiler [![CircleCI](https://circleci.com/gh/Eraden/tsc.svg?style=shield&circle-token=14ccc05cdfb02c1568196ff10be25121a8bddaf7)](https://circleci.com/gh/Eraden/tsc)

This is C typescript compiler. It can compile typescript file faster than normal tsc provided by Microsoft.

## Soft requirements

* `check` - if build with `-DTSC_TEST=1` requires test framework for c language, this creates tests which check internal pre-compiler functionalities

Please do not use clang-3.8 because build crash on it.

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

