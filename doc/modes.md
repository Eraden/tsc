# Modes

## Relaxed

* Accept `any` as type.
* Accept `undefined` types as `any`.
* JavaScript will be generated regardless of errors and warnings.

## Strict (default)

* Accept `any` as type.
* Unknown types will not be accepted.
* JavaScript will not be generated if error occurred.
* JavaScript will be generated regardless of warnings.

## Strong

* Does not accept `any` as type.
* Unknown types will not be accepted.
* JavaScript will not be generated if errors or warnings occurred.
