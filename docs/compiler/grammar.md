
# conventions

- ```T?```: optional occurrence of T
- ```T+```: one or more interations of T
- ```T_n```: n iterations of T, n >= 0
- ```T*```: Tn where n is not relevant
- (T): T as a whole

# grammar

```
module: function_list
function_list   : function*
function        : "func" "%"id signature "{" function_body "}"
signature       : sig_params "->" type
sig_params      : "()"
                | "(" type_1 ("," type)* ")"
type            : i32
                | i64
                | f32
                | f64
function_body   : extended_basic_block+
```
