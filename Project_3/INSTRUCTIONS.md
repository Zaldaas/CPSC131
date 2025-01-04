# Project 3 - Palindrome Detector!

In this project, you'll do three major things:

1. Practice the Adapter/Wrapper pattern

2. Learn to implement both a *Stack* and a *Queue* using an existing underlying data structure

3. Combine your new *Stack* and *Queue* to write a [Palindrome](https://en.wikipedia.org/wiki/Palindrome) detector

In other words, you'll create a custom *Stack* class using a singly linked-list under the hood. You'll also create a custom *Queue* class using a doubly linked-list under the hood. Once finished, you'll use the two new data structures together to detect palindromes.

## Source Files

You'll implement code in several files:

* MyDetector.hpp
* MyDetector.cpp
* MyQueue.hpp
* MyStack.hpp
* sandbox.cpp (optional)

You may add helper functions to classes within the above source files, but do not remove or modify any existing functions.

Unit tests to determine your grade will be run against the code in the above files, and no other files.

The *Makefile*, *main.cpp*, and *CPP_Tests.cpp* files should not be edited.

The *main.cpp* file should not be edited. It is just a very simple interface to help you play around with your palindrome detector. It will not be graded. If you wish to make a custom program to help your debugging/development, you should edit the *sandbox.cpp* file instead.

As before, you can inspect the source files to find lots of instructions written as comments, and a skeleton structure of your implementation files. Use those comments to guide you. You won't need to worry about changing arguments, return types, or method names; All you need to do is complete the body of each function.

## Underlying Data Structures

Your *Stack* will wrap an [std::forward_list](https://en.cppreference.com/w/cpp/container/forward_list) as its underlying data structure. Your *Queue* will wrap an [std::list](https://en.cppreference.com/w/cpp/container/list) as its data structure. This means your *Stack* and *Queue* classes will act as wrappers around another data structure, to get the desired behavior.

Your *Stack* and *Queue* classes should operate with the correct efficiency complexity. In other words, write your code such that the *Big-O* for each operation of your *Stack* and *Queue* classes would be correct (and not slower).

# Hints and Tips

This section contains hints and tips that may help you along your way.

## Detecting a Palindrome

Recall you can detect a palindrome by using both a *Queue* and a *Stack*:

1. Send every character of the string into your *Queue*

2. Send every character of the string into your *Stack*

3. Pull characters back out of your *Queue* and *Stack*, in a loop

    1. If ***all*** characters are identical, you have a palindrome.
    2. If ***any*** characters are mismatched, you do not have a palindrome.

You can detect palindromes containing a mixture of uppercase-lowercase letters, by converting each character to lowercase before adding to the *Stack* and *Queue*. You can also detect more complex palindromes by ignoring characters that aren't letters.

# Execution and Testing

Execution and testing are controlled with a *Makefile* written for [GNU Make](https://www.gnu.org/software/make/). The included Makefile has several targets you can use during development. As mentioned earlier, do not modify the *Makefile* file.

See a help menu with available commands:
```console
$ make help
```

# Submission

As before, we'll be using [Github](https://github.com/) to push code, but [Gradescope](https://www.gradescope.com/) to submit for grading. Do not only submit to Github. If you forget to submit through Gradescope, you will receive a zero grade.

Please note that all grades are subject to further deductions via manual grading, as needed.




