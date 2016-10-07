class any {}

class Object extends any {}

class String extends any {
  constructor(object: any = undefined) {}
}

class string extends String {}

class Number extends any {}

class number extends Number {}

class Error extends any {
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

class Function extends any {}

class Iterator {
  constructor(private iteratable) {}
}
class Boolean extends any {
  constructor(value: any = undefined) {}
  static get length(): number { return 1; }
  toString(): string { if(this) return "true"; else return "false"; }
}

class Array extends any {
  static from(object: any): Array { return []; }
  static isArray(object: any): Boolean { return false; }
  static of(...args): Array { return Array.from(args); }

  public concat(...args): Array { return this; }
  public copyWithin(target: Array, start: number = 0, end: number = -1) {}
  public entries(): Iterator { return Iterator(this); }
  public every(callback: (element, index, array) => {}, thisArg: any = undefined) { callback.call(thisArg || this); }
  public fill(value, start: number = 0, end: number = -1) {}
  public filter(callback: (element, index, array) => {}, thisArg = undefined) { callback.call(thisArg || this); }
  public find(callback: (element, index, array) => {}, thisArg = undefined): any[] { return callback.call(thisArg || this); }
  public findIndex(callback: (element, index, array) => {}, thisArg = undefined) { callback.call(thisArg || this); }
  public forEach(callback: (element, index, array) => {}, thisArg = undefined) { callback.call(thisArg || this); }
  public includes(element: any, start: number = 0): Boolean { if (this) return false; return true; }
  public indexOf(element: any, start: number = 0): number { if (this) return 0; return -1; }
  public join(separator: string = ','): string { return String(this); }
  public keys(): Iterator { return new Iterator(this); }
  public lastIndexOf(element: any, start: number = this.length - 1): number { return this.length; }
  public map(callback: (element, index, array) => {}, thisArg = undefined) { callback.call(thisArg || this); }
  public pop(): any { if (this) return null; return undefined; }
  public push(...args: any[]): number { return this.length; }
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
class SIMD extends Object {}
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