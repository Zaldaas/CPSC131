# Project 2 - Book Store (DLL Adapter)

In this project, you'll do two major things:

1. Implement a custom doubly linked list class named ```DoublyLinkedList```

2. Implement a wrapper class (aka an adapter) named ```BookStore``` that allows user code to interact directly with a simulation of a bookstore, while avoiding direct interaction with an actual ```DoublyLinkedList```.

In other words, you'll write a bookstore class that is *powered by* a doubly linked list, under the hood. The book store will maintain a list of books, allow the user to add new books to inventory, sell books to a customer, and view a list of all books in the inventory.

## Source Files

You'll implement code in several files:

* DoublyLinkedList.hpp
* BookStore.hpp (optional)
* BookStore.cpp

Unit tests to determine your grade will be run against the code in the above files, and no other files.

You won't need to edit *Book.hpp* or *Book.cpp*, though you can add helper methods there if you wish, for some reason. The *Makefile* and *CPP_Tests.cpp* files should not be edited.

The *main.cpp* files doesn't need to be edited. It is just a very simple interface to help you play around with your bookstore. You can edit it if you want, or not; It won't be graded.

As before, you can inspect the source files to find lots of instructions written as comments, and a skeleton structure of your implementation files. Use those comments to guide you. You won't need to worry about changing arguments, return types, or method names; All you need to do is complete the body of each function.

Note that all methods within the *DoublyLinkedList* source file should be *inline* (body present where the declaration occurs), but the *Book* and *BookStore* classes use two different source files (one for declaration; one for implementation).

## Resources for Learning

You may research [other implementations](https://en.cppreference.com/w/cpp/container) of a doubly linked list. Just ***be very careful not to plagiarize code***, as discussed previously.

# Execution and Testing

Execution and testing are controlled with a *Makefile* written for [GNU Make](https://www.gnu.org/software/make/). The included Makefile has several targets you can use during development. As mentioned earlier, do not modify the *Makefile* file.

See a help menu with available commands:
```console
$ make help
```

See a quick preview/estimate of your grade with the following command:
```console
$ make test
```

Execute the program defined by *main.cpp*:
```console
$ make run
```

Run tests in debug mode (with gdb):
```console
$ make debug-test
```

# Submission

As before, we'll be using Github to push code, but Gradescope to submit for grading. Do not only submit to Github. If you forget to submit through Gradescope, you will receive a zero grade.

Please note that all grades are subject to further deductions via manual grading, as needed.

# Group Work

If working in a group, all group members are responsible for making sure all work is completed on time. No extensions will be given when a group member does not contribute, so make sure someone picks up the slack. You may email the professor about a group member failing to contribute their fair share to have their grade reduced.

