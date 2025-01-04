
# Project 4 - Hash Tables!

In this project, you'll do three major things:

1. Practice writing Hashing Functions

2. Practice writing a Hash Table

3. Implement a digital Rolodex (yesssss!)

4. Compete for the top spot on a Gradescope Leaderboard!

## Source Files

You'll implement code in several files:

* MyHashTable.hpp
* MyRolodex.hpp
* MyRolodex.cpp
* sandbox.cpp (optional)

You may add helper functions to classes within the above source files, but do not remove or modify any existing functions. Unit tests to determine your grade will be run against the code in the above files.

The *Makefile*, *main.cpp*, *Address.hpp*, *CPP_Tests.cpp*, and *words.txt* files should not be edited.

The *main.cpp* file should not be edited. It is just a very simple interface to help you play around with your Rolodex. It will not be graded. If you wish to make a custom program to help your debugging/development, you should edit the *sandbox.cpp* file instead.

As before, you can inspect the source files to find lots of instructions written as comments, and a skeleton structure of your implementation files. Use those comments to guide you. You won't need to worry about changing arguments, return types, or method names; All you need to do is complete the body of each function.

## Implementing a Hash Function

You'll remember from lecture that a hash function is simply a function that compresses some input data down to a fixed range of numbers. In this project, we'll use a modified version of something called a "Mid Square" hash, to convert our key into an actual number. The formula here is fairly simple:

1. Define a reasonably high limit for a running `sum` so its value doesn't fly out of control. Since the maximum value of an `unsigned long long int` is `18446744073709551615`, we'll estimate our limit to be just a little lower than its square root, or `18446744073709551615/4294967297`, or roughly `4294967295`. You'll find this in your starter code available as a static member variable `MyHashTable::ULLONG_WRAP_AT`.

2. Start with a `sum` of 1.

3. Iterate over each character in the `std::string` and do the following:

    1. Cast the character to an `unsigned long long int`.

    2. Multiply the `sum` by the value of the current character.

    3. Keep the `sum` under control by applying a modulo against `MyHashTable::ULLONG_WRAP_AT`.

4. You should now have some value for `sum`.

5. Multiply `sum` by itself, to get its square.

6. Keep the "middle digits" of the `sum`, and discard the rest (more explanation on this later). We'll call this new value `hashCode`.

7. Make sure the `hashCode` value stays in-bounds for the current size of your table, by applying a final modulo against the current capacity of your table.

8. This final value is the result of your hashing function.

### Converting std::string to a Number

Our particular hash table will only support keys represented as `std::string`, so let's first decide how to convert an `std::string` into a single number.

Before we can actually take the "mid square" of a number, we need to convert our input key from an `std::string` to a number. For our purposes, this can be done by starting with a `1` and multiplying by the value of each character. We also need to perform a modulo to wrap against `MyHashTable::ULLONG_WRAP_AT` so our number doesn't become too large after each multiplication.

For example, take the string "Hello". We can perform the following steps:

1. Start `sum` with a value of `1`

2. Iterate over each character's numeric representation (hint: *static casting*). Think of this like breaking down each character to the following array: `[72, 101, 108, 108, 111]`.

4. Process `sum` with the first letter's value: `1 * 72 == 72`. Apply modulo of `MyHashTable::ULLONG_WRAP_AT` to make sure our sum stays within a reasonable bound, and end up with `72`. (modulo omitted from further steps unless needed)

5. Process the next digit: `72 * 101 == 7272`

6. Process the next digit: `7272 * 108 == 785376`

7. Process the next digit: `785376 * 108 == 84820608`

8. Process the last digit: `84820608 * 111 == 9415087488`. This result is beyond `MyHashTable::ULLONG_WRAP_AT`, so apply a modulo and end up with `825152898`.

At this point, we've successfully converted our `std::string` key into a number. All that's left now is to square it, then keep the middle digits.

### Keeping the Middle Digits of a Number

For performance reasons, it's probably best if you can figure out some math to keep only the middle digits of some number. However, for the purposes of this project, you're allowed to take the easy route and convert to a string first. Think about the following pattern:

