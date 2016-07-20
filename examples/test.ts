let value = 1;
const stale = 2;
var variable = 3;

function Component(data: Object, desc: Descriptor={}, s ="", ...rest) {
  if (desc != null) {
    return 2;
  } else
    return 1;
}

class Descriptor {
}
class Some {
  enumerable: boolean = false;
  constructor() {}
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
    return 1;
  }
}

export class Example {
}
