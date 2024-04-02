const id = "MY_EVENT_CALL_NATIVE";
const groups = ["Native Helper"];
const name = "Call Native";

const fields = [
  {
    key: "funName",
    label: "Native Function Name",
    type: "text"
  }
];

const compile = (input, helpers) => {
  const { _callNative } = helpers;
  _callNative(input.funName);
};

module.exports = {
  id,
  name,
  groups,
  fields,
  compile,
};
