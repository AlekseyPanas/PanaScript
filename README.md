# PanaScript

## Description & Motivation
A simple language which compiles into MIPs assembly, specifically to be run in the simulator software called MARs or Saturn.

The motivation for the project was primarily for my own learning benefit, however, it deals directly with the course content for the UofT CSC258 course so it might be helpful there as well.

Here is some sample syntax

```
fn square_cube(w num) -> w, w {
return num * num, num * num * num;
}

b PRINT = 0
b x = 5 * 10;
w o = read_keyboard();
if (x > 4) {
	syscall(PRINT, x);
	b i = 0;
	b j = 0;
while (i < 127) {
	while (j < 127) {
		draw_pixel(0, 0, 255, to_w(i), to_w(j));
		j++;
}
i++;
}
	draw_pixel(0, 0, 255, 0, 0);
} elif (x <= 4 & o >= 0) {
	syscall(PRINT, o);
}
```

## Language Features

### Variable Assignment

```
w x;   // Declare a word-sized variable called x
h y;   // Declare a half word-sized variable called y
b z:   // Declare a byte-sized variable called z

b a = 3;   // Declare and assign using decimal literal
a = 0x5;   // Reassign using hex literal
a = 0b1011;  // Reassign using binary literal

w g, h, i = 0;   // Declare and assign multiple variables to 0

g++;   // Increment
g--;   // Decrement

w arr[10] = 5;   // Create a word array of ten 5's
b arr2[5];       // Declare a byte array of size 5
h arr3[3] = 55, 0x0200, 0b11011;   // Create half word array with 3 custom values

arr[2] = 4;    // Assign to array index
```

All the following shorthand assignments exist:  
`+=` 
`-=`
`*=`
`/=`
`++`
`--`

### Expression Operators

`>` signed gt  
`>=` signed gte  
`<=` signed lte  
`<`  signed le  
`==` equal to  
`!=` not equal to  
`+`  add  
`-` subtract  
`/` divide  
`%` modulo
`*` multiply  
`<<`  shift left logical   
`>>`  shift right logical  
`>>>` shift right arithmetic   
`&` bitwise and  
`|` bitwise or  
`!` bitwise not  



Literals like `0`, `0x44`, `0b101`, are typed based on context:
- If passed as a parameter, they are typed to the parameter. 
- If assigned to a variable, they are typed based on the variable.
- If in an expression with at least one variable, typed to match that variable. 
- If an expression has two variables which don't match, an error occurs
- If in an expression with no variables, all are typed as the longest of all the literals in the expression
- If no context is available, type based on length (If fits into byte, type as byte. Otherwise check half word, then word)


### Functions

```
fn main() {
    // entry point
}

// General Funtion Definition
fn fn_name (<b/w/h> p1, <b/w/h> p2, ...) -> [b/w/h], [b/w/h], ... {
    [return [...];]
}

// Example: Return first 3 multiple, and 1 if num is even else 0
fn multiples (w num) -> w, w, w, b {
    b r = num % 2;
    return w * 2, w * 3, w * 4, r == 0;
}

// Void function example
fn print_five() {
    syscall(0, 5);
    return;  // Can be omitted
}

// Calling and unpacking example
w m1, m2, m3;
b is_even;
m1, m2, m3, is_even = multiple(5);    // Unpacks return into 4 variables
_, _, _, is_even = multiple(5);     // _ is special for ignoring a return value
```

### If Statements

```
if (<condition expression>) {

} elif (<condition expression>) {

} else {

}
```
If the condition expression evaluates to 0, the block won't run. Otherwise it runs.

### Loops

```
while (<condition expression>) {

}
```

### Builtin Functions

`syscall(w id, w p1, w p2, ...) -> w, ...`  
Make a system call with parameters p1, p2 and return value w. Note that some syscalls return nothing, and different syscalls require different numbers of parameters

`set_screen_dims(w width, w height)`   
Set 2D pixel dimensions of screen

`set_keyboard_addr(w addr)`  
Set address of where to read keyboard from

`set_screen_addr(w addr)`   
Set address of first screen pixel (topleft)

`read_keyboard() -> w`   
Returns -1 if not pressed, -2 if addr not set, otherwise ascii key id of key pressed

`write_pixel(b red, b green, b blue, w x, w y) -> w`    
Returns -1 if addr not set, -2 if dims not set, 0 if success. Draws pixel at position x,y with provided color

`to_w(<h|b|w> val) -> w`     
Convert to word. Sign extends the given value to word length

`to_b(<h|b|w> val) -> b`     
Convert to byte. Takes least significant byte

`to_h(<h|b|w> val) -> h`    
Convert to half word. Sign extend or take least significant two bytes
