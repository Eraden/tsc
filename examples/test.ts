class Descriptor {}
class Some {}

function Component(data: Object, desc: Descriptor={}, s ="", ...rest) {
  if (desc) {
  } else
    return 1;
}

@Component({
})
export default class Test extends Some {
  public field1: number = 1;
  protected field2 = /test/;
  private field3: Date;
  field;

  public constructor() {
    super();
  }
}

export class Example {
}
