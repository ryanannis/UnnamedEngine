#pragma once

// Call a function from the compilation unit of each statically registered component
// to ensure that it gets statically registered before being used.

int StaticRegister();
