{
  function normalizeOpts(options/*, …options*/) {
    var result = Object.create(null);
    [].forEach.call(arguments, function (options) {
      if (options == null) return;
      process(Object(options), result);
    });
    return result;
  }

  function process(src, obj) {
    var key;
    for (key in src) if (src.hasOwnProperty(key)) obj[key] = src[key];
  }

  function value(value) {
    if (value == null) throw new TypeError("Cannot use null or undefined");
    return value;
  }

  function assign(destination, src/*, …srcn*/) {
    var error, i, l = Math.max(arguments.length, 2), assign;
    destination = Object(value(destination));
    assign = function (key) {
      try {
        destination[key] = src[key];
      } catch (e) {
        if (!error) error = e;
      }
    };
    for (i = 1; i < l; ++i) {
      src = arguments[i];
      Object.keys(src).forEach(assign);
    }
    if (error !== undefined) throw error;
    return destination;
  }

  function isCallable(obj) {
    return typeof obj === 'function';
  }

  var contains = String.prototype.includes;

  function D_lib(descriptor, value/*, options*/) {
    var c, e, w, options, desc;
    if ((arguments.length < 2) || (typeof descriptor !== 'string')) {
      options = value;
      value = descriptor;
      descriptor = null;
    } else {
      options = arguments[2];
    }
    if (descriptor == null) {
      c = w = true;
      e = false;
    } else {
      c = contains.call(descriptor, 'c');
      e = contains.call(descriptor, 'e');
      w = contains.call(descriptor, 'w');
    }

    desc = {value: value, configurable: c, enumerable: e, writable: w};
    return !options ? desc : assign(normalizeOpts(options), desc);
  }

  D_lib['gs'] = function (descriptor, get, set/*, options*/) {
    var c, e, options, desc;
    if (typeof descriptor !== 'string') {
      options = set;
      set = get;
      get = descriptor;
      descriptor = null;
    } else {
      options = arguments[3];
    }
    if (get == null) {
      get = undefined;
    } else if (!isCallable(get)) {
      options = get;
      get = set = undefined;
    } else if (set == null) {
      set = undefined;
    } else if (!isCallable(set)) {
      options = set;
      set = undefined;
    }
    if (descriptor == null) {
      c = true;
      e = false;
    } else {
      c = contains.call(descriptor, 'c');
      e = contains.call(descriptor, 'e');
    }

    desc = {get: get, set: set, configurable: c, enumerable: e};
    return !options ? desc : assign(normalizeOpts(options), desc);
  };

  var validTypes = {object: true, symbol: true};

  function isImplemented() {
    var symbol;
    if (typeof Symbol !== 'function') return false;
    //noinspection TypeScriptValidateTypes
    symbol = Symbol('test symbol');
    try {
      String(symbol);
    } catch (e) {
      return false;
    }

    if (!validTypes[typeof Symbol['iterator']]) return false;
    if (!validTypes[typeof Symbol['toPrimitive']]) return false;
    //noinspection RedundantIfStatementJS
    if (!validTypes[typeof Symbol['toStringTag']]) return false;

    return true;
  }

  function isSymbol(x) {
    if (!x) return false;
    if (typeof x === 'symbol') return true;
    if (!x.constructor) return false;
    if (x.constructor.name !== 'Symbol') return false;
    return (x[x.constructor['toStringTag']] === 'Symbol');
  }

  function validateSymbol(value) {
    if (!isSymbol(value)) throw new TypeError(value + " is not a symbol");
    return value;
  }

  function polyfill() {
    var create = Object.create,
      defineProperties = Object.defineProperties,
      defineProperty = Object.defineProperty,
      objPrototype = Object.prototype,
      NativeSymbol, SymbolPolyfill, HiddenSymbol,
      globalSymbols = create(null),
      isNativeSafe;
    if (typeof Symbol === 'function') {
      NativeSymbol = Symbol;
      try {
        String(NativeSymbol());
        isNativeSafe = true;
      } catch (ignore) {
      }
    }

    var generateName = (function () {
      var created = create(null);
      return function (desc) {
        var postfix = 0, name, ie11BugWorkaround;
        while (created[desc + (postfix || '')]) ++postfix;
        desc += (postfix || '');
        created[desc] = true;
        name = '@@' + desc;
        defineProperty(objPrototype, name, D_lib['gs'](null, function (value) {
          if (ie11BugWorkaround) return;
          defineProperty(this, name, D_lib(value));
          ie11BugWorkaround = false;
        }));
        return name;
      };
    }());

    HiddenSymbol = function Symbol(description) {
      if (this instanceof HiddenSymbol) throw new TypeError('TypeError: Symbol is not a constructor');
      return SymbolPolyfill(description);
    };

    SymbolPolyfill = function Symbol(description) {
      var symbol;
      if (this instanceof Symbol) throw new TypeError('TypeError: Symbol is not a constructor');
      if (isNativeSafe) return NativeSymbol(description);
      symbol = create(HiddenSymbol.prototype);
      description = (description === undefined ? '' : String(description));
      return defineProperties(symbol, {
        __description__: D_lib('', description),
        __name__: D_lib('', generateName(description))
      });
    };
    Object.defineProperty(window, 'Symbol', {value: SymbolPolyfill});

    defineProperties(SymbolPolyfill, {
      'for': D_lib(function (key) {
        if (globalSymbols[key]) return globalSymbols[key];
        return (globalSymbols[key] = SymbolPolyfill(String(key)));
      }),
      keyFor: D_lib(function (s) {
        var key;
        validateSymbol(s);
        for (key in globalSymbols)
          if (globalSymbols.hasOwnProperty(key) && globalSymbols[key] === s)
            return key;
      }),
      hasInstance: D_lib('', (NativeSymbol && NativeSymbol.hasInstance) || SymbolPolyfill('hasInstance')),
      isConcatSpreadable: D_lib('', (NativeSymbol && NativeSymbol.isConcatSpreadable) ||
        SymbolPolyfill('isConcatSpreadable')),
      iterator: D_lib('', (NativeSymbol && NativeSymbol.iterator) || SymbolPolyfill('iterator')),
      match: D_lib('', (NativeSymbol && NativeSymbol.match) || SymbolPolyfill('match')),
      replace: D_lib('', (NativeSymbol && NativeSymbol.replace) || SymbolPolyfill('replace')),
      search: D_lib('', (NativeSymbol && NativeSymbol.search) || SymbolPolyfill('search')),
      species: D_lib('', (NativeSymbol && NativeSymbol.species) || SymbolPolyfill('species')),
      split: D_lib('', (NativeSymbol && NativeSymbol.split) || SymbolPolyfill('split')),
      toPrimitive: D_lib('', (NativeSymbol && NativeSymbol.toPrimitive) || SymbolPolyfill('toPrimitive')),
      toStringTag: D_lib('', (NativeSymbol && NativeSymbol.toStringTag) || SymbolPolyfill('toStringTag')),
      unscopables: D_lib('', (NativeSymbol && NativeSymbol.unscopables) || SymbolPolyfill('unscopables'))
    });

    defineProperties(HiddenSymbol.prototype, {
      constructor: D_lib(SymbolPolyfill),
      toString: D_lib('', function () {
        return this.__name__;
      })
    });

    defineProperties(SymbolPolyfill.prototype, {
      toString: D_lib(function () {
        return 'Symbol (' + validateSymbol(this).__description__ + ')';
      }),
      valueOf: D_lib(function () {
        return validateSymbol(this);
      })
    });
    defineProperty(SymbolPolyfill.prototype, SymbolPolyfill['toPrimitive'], D_lib('', function () {
      var symbol = validateSymbol(this);
      if (typeof symbol === 'symbol') return symbol;
      return symbol.toString();
    }));
    defineProperty(SymbolPolyfill.prototype, SymbolPolyfill['toStringTag'], D_lib('c', 'Symbol'));

    defineProperty(HiddenSymbol.prototype, SymbolPolyfill['toStringTag'],
      D_lib('c', SymbolPolyfill.prototype[SymbolPolyfill['toStringTag']]));

    defineProperty(HiddenSymbol.prototype, SymbolPolyfill['toPrimitive'],
      D_lib('c', SymbolPolyfill.prototype[SymbolPolyfill['toPrimitive']]));
  }

  if (!isImplemented()) {
    polyfill();
  }
}