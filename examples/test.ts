function a() {
  // inline comment
  if (1) { return 1; }
}

/**
 * Variables
 * @type {number}
 */
let value = 1;
const stale = 2;
var variable = 3;

class Descriptor {
}

function Component(data: Object, desc: Descriptor={}, s ="", ...rest) {
  if (desc != null) {
    return 2;
  } else
    return 3;
}


class Some {
  enumerable: boolean = false;

  constructor() {
    if (1) this.enumerable = true;
  }
}

@Component({
})
export default class Test extends Some {
  public field1: number = 1;
  protected field2 = /test/;
  private field3: Date;
  field;

  constructor() {
    super();
  }

  public method() {
    return this.field;
  }
}

export class Example {
}