1. Convert your number to an `std::string` using the [std::to_string](https://en.cppreference.com/w/cpp/string/basic_string/to_string) function.

2. Plan on making a new string that is only *half* the first `string`'s length.

3. Create a new `string` using the [std::string::substr](https://en.cppreference.com/w/cpp/string/basic_string/substr) function.

    * The substring should start at the floor of 1/2 of the desired `string`'s length, and continue forward until the new string is half the original `string`'s length.

4. Use the function [stoull](https://www.cplusplus.com/reference/string/stoull/) to convert the string back to an `unsigned long long int`.

For example, suppose we've just converted the `string` `Hello` into the number `825152898`. We can perform the following steps:

1. Square the number: `825152898 * 825152898 == 680877305077798404`

2. Convert the number to a string: `"680877305077798404"`

3. Since the string has 18 characters, our new target string will be 9 characters long. We should take the floor in the case of odd numbers, so a 19 character string would also become a 9 character string.

4. Since our target string is 9 characters long, and we want to keep the middle of the original string, we'll start at an offset of `floor(9 / 2) == 4`

5. We then start with the index-4 character (`7`), and grab characters until we have a new string of length 9.

6. The resulting `string` is `"773050777"`. Notice this is just the "middle" of the square of the original number `680877305077798404`.

7. Convert `"773050777"` back to an `unsigned long long int` with [stoull](https://www.cplusplus.com/reference/string/stoull/).

Don't forget to wrap the result of the "mid square" by the number of rows in your hash table, so it doesn't go out of bounds. Examples:

* `773050777` wrapped to a table of size 100000 would be `50777`

* `773050777` wrapped to a table of size 1000 would be `777`

* `773050777` wrapped to a table of size 1024 would be `409`

## Implementing the Hash Table

For the most part, your hash table will follow the form presented in lecture. Some of the function prototypes are a bit different to make things interesting, but the general concept is the same.

### Difference Between Size and Capacity

For the purposes of this project, we'll say that `size` is the number of items currently inside our hash table, while `capacity` is the number of rows in our hash table. This may be a little misleading at first, because technically our hash table can store more than its capacity, through the use of collisions.

(also remember that collisions are handled through the use of a singly linked list sitting in each row)

## Implementing the Rolodex

Once again, we're going to use the wrapper pattern. You'll see the `MyRolodex` class contains all the functions you'd probably expect from a Rolodex, which are very similar to the functionality your hash table already provides. For the most part, you'll simply wrap each function of the `MyRolodex` class around a call to your `MyHashTable`. Easy as Pi.

## Gradescope Leaderboard

You may notice several empty functions inside `MyHashTable`:

* `myCustomHashFunction1`

* `myCustomHashFunction2`

* `myCustomHashFunction3`

* `myCustomHashFunction4`

Although the unit tests will primarily grade your submission against the `midSquareHash` function, you can also play around with the four custom functions above. None of the four custom hash functions will improve your test scores, BUT .... implementing something really good ***might*** result in bonus points to your overall projects category score!

Try to use each of the four functions to experiment with making your very own custom hashing functions. You may search the internet for formulas and algorithms, but don't copy any code (else you may risk plagiarism). The unit tests will determine how many collisions your hashing functions cause. Remember: One good property for a hashing function is that it produces the fewest amount of collisions as possible.

Once finished, the unit tests will take your *best* hashing function (read as: The one that produces the least number of collisions), and upload the number of collisions it produces, to Gradescope. Each student/group's best hashing function will be placed onto a Leaderboard hosted by Gradescope, for everyone to see. When this assignment is finished, the ***top three submissions*** (including any tie for 3rd place) will earn a small bonus to each student's semester grade, placed in the project category (amount: TBD).

It should go without saying that you shouldn't try to cheat the system by using any pre-made hashing functions that may or may not be available within C++ or other libraries. Doing so may be considered academic dishonesty.

*Just to clarify: Gradescope won't show your entire project grade to everyone; Only your custom hashing function's performance will be shown.*

# Hints and Tips

This section contains hints and tips that may help you along your way.

## Using std::forward_list::erase_after

We handle collisions by making each row of your hash table an `std::forward_list`. But you may notice that an `std::forward_list` doesn't have an `erase` function! Instead, you'll have to use a function called `erase_after`, which takes an Iterator to an element, and erases the element *after* that iterator.

This doesn't sound too hard at first. You could easily erase any element by simply iterating through the list, making a copy of the iterator at each step that can be incremented to peek at the next element. Once your peek-ahead iterator see's the element to be deleted, you just call `std::forward_list::erase_after` on the original iterator.

But what if the element to be erased is at the beginning of your list? In this case, you can grab an iterator that starts *before* the beginning of your list by calling `std::forward_list::before_begin` instead of `std::forward_list::begin`.

See the following example for more information: [before_begin](https://www.cplusplus.com/reference/forward_list/forward_list/before_begin/)

# Execution and Testing

Execution and testing are controlled with a *Makefile* written for [GNU Make](https://www.gnu.org/software/make/). The included Makefile has several targets you can use during development. As mentioned earlier, do not modify the *Makefile* file.

See a help menu with available commands:
```console
$ make help
```

# Submission

As before, we'll be using [Github](https://github.com/) to push code, but [Gradescope](https://www.gradescope.com/) to submit for grading. Do not only submit to Github. If you forget to submit through Gradescope, you will receive a zero grade.

Please note that all grades are subject to further deductions via manual grading, as needed.


