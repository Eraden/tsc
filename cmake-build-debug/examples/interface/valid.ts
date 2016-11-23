interface CallbackVoidFunction {
  (): void;
}

interface CallbackVoidWithArgsFunction {
  (arg1: number = 1, args2: string = "2");
}