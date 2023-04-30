# PanaScript
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


