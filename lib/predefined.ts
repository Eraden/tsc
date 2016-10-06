const undefined = void(0);

class Iterator {
  constructor(private iteratable) {}
}

class Array {
  static from(object: any): Array { return []; }
  static isArray(object: any): Boolean { return false; }
  static of(...args): Array { return Array.from(args); }

  public concat(...args): Array { return this; }
  public copyWithin(target: Array, start: number = 0, end: number = -1) {}
  public entries(): Iterator { return Iterator(this); }
  public every(callback: (element, index, array) => {}, thisArg: any = undefined) { callback.call(thisArg || this); }
  public fill(value, start: number = 0, end: number = -1) {}
  public filter(callback: (element, index, array) => {}, thisArg = undefined) { callback.call(thisArg || this); }
  public find(callback: (element, index, array) => {}, thisArg = undefined): Array<any> { callback.call(thisArg || this); }
  public findIndex(callback: (element, index, array) => {}, thisArg = undefined) { callback.call(thisArg || this); }
  public forEach(callback: (element, index, array) => {}, thisArg = undefined) { callback.call(thisArg || this); }
  public includes() {}
  public indexOf() {}
  public join() {}
  public keys() {}
  public lastIndexOf() {}
  public map(callback: (element, index, array) => {}, thisArg = undefined) { callback.call(thisArg || this); }
  public pop() {}
  public push() {}
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
}
class ArrayBuffer {}
class Atomics {}
class Boolean {}
class DataView {}
class Date {}
class Error {}
class EvalError {}
class Float32Array {}
class Float64Array {}
class Function {}
class Generator {}
class GeneratorFunction {}
class Infinity {}
class Int16Array {}
class Int32Array {}
class Int8Array {}
class InternalError {}
// class Intl {}
// class Collator {}
// class Intl.Collator {}
// class Intl.DateTimeFormat {}
// class Intl.NumberFormat {}
class JSON {}
class Map {}
class Math {}
class NaN {}
class Number {}
class Object {}
class ParallelArray {}
class Promise {}
class Proxy {}
class RangeError {}
class ReferenceError {}
class Reflect {}
class RegExp {}
class SIMD {}
// class SIMD.Bool16x8 {}
// class SIMD.Bool32x4 {}
// class SIMD.Bool64x2 {}
// class SIMD.Bool8x16 {}
// class SIMD.Float32x4 {}
// class SIMD.Float64x2 {}
// class SIMD.Int16x8 {}
// class SIMD.Int32x4 {}
// class SIMD.Int8x16 {}
// class SIMD.Uint16x8 {}
// class SIMD.Uint32x4 {}
// class SIMD.Uint8x16 {}
class Set {}
class SharedArrayBuffer {}
class StopIteration {}
class String {
  constructor(object: any = undefined) {}
}
class Symbol {}
class SyntaxError {}
class TypeError {}
class TypedArray {}
class URIError {}
class Uint16Array {}
class Uint32Array {}
class Uint8Array {}
class Uint8ClampedArray {}
class WeakMap {}
class WeakSet {}