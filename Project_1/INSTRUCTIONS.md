
# Instructions

In this project, you'll implement a custom vector class named ```MyVector```. The ```MyVector``` class will have much of the functionality of the familiar ```std::vector``` class, including the ability to dynamically resize.

## Source Files

All your code should be implemented in the *MyVector.hpp* file. Only code written in the *MyVector.hpp* file will be considered for your grade.

If you inspect the *MyVector.hpp* file, you'll notice a skeleton structure has been prepared for you. All required class methods have been declared with empty bodies. You won't need to worry about arguments, return types, or method names; All you need to do is complete the body of each function. Note that all methods should be *inline* (body present where the declaration occurs). Read the comment blocks above each method carefully, as they will help explain how your vector implementation should behave.

If you wish, you may modify and use *main.cpp* to help debug while you develop ```MyVector```, as it won't be considered for grading. This file is basically empty and does not need to be developed unless you think it will be useful to debug your ```MyVector``` implementation as you work.

Of course, per the *Code Submission Guidelines* document, you'll need to create a *README.md* file according to specifications.

***Do not modify*** any other files in your repository.

## Resources for Learning

You may research the ```FixedVector``` code repository discussed during lecture to help guide you. However, ***be very careful not to plagiarize code***. A good test to determine whether you have unintentionally plagiarized code, is whether you are able to write all the code from scratch *without* referencing someone else's code. If you find yourself repeatedly getting stuck until you reference someone else's code, it may be a sign you need to study a bit more and start your project again from scratch to be safe.

You may also draw inspiration from explanations of the standard ```std::vector``` class on reference websites like [cppreference.com](https://en.cppreference.com/w/cpp/container/vector). You may also want to study our slideshows.

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

Execute the program defined by *main.cpp* (which initially does nothing and is only there for you to debug your vector):
```console
$ make run
```

# Submission

You should push your code to Github during development on a regular basis. When you are ready to submit your code for grading, you will submit through [Gradescope.com](https://www.gradescope.com/). Your instructor has already linked Canvas with Gradescope, so you will (hopefully) find you already have an account at Gradescope using your school email address. You might need to verify your email address there.

Once logged in, you should see entries for this class and this assignment. Within the assignment page, you can link your Github account to Gradescope, allowing you to quickly submit your project repository for grading. Gradescope will automatically process your code and send your grade directly to Canvas.

You may submit your code as many times as you want while you develop; Only the most recent submission will count towards your grade.

This means your workflow might look similar to the following:

1. Develop code
2. Push to Github whenever you make significant progress.
3. Go back to Step 1 if needed
4. Submit code to Gradescope
5. If grade earned is acceptable, finish. Otherwise, go back to Step 1

If you forget to submit through Gradescope, you will receive a zero grade.

Please note that all grades are subject to further deductions via manual grading, as needed.

## Due Date Extensions

Please also note, extensions will generally not be given. ***Do not wait*** to the last days to work on your project, and ***do not wait*** to the last day to submit your code. Issues with your computer, wifi, Gradescope, Github, or most other things will not result in a due date extension. In other words: Plan to ***submit your project early!***

Remember: You have a lot of time to complete your project. Take advantage of that time.

|Excuse                                                          |Result                                                     |
|----------------------------------------------------------------|-----------------------------------------------------------|
|Gradescope was down for 3 hours and I couldn't submit!          |No extension                                               |
|My computer broke!                                              |That's really sad but: No extension                        |
|My wireless was down all day!                                   |What was your excuse all the other days? (No extension)    |
|I didn't realize I was supposed to XYZ.                         |You also didn't realize you were going to get No extension |
|It was really hard to learn everything so close to the deadline |Wow, really? (No extension)                                |
|I'm in the hospital                                             |Extension granted, and well wishes                         |

# Group Work

If working in a group, all group members are responsible for making sure all work is completed on time. No extensions will be given when a group member does not contribute, so make sure someone picks up the slack. You may email the professor about a group member failing to contribute their fair share to have their grade reduced.














