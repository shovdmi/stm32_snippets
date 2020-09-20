tatic uint32_t v;
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |    (value to be written)
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |    (register value)
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |    (mask)

w_tbl [i] = ((m[i] == 1) && (v[i] == 1)) ? 1 : r[i];
t_tbl [i] = ((m[i] == 1) && (v[i] == 1)) ? 1 : r[i];


## Toggleable on write bits

### Set bits

If we set "toggleable" bits in a register we treat other bits as read-write.
If the register value is binary                reg : "00110011"
All the toggleable bits are described by     t_mask: "01010101"
                                                       | | | |
This means we only touch next bits                   "_0_1_0_1" (let''s call it "t_reg")
                                                       | | | |
If we want to set bits described by value            "00001111" ,
next value''s bits must affect on                      | | | |
  the toggleable bits only (let''s call it "t_val")  "_0_0_1_1" (i.e. in accordance to "t_mask")
we treat the rest as writable bits                   "0_0_1_1_" and we keep their values

We keep writable bits untouch on the step where we change toggleable bits.
We only set t_reg''s bits where t_val bits are 1     
i.e.                                           t_val "_0_0_1_1"
                                               t_reg "_0_1_0_1"
                                                       | | ^-this one
So in the end of the day we want these values          | | | |
of toggleable bits to be in the register:            "_0_1_1_1" (let''s call it "t_value")

The main point of this explanation is that if we write to the register directry,
all the toggleable bit make an XOR operation.
I.e. t_reg xor reg :      t_reg "_0_1_0_1" 
                     bitwise-xor  | | | |
                        t_value "_0_1_1_1"
                          gives "_0_0_1_0" bits value in the register
And that is not what we want.
There is a XOR properties: (x^y)^y == x. So if we want t_value to be the value of register
after writing into it, we may calculate "x_value" by xor-ing t_reg and t_value.
I.e.
t_value = t_reg ^ x_value, we find x_value by xor-ing both sides of the equation:
t_value ^ t_reg = t_reg ^ x_value ^ t_reg,
t_value ^ t_reg = x_value ^ t_reg ^ t_reg = x_value ^ 0 = x_value
so x_value = t_value ^ t_reg, calculated new value of register XOR it''s current value.

reg <-- x_value gives us : "_0_1_0_1" ^ "_0_0_1_0" = "_0_1_1_1" 

### Clear bits

In that case our e

## Regular Read-Write bits

When we set "rw" bits we treat the rest as "rw" which values we must keep unchanged.
i.e. for "rw" x_value <-- (value & w_mask) | reg;
              reg <-- x_value; 
