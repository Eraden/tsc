```typescript
class Example {
  public some = 1;
  protected other = 2;
  private last = 3;
  
  data() {
    return this.other;
  }
}

class Test {
  public example: Example = new Example;
  
  get some() {
    return this.example['other']; // <- parse accessor ???
  }
}
```

```javascript
(function () {
  var PRIVATE_MAP = new WeakMap();
  var PROTECTED_MAP = new WeakMap();
  
  function _private_(accessor, instance, field, value) {
    if (accessor != instance)
      throw new TypeError('Private field are available only from their owner!');
    if (!PRIVATE_MAP.has(instance)) PRIVATE_MAP.set(instance, {});
    var data = PRIVATE_MAP.get(instance);
    if (arguments.length === 4) data[field] = value;
    return data[field];
  }
  
  function _protected_(accessor, instance, field, value) {
    if (!(accessor instanceof instance.constructor))
      throw new TypeError('Protected fields are available only from sub-classes!');
    if (!PROTECTED_MAP.has(instance)) PROTECTED_MAP.set(instance, {});
    var data = PROTECTED_MAP.get(instance);
    if (arguments.length === 4) data[field] = value;
    return data[field];
  }
  
  function Example() {
    if (!(this instanceof Example)) 
      throw new TypeError('Class constructor A cannot be invoked without \'new\'');
    this.some = 1;
    _private_(this, this, 'last', 3);
    _protected_(this, this, 'other', 2);
  }
  Object.defineProperties(Example.prototype, {
    constructor: { get: function () { return Example; } },
    data: { value: function () {
      return _protected_(this, this, 'other');
    } }
  });
  
  function Test() {
    if (!(this instanceof Test)) 
      throw new TypeError('Class constructor A cannot be invoked without \'new\'');
    this.example = new Example();
  }
  Object.defineProperties(Test.prototype, {
    constructor: { get: function () { return Test; } },
    some: {
      get: function() {
        return _protected_(this, this.example, 'other');
      }
    }
  });
}());
```