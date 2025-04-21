dwasm spec test
==============

# Docs

* https://yuque.antfin.com/twdn2z/project/lz3qrrgt42r61g78

# Dwasm spec errors cases

* Special Cases

Some test cases are difficult to construct in cli, but are easier to verify on-chain and have deterministic behavior that can be controlled. These cases will be skipped during cli but should be implemented in chain.

* Bytecode too large, error code 10100
* Invalid bytecode format, error code 10097
* Incorrect LEB integer value in bytecode, error code 10096
* Insufficient gas, error code 90099
