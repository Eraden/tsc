# Syntax

## non-standard

Syntax sugar for symbols.

* backward compatibility (limited)
* easier to use
* nice look

```typescript
const symbol = (("symbol"));

class Example {
  [(("beacon-like-symbol-propety"))] = 1;
  [(("beacon-like-symbol-method"))](): number {
    return 1;
  }
}
```

* Must register symbol for current file
* Cannot export symbol to other file
* Must have unique names
* Must validate name uniqueness
* Must recognize `((` and `))` as text token

```c
TS_register_file_symbol(TSParserToken *symbol);
```