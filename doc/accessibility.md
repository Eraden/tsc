# Draft

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
  var Example_some = Symbol('Example some');
  var Example_other = Symbol('Example other');
  var Example_last = Symbol('Example last');
  
  var Example = function Example() {};
  (function (proto) {
    var definition = {};
    definition['some'] = {
      get: function () {
        return this[Example_some];
      },
      set: function (val) {
        this[Example_some] = val;
      }
    };
    definition['other'] = {
        get: function () {
          return this[Example_other];
        },
        set: function (val) {
          this[Example_other] = val;
        }
    };
    definition['last'] = {
        get: function () {
          return this[Example_last];
        },
        set: function (val) {
          this[Example_last] = val;
        }
    };
    Object.defineProperties(proto, definition);
  } (Example.prototype));
}());
```