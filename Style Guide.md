# Style Guide (WIP)


## Chapter 1: Indentation

Tabs are 8 characters, and thus indentations are also 8 characters. Outside of
comments, and in documentation, tabs are the preferred indentation method.

Align "case" labels with their "switch" statement, e.g.:

```C
	switch (suffix) {
	case 'G':
	case 'g':
		mem <<= 30;
		break;
	case 'M':
	case 'm':
		mem <<= 20;
		break;
	case 'K':
	case 'k':
		mem <<= 10;
		/* fall through */
	default:
		break;
	}
```

Don't put multiple statements on a single line. Each line should do only one
thing, that is to say that this:

```C
	if (condition) do_this;
	  do_something_everytime;
```

is less preferable to this:

```C
	if (condition)
		do_this;
	do_something_everytime;
```

Whitespace should not be left at the end of a line, and there should be a new
line at the end of a file.

## Chapter 2: Breaking long lines and strings

The preferred limit on the length of a line is 80 characters, however, if by
extending this limit you increase the readability of the code, then you may do so.

## Chapter 3: Placing Braces and Spaces

The other issue that always comes up in C styling is the placement of braces.
The preferred way, is to put the opening brace last on the line, and put the
closing brace first, thusly:

```C
	if (x is true) {
		do_y;
		do_z;
	}

	switch (action) {
	case KOBJ_ADD:
		return "add";
	case KOBJ_REMOVE:
		return "remove";
	case KOBJ_CHANGE:
		return "change";
	default:
		return NULL;
	}

	int function(int x) {
		...
	}
```

The only time a closing brace shouldn't be on a lint of it's own is when it's a
part of a do {} while loop, or in the middle of a conditional statement, e.g.:

```C
	do {
		...
	} while (condition);

	if (x == y) {
		..
	} else {
		....
	}
```

Do not unnecessarily use braces on a single statement. This does not apply if
only one of the branches contains a single statement, e.g.:

```C
	if (condition)
		action();
	if (other_condition)
		do_this();
	else
		do_that();

	if (some_condition) {
		do_this();
		do_that();
	} else {
		otherwise();
	}
```
### 3.1:  Spaces

Use a space after these:

```C
	if, switch, case, for, do, while
```

Spaces should not be used right after, or right before, parenthises, other than
in the above exceptions.

The preferred placement of the '\*' when declaring a pointer, is to put it next
to the variable, and not the type, as in:

```C
	char *linux_banner;
	unsigned long long memparse(char *ptr, char **retptr);
	char *match_strdup(substring_t *s);
```

Use one space on each side of most binary and ternary operators, such as these:

```C
	=  +  -  <  >  *  /  %  |  &  ^  <=  >=  ==  !=  ?  :
```

but no space after/before unary operators, and element selectors:

```C
	&  *  +  -  ~  !  -- ++ . ->
```

## Chapter 4: Naming

Multi-worded functions, variables, structs, etc. should follow the camelCase
naming convention, e.g.

```C
	void reallyLongNameFunction(int reallyLongVariableName) {
		...
	}
```

Global and #define'd values should be all uppercase, as in:

```C
	#define MYDEFINE 1
	const int GLOBALVAL = 10;
```

## Chapter 5: Typedefs

Typedefs shouldn't be used. They add an extra layer of obscurity that should be
avoided.

## Chapter 6: Functions

The point of a function is to do one thing, and one thing only. If you find
that a function is doing too much, try to split it up into two, or more, functions.
The exception to this is when a single function has a long switch statement, or
multiple if/else if statements that cannot be moved out of the function.

## Chapter 7: Centralized exiting of functions

The goto statement comes in handy when a function exits from multiple
locations and some common work (such as cleanup) has to be done. If there is no
cleanup needed then just return directly. If using a goto statement, give the
label a name which says what that goto does, or why it exists.

```C
	int fun(int a)
	{
		int result = 0;
		char *buffer;

		buffer = kmalloc(SIZE, GFP_KERNEL);
		if (!buffer)
			return -ENOMEM;

		if (condition1) {
			while (loop1) {
				...
			}
			result = 1;
			goto out_buffer;
		}
		...
	out_buffer:
		kfree(buffer);
		return result;
	}
```

## Chapter 8: Commenting

Generally, you want comments to tell WHAT your code does, not HOW.

```C
	/*
	 * This is the preferred style for multi-line
	 * comments.
	 * Please use it consistently.
	 *
	 * Description:  A column of asterisks on the left side,
	 * with beginning and ending almost-blank lines.
	 */
```

It's also important to comment data, whether they are basic types or derived
types. To this end, use just one data declaration per line (no commas for
multiple data declarations). This leaves you room for a small comment on each
item, explaining its use.

## Chapter 9: Use of inline

You shouldn't inline functions that have more than 3 lines of code in them.

Don't use inline assembly if C can do the job. You should poke hardware from C
when possible. Large, non-trivial assembly functions should go in .asm files,
with corresponding C prototypes defined in header files.

Don't unneccessarily mark asm as volatile.

When writing a single inline assembly statement containing multiple
instructions, put each instruction on a separate line in a separate quoted
string, and end each string except the last with \n\t to properly indent the
next instruction in the assembly output:

```C
	asm ("magic %reg1, #42\n\t"
	     "more_magic %reg2, %reg3"
	     : /* outputs */ : /* inputs */ : /* clobbers */);
```

## Chapter 10: Function return values and names

One of the most common return value is indicating whether the function succeeded
or failed. Such a value can be represented as an error-code integer (-Exxx =
failure, 0 = success) or a "succeeded" boolean (0 = failure, non-zero = success).

Mixing up these two sorts of representations is a ource of difficult-to-find bugs,
as C doesn't have a common way of distinguishing between the two types. To help
prevent such bugs, follow this:

>	If the name of a function is an action or an imperative command,
>	the function should return an error-code integer. If the name
>	is a predicate, the function should return a "succeeded" boolean.

For example, "add work" is a command, and the add_work() function returns 0
for success or -EBUSY for failure. In the same way, "PCI device present" is
a predicate, and the pci_dev_present() function returns 1 if it succeeds in
finding a matching device or 0 if it doesn't.

Functions whose return value is the actual result of a computation, rather
than an indication of whether the computation succeeded, are not subject to
this rule.

## Chapter 11: Conditional Compilation

It is preferred that you don't use preprocessor conditionals in .c files, as
doing so makes the code harder to read, and logic harder to follow. If you must
use them, place them in a header file which you can then include in those .c
files. It is also preferred that, if you use a preprocessor conditional, that
you use it around the entire function, instead of just a few lines in the middle
of the function.

At the end of any non-trivial #if or #ifdef block (more than a few lines),
place a comment after the #endif on the same line, noting the conditional
expression used. For instance:

	#ifdef CONFIG_SOMETHING
	...
	#endif /* CONFIG_SOMETHING */

## Appendix I: References

Linux Kernel Style Guide

