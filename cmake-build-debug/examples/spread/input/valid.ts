function first(...args) {}
function second(firstArgument, ...functionRest) {}

class Example {
  constructor(...constructorArgs) {}
  method(methodArgument, ...methodRest) {}
}

class Child extends Example {
  secondMethod(...secondMethodArgs) {
    this.method(1, ...secondMethodArgs);
  }
}
