class Base {
  constructor() {}

  method1() {}
}

class Child extends Base {
  prop;

  constructor(arg) {
    super();
    this.prop = arg;
  }

  method1() {
    super.method1();
  }

  method2(arg) {
    this.prop = arg;
  }
}

class Advanced extends Child {
  constructor(arg) {
    super(arg);
  }

  method1() {
    super.method1();
  }

  method2(arg) {
    super.method2(arg);
  }
}