var globalScopeVariable = 1;

function fn() {
  var fnA = globalScopeVariable;
  fnA += 3;
  return fnA;
}

{
  const subScopeVariable = globalScopeVariable;
  function scopeFn() {
    return subScopeVariable;
  }
}