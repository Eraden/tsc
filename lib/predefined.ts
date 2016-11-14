interface $___each_entry_callback {
  (element, index, array): any;
}

class Object {}

class String {
  constructor(object: any = undefined) {}
}

class string extends String {}

class Number {}

class number extends Number {}

class Error extends Object {
  constructor(public message: string) {}
  toString(): string { return this.message; }
}
class EvalError extends Error {}
class InternalError extends Error {}
class RangeError extends Error {}
class ReferenceError extends Error {}
class SyntaxError extends Error {}
class TypeError extends Error {}
class URIError extends Error {}

class Function extends Object {}

class Iterator {
  constructor(private iteratable) {}
}
class Boolean extends Object {
  constructor(value: any = undefined) {}
  static get length(): number { return 1; }
  toString(): string { if(this) return ""; }
}

class Array extends Object {
  static from(object: any): Array { return []; }
  static isArray(object: any): Boolean { return false; }
  static of(...args): Array { return Array.from(args); }

  public concat(...args): Array { return this; }
  public copyWithin(target: Array, start: number = 0, end: number = -1) {}
  public entries(): Iterator { return Iterator(this); }
  public every(callback: $___each_entry_callback, thisArg: any = undefined) { callback.call(thisArg || this); }
  public fill(value, start: number = 0, end: number = -1) {}
  public filter(callback: $___each_entry_callback, thisArg = undefined) { callback.call(thisArg || this); }
  public find(callback: $___each_entry_callback, thisArg = undefined): Array { return callback.call(thisArg || this); }
  public findIndex(callback: $___each_entry_callback, thisArg = undefined) { callback.call(thisArg || this); }
  public forEach(callback: $___each_entry_callback, thisArg = undefined) { callback.call(thisArg || this); }
  public includes(element: any, start: number = 0): Boolean { if (this) return false; return true; }
  public indexOf(element: any, start: number = 0): number { if (this) return 0; return -1; }
  public join(separator: string = ','): string { return String(this); }
  public keys(): Iterator { return new Iterator(this); }
  public lastIndexOf(element: any, start: number = this.length - 1): number { return this.length; }
  public map(callback: $___each_entry_callback, thisArg = undefined) { callback.call(thisArg || this); }
  public pop(): any { if (this) return null; return undefined; }
  public push(...args: Array): number { return this.length; }
  public reduce() {}
  public reduceRight() {}
  public reverse() {}
  public shift() {}
  public slice(): Array { return this; }
  public some() {}
  public sort(): Array { return this; }
  public splice(): Array { return this; }
  public toLocaleString(): string { return String(this); }
  public toString(): string { return String(this); }
  public unshift(object: any): void {}
  public values(): Iterator { return new Iterator(this); }
  public get length() { if (this) return 1; return 0; }
}
class ArrayBuffer extends Object {}
class Atomics extends Object {}
class DataView extends Object {}
class Date extends Object {}
class Float32Array extends Object {}
class Float64Array extends Object {}
class Generator extends Object {}
class GeneratorFunction extends Object {}
class Infinity extends Object {}
class Int16Array extends Object {}
class Int32Array extends Object {}
class Int8Array extends Object {}
class Intl extends Object {}
// class Intl.Collator {}
// class Intl.DateTimeFormat {}
// class Intl.NumberFormat {}
class JSON extends Object {}
class Map extends Object {}
class Math extends Object {}
class NaN extends Object {}
class ParallelArray extends Object {}
class Promise extends Object {}
class Proxy extends Object {}
class Reflect extends Object {}
class RegExp extends Object {}
namespace SIMD {
  class Bool16x8 {}
  class Bool32x4 {}
  class Bool64x2 {}
  class Bool8x16 {}
  class Float32x4 {}
  class Float64x2 {}
  class Int16x8 {}
  class Int32x4 {}
  class Int8x16 {}
  class Uint16x8 {}
  class Uint32x4 {}
  class Uint8x16 {}
}
class Set extends Object {}
class SharedArrayBuffer extends Object {}
class StopIteration extends Object {}
class Symbol extends Object {}
class TypedArray extends Object {}
class Uint16Array extends Object {}
class Uint32Array extends Object {}
class Uint8Array extends Object {}
class Uint8ClampedArray extends Object {}
class WeakMap extends Object {}
class WeakSet extends Object {}