

function marked() {
  return function ret(variable) {
    variable.__marked__ = true;
    return variable;
  };
}

@marked()
var a = 1;